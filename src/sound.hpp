#ifndef sound_hpp
#define sound_hpp

#include "type.hpp"

struct Sound {
    enum Effect {
	shipFire,	// middle-high frequency decaying "pew"
	alienFire,	// high frequency decaying "ping"
	rockExplode,	// explosion with quick decay
	shipExplode,	// louder explosion with long decay
	alienPoints,	// quick ascending bee-bee-beep
	alienExplode,	// explosion with medium-quick decay
	alienMotor,	// one cycle of siren, ~1000 Hz, modulating at ~5 Hz
	extraShip,	// loud, high frequency "beep beep beep"
	lowDeet,	// the background noise (low-freq short beep)
	highDeet,	// the background noise (middle-low short beep)
	shipThrust,	// one cycle of ship thrust (subdued white noise)
	inHyperspace,	// ship goes into hyperspace
	outHyperspace,	// ship exits hyperspace
    };

    // Init sound driver resources and pre-calculate waveforms
    static void init();

    // Stop playing all sounds, release data and driver resources
    static void term();

    // Check whether sound is available (valid after init)
    static bool available();

    // Immediately mute all sound (even if they are still being
    // generated) (this is the default after init).
    static void off();

    // Enable the generation of sound (unmute).
    static void on();
    static bool isOn();

    // Begin background writing of the audio samples for a specified
    // Effect (e.g. Sound::shipFire) into the sound device buffer. If
    // loop is true, the sound repeats until stopped.
    static void play(enum Effect effect, bool loop = false);

    // If the specified Effect sound is playing, stop it immediately.
    static void stop(enum Effect effect);
};

#endif // !sound_hpp
