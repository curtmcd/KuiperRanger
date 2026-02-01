// SDL2-based sound module

#include "sound.hpp"
#include "rand.hpp"

#include <map>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>

// This module uses only floats, since that's the audio sample type
static const float PIf = 3.1415927f;

static const int SAMPLE_RATE = 44100;
static const int SAMPLE_BUFFER_DEFAULT = 1024;

static SDL_AudioDeviceID dev = 0;
static SDL_AudioSpec spec;

// IIR Biquad filter (Direct Form 1): low pass, high pass and band pass
// Formulas from the "Audio EQ Cookbook" by Robert Bristow-Johnson.
class Biquad {
private:
    float a1, a2, b0, b1, b2;    	// Filter coefficients
    float x_z1, x_z2, y_z1, y_z2;    	// Filter state (delay line)

public:
    Biquad() : a1(0), a2(0), b0(0), b1(0), b2(0),
               x_z1(0), x_z2(0), y_z1(0), y_z2(0) {}

    void setLowPass(float freq, float Q)
    {
        if (freq <= 0.0f)
	    freq = 1.0f;
        if (freq >= SAMPLE_RATE / 2.0f)
	    freq = (SAMPLE_RATE / 2.0f) - 1.0f;
        if (Q <= 0.0f)
	    Q = 0.1f;

        float omega = 2.0f * PIf * freq / SAMPLE_RATE;
        float cosOmega = std::cos(omega);
        float alpha = std::sin(omega) / (2.0f * Q);
        float a0Inv = 1.0f / (1.0f + alpha);

        b0 = (1.0f - cosOmega) / 2.0f * a0Inv;
        b1 = (1.0f - cosOmega) * a0Inv;
        b2 = (1.0f - cosOmega) / 2.0f * a0Inv;
        a1 = -2.0f * cosOmega * a0Inv;
        a2 = (1.0f - alpha) * a0Inv;
    }

    void setHighPass(float freq, float Q) {
        if (freq <= 0.0f)
	    freq = 1.0f;
        if (freq >= SAMPLE_RATE / 2.0f)
	    freq = (SAMPLE_RATE / 2.0f) - 1.0f;
        if (Q <= 0.0f)
	    Q = 0.1f;

        float omega = 2.0f * PIf * freq / SAMPLE_RATE;
        float cosOmega = std::cos(omega);
        float alpha = std::sin(omega) / (2.0f * Q);
        float a0Inv = 1.0f / (1.0f + alpha);

        b0 = (1.0f + cosOmega) / 2.0f * a0Inv;
        b1 = -(1.0f + cosOmega) * a0Inv;
        b2 = (1.0f + cosOmega) / 2.0f * a0Inv;
        a1 = -2.0f * cosOmega * a0Inv;
        a2 = (1.0f - alpha) * a0Inv;
    }

    // Process one sample
    inline float process(float in) {
        float out = b0 * in + b1 * x_z1 + b2 * x_z2 - a1 * y_z1 - a2 * y_z2;

        x_z2 = x_z1;
        x_z1 = in;
        y_z2 = y_z1;
        y_z1 = out;

        return out;
    }
};

// Waveform generators

struct Waveform {
    Waveform() {};
    Waveform(size_t samples) : buf(samples) {};

    Waveform& volume(float level)
    {
	for (size_t i = 0; i < buf.size(); i++)
	    buf[i] *= level;

	return *this;
    }

    Waveform& decay(float rate)
    {
	for (size_t i = 0; i < buf.size(); i++) {
	    float t = (float)i / (float)buf.size();
	    buf[i] *= std::pow(1.0f - t, rate);
	}

	return *this;
    }

    // modulates the waveform's amplitude by one full period of a sine
    // wave scaled between minLevel and maxLevel.
    Waveform& amplitudeModulate(float minLevel, float maxLevel)
    {
	for (size_t i = 0; i < buf.size(); i++) {
	    float t = (float)i / (float)buf.size();
	    float atten = ((1.0f + std::sin(2.0f * PIf * t)) / 2.0f *
			   (maxLevel - minLevel) + minLevel);
	    buf[i] *= atten;
	}

	return *this;
    }

    // Band-pass filter
    // Default Q is 1/sqrt(2) for a Butterworth filter.
    Waveform& bandPass(float lowFreq, float highFreq, float Q = 0.707f)
    {
	const float nyquist = SAMPLE_RATE / 2.0f;

	bool useHighPass = (lowFreq > 0.0f);
	bool useLowPass = (highFreq < nyquist);

	Biquad hpf;
	if (useHighPass)
	    hpf.setHighPass(lowFreq, Q);

	Biquad lpf;
	if (useLowPass)
	    lpf.setLowPass(highFreq, Q);

	for (size_t i = 0; i < buf.size(); i++) {
	    float sample = buf[i];

	    // Chain the filters (order: HPF -> LPF)
	    if (useHighPass)
		sample = hpf.process(sample);
	    if (useLowPass)
		sample = lpf.process(sample);

	    buf[i] = sample;
	}

	return *this;
    }

    std::vector<float> buf;
};

Waveform silence(float seconds)
{
    size_t samples = size_t(seconds * SAMPLE_RATE);
    Waveform w(samples);

    for (size_t i = 0; i < samples; i++)
	w.buf[i] = 0.0;

    return w;
}

Waveform tone(float freq, float seconds)
{
    size_t samples = size_t(seconds * SAMPLE_RATE);
    Waveform w(samples);
    float phase = 0.0f;

    for (size_t i = 0; i < samples; i++) {
	w.buf[i] = std::sin(phase);
	phase += 2.0f * PIf * freq / SAMPLE_RATE;
    }

    return w;
}

Waveform slide(float startFreq, float endFreq, float seconds)
{
    size_t samples = size_t(seconds * SAMPLE_RATE);
    Waveform w(samples);
    float phase = 0.0f;

    for (size_t i = 0; i < samples; i++) {
	w.buf[i] = std::sin(phase);
	float t = (float)i / (float)samples;
	float freq = startFreq * (1.0f - t) + endFreq * t;
	phase += 2.0f * PIf * freq / SAMPLE_RATE;;
    }

    return w;
}

// Firing "pew pew" sound, frequency modulated with exponential decay.
Waveform pew(float startFreq, float endFreq, float seconds)
{
    size_t samples = size_t(seconds * SAMPLE_RATE);
    Waveform w(samples);
    float phase = 0.0f;

    for (size_t i = 0; i < samples; i++) {
	w.buf[i] = std::sin(phase);
	float t = (float)i / (float)samples;
	float freq = startFreq * std::pow(endFreq / startFreq, t);
	phase += 2.0f * PIf * freq / SAMPLE_RATE;;
    }

    return w;
}

// Make one cycle for one full period of a frequency-modulated siren
Waveform siren(float minFreq, float maxFreq, float seconds)
{
    size_t samples = size_t(seconds * SAMPLE_RATE);
    Waveform w(samples);
    float phase = 0.0f;

    for (size_t i = 0; i < samples; i++) {
	w.buf[i] = std::sin(phase);
	float t = (float)i / (float)samples;
	float freq = ((1.0f + std::sin(2.0f * PIf * t)) / 2.0f *
		      (maxFreq - minFreq) + minFreq);
	phase += 2.0f * PIf * freq / SAMPLE_RATE;
    }

    return w;
}

Waveform noise(float seconds)
{
    size_t samples = size_t(seconds * SAMPLE_RATE);
    Waveform w(samples);

    for (size_t i = 0; i < samples; i++)
	w.buf[i] = Rand::rangef(-1.0f, 1.0f);

    return w;
}

Waveform operator+(const Waveform& lhs, const Waveform&rhs)
{
    Waveform result = lhs;

    result.buf.insert(result.buf.end(), rhs.buf.begin(), rhs.buf.end());

    return result;
}

static std::mutex mtx;
static std::atomic<bool> enabled{false};
static std::atomic<bool> initialized{false};

struct Playing {
    Sound::Effect effect;
    size_t pos;
    bool loop;
};

static std::map<Sound::Effect, Waveform> sounds;
static std::vector<Playing> playing;

// SDL audio callback
static void callback(void *, Uint8 *stream, int len)
{
    float *out = (float *)stream;
    size_t samples = len / sizeof (float);

    std::fill(out, out + samples, 0.0f);

    std::lock_guard<std::mutex> lock(mtx);

    float clampLow, clampHigh;
    if (!enabled) {
	clampLow = 0.0f;
	clampHigh = 0.0f;
    } else {
	clampLow = -1.0f;
	clampHigh = 1.0f;
    }

    for (size_t i = 0; i < samples; i++) {
	float mix = 0.0f;

	for (auto& p : playing) {
	    auto& buf = sounds[p.effect].buf;
	    if (p.pos < buf.size())
		mix += buf[p.pos++];
	    else if (p.loop) {
		p.pos = 0;
		mix += buf[p.pos++];
	    }
	}

	out[i] = std::clamp(mix, clampLow, clampHigh);
    }

    // Remove finished sounds
    playing.erase(std::remove_if(playing.begin(), playing.end(),
				 [](const Playing& p)
				 {
				     return (!p.loop &&
					     p.pos >= sounds[p.effect].buf.size());
				 }),
		  playing.end());
}

void Sound::init()
{
    if (initialized)
	return;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
	std::cerr << "SDL audio init failed: " << SDL_GetError() << "\n";
	return;
    }

    const char *sampleOverride = getenv("AUDIO_SAMPLES");
    int samples;

    if (sampleOverride)
	samples = atoi(sampleOverride);
    else
	samples = SAMPLE_BUFFER_DEFAULT;

    SDL_AudioSpec want{};
    want.freq = SAMPLE_RATE;
    want.format = AUDIO_F32SYS;
    want.channels = 1;
    want.samples = (Uint16)samples;
    want.callback = callback;

    dev = SDL_OpenAudioDevice(nullptr, 0, &want, &spec, 0);
    if (!dev) {
	fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return;
    }

    SDL_PauseAudioDevice(dev, 0);

    // Precompute all waveforms

    sounds[shipFire] = pew(1800.0f, 400.0f, 0.2f).volume(0.3f).decay(2.0f);

    sounds[alienFire] = pew(2400.0f, 1000.0f, 0.4f).volume(0.2f).decay(2.0f);

    sounds[rockExplode] = noise(0.75f).bandPass(0.0, 200.0).volume(2.0f).decay(3.0f);

    sounds[shipExplode] = noise(2.0f).bandPass(0.0, 240.0).volume(3.0f).decay(2.0f);

    sounds[alienPoints] = (tone(1000.0f, 0.1f) +
			   tone(1200.0f, 0.1f) +
			   tone(1400.0f, 0.1f)).volume(0.5f);

    sounds[alienExplode] = noise(0.75f).bandPass(0.0, 400.0).volume(2.0f).decay(2.0f);

    sounds[alienMotor] = (siren(2000.0f, 2400.0f, 0.2f).
			  volume(0.05f).
			  amplitudeModulate(0.5f, 1.0f));

    sounds[extraShip] = (tone(2500.0f, 0.1f) +
			 silence(0.1f) +
			 tone(2500.0f, 0.1f) +
			 silence(0.1f) +
			 tone(2500.0f, 0.1f)).volume(0.5f);

    sounds[lowDeet] = tone(90.0f, 0.15f).volume(0.6f);

    sounds[highDeet] = tone(100.0f, 0.15f).volume(0.3f);

    sounds[shipThrust] = noise(0.2f).bandPass(200.0, 400.0).volume(0.5f);

    sounds[inHyperspace] = slide(60.0, 1000.0, 0.25f).volume(0.5f);

    sounds[outHyperspace] = slide(1000.0, 60.0, 0.25f).volume(0.5f);

    enabled = false;		// default off
    initialized = true;
}

void Sound::term()
{
    if (!initialized)
	return;

    SDL_PauseAudioDevice(dev, 1);
    SDL_CloseAudioDevice(dev);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    dev = 0;

    {
        std::lock_guard<std::mutex> lock(mtx);
        playing.clear();
        sounds.clear();
    }

    enabled = false;
    initialized = false;
}

bool Sound::available()
{
    return initialized;
}

void Sound::on()
{
    enabled = true;
}

void Sound::off()
{
    enabled = false;
}

bool Sound::isOn()
{
    return enabled;
}

void Sound::play(enum Effect effect, bool loop)
{
    if (!initialized)
	return;

    std::lock_guard<std::mutex> lock(mtx);

    // Don’t stack multiple of the same looping sound
    if (loop)
	for (auto& p : playing)
	    if (p.effect == effect && p.loop)
		return;

    playing.push_back({effect, 0, loop});
}

void Sound::stop(enum Effect effect)
{
    std::lock_guard<std::mutex> lock(mtx);

    playing.erase(std::remove_if(playing.begin(), playing.end(),
				 [effect](const Playing& p) {
				     return p.effect == effect;
				 }),
		  playing.end());
}
