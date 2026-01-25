// Sound Test - utility to assist in waveform design
// g++ -DSOUND_SDL2 -o soundtest soundtest.cpp sound.cpp -lSDL2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sound.hpp"

void commandHelp()
{
    printf("Sound commands:\n");
    printf("%d: shipFire\n", Sound::shipFire);
    printf("%d: alienFire\n", Sound::alienFire);
    printf("%d: rockExplode\n", Sound::rockExplode);
    printf("%d: shipExplode\n", Sound::shipExplode);
    printf("%d: alienPoints\n", Sound::alienPoints);
    printf("%d: alienExplode\n", Sound::alienExplode);
    printf("%d: alienMotor\n", Sound::alienMotor);
    printf("%d: extraShip\n", Sound::extraShip);
    printf("%d: lowDeet\n", Sound::lowDeet);
    printf("%d: highDeet\n", Sound::highDeet);
    printf("%d: shipThrust\n", Sound::shipThrust);
    printf("%d: inHyperspace\n", Sound::inHyperspace);
    printf("%d: outHyperspace\n", Sound::outHyperspace);
    printf("+: Add to a sound number to play it in a loop\n");
    printf("-: Add to a sound number to stop it\n");
    printf("r: rapid ship fire (for tuning AUDIO_SAMPLES)\n");
    printf("q: exit\n");
    printf("h: repeat this help\n");
}

static bool stringRemove(char *s, char ch)
{
    bool found = false;
    char *d = s;

    do {
	if (*s == ch)
	    found = true;
	else
	    *d++ = *s;
    } while (*s++ != '\0');

    *d = '\0';

    return found;
}

void rapidFire()
{
    for (int i = 0; i < 20; i++) {
	Sound::play(Sound::shipFire);
	usleep(250000);
    }
}

int main()
{
    Sound::init();

    if (!Sound::available()) {
	fprintf(stderr, "Could not initialize for sound output\n");
	exit(1);
    }

    Sound::on();

    while (1) {
	printf("Command (h for help): ");
	fflush(stdout);

	char buf[80];
	if (fgets(buf, sizeof (buf), stdin) == NULL)
	    break;

	if (stringRemove(buf, 'q'))
	    break;

	if (stringRemove(buf, 'h')) {
	    commandHelp();
	    continue;
	}

	if (stringRemove(buf, 'r')) {
	    rapidFire();
	    continue;
	}

	bool loop = stringRemove(buf, '+');
	bool stop = stringRemove(buf, '-');

	if (buf[0] < '0' || buf[0] > '9') {
	    commandHelp();
	    continue;
	}

	int sound = atoi(buf);

	if (sound < Sound::shipFire || sound > Sound::outHyperspace) {
	    printf("Sound number out of range\n");
	    continue;
	}

	Sound::Effect effect = (Sound::Effect)sound;

	if (stop)
	    Sound::stop(effect);
	else
	    Sound::play(effect, loop);
    }

    Sound::term();

    return 0;
}
