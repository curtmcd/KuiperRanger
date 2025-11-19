#include "highlist.hpp"
#include "persist.hpp"

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#define HIGHCOUNT (HIGHCOLS * HIGHROWS)

namespace HighList {
    struct highEntry {
	char name[NICKMAXLEN + 1];
	int score;
	time_t timeStamp;
    };

    static struct highEntry highs[HIGHCOUNT];
    static int highCount;		// non-decreasing

    static Text *highTexts[HIGHCOUNT];

    static Linefont *scoreFont = NULL;

    void load()
    {
	for (int i = 0; i < HIGHCOUNT; i++) {
	    highs[i].name[0] = 0;
	    highs[i].score = 0;
	    highs[i].timeStamp = (time_t)0;
	}

	std::string buf;

	if (!Persist::load(buf)) {
	    printf("Unable to save high scores (see kuiper-ranger(6))\n");
	}

	std::istringstream iss(buf);
	std::string line;

	time_t now = time(NULL);

	for (highCount = 0; highCount < HIGHCOUNT; ) {
	    if (!std::getline(iss, line))
		break;

	    const char *s = line.c_str();

	    highEntry *he = &highs[highCount];

	    int i = 0;
	    while (*s != ':' && *s != '\0')
		if (i < NICKMAXLEN)
		    he->name[i++] = *s++;

	    he->name[i] = '\0';

	    if (*s++ != ':')
		continue;	// ignore bad line

	    he->score = 0;
	    while (*s >= '0' && *s <= '9')
		he->score = he->score * 10 + (*s++ - '0');

	    if (*s++ != ':' || he->score < 1)
		continue;

	    he->timeStamp  = (time_t)0;
	    while (*s >= '0' && *s <= '9')
		he->timeStamp = he->timeStamp * 10 + (*s++ - '0');

	    if (*s != '\0')
		continue;

	    // Ignore highs from the future
	    if (he->timeStamp > now)
		continue;

	    // Ignore highs that have timed out (except top HIGHPERMANENT scores)
	    if (highCount >= HIGHPERMANENT && he->timeStamp < now - HIGHTIMEOUT)
		continue;

	    highCount++;
	}
    }

    void save()
    {
	std::ostringstream buf;

	for (int i = 0; i < highCount; i++)
	    buf << highs[i].name
		<< ":"
		<< highs[i].score
		<< ":"
		<< highs[i].timeStamp
		<< std::endl;

	if (!Persist::save(buf.str()))
	    printf("Unable to save high scores (see kuiper-ranger(6)\n");
    }

    int getBest()
    {
	if (highCount == 0)
	    return 0;

	return highs[0].score;
    }

    void record(const char *name, int score)
    {
	int i;

	if (score <= 0)
	    return;

	for (i = 0; i < highCount; i++)
	    if (score > highs[i].score)
		break;

	// 0 <= (i = insertion point) <= highCount
	if (i == HIGHCOUNT)
	    return;

	for (int j = MIN(highCount, HIGHCOUNT - 1); j > i; j--)
	    highs[j] = highs[j - 1];

	strncpy(highs[i].name, name, NICKMAXLEN);
	highs[i].name[NICKMAXLEN] = '\0';
	highs[i].score = score;
	highs[i].timeStamp = time(NULL);

	if (highCount < HIGHCOUNT)
	    highCount++;

	save();
    }

    // Routines dealing with the graphics

    void on()
    {
	for (int i = 0; i < highCount; i++) {
	    char scorebuf[SCOREDIGITS + 1];
	    sprintf(scorebuf, "%d", highs[i].score);

	    char buf[80];
	    sprintf(buf,
		    "%*.*s%.*s%s", NICKMAXLEN, NICKMAXLEN, highs[i].name,
		    (int)(SCOREDIGITS + 1 - strlen(scorebuf)),
		    ".........................",
		    scorebuf);

	    highTexts[i]->set(buf);
	    highTexts[i]->on();
	    highTexts[i]->update();
	}
    }

    void off()
    {
	for (int i = 0; i < highCount; i++)
	    highTexts[i]->off();
    }

    void update()
    {
	for (int i = 0; i < highCount; i++)
	    highTexts[i]->update();
    }

    void init()
    {
	Vect screenSize = Plot::getSize();
	Vect scorePos(0, screenSize.y * PERCENT(55));
	Vect scoreSize(screenSize.x / (HIGHCOLS + 1),
		       screenSize.y / 3 / HIGHROWS);

	scoreFont = new Linefont(PERCENT(200), 0, false);

	load();

	Vect charSpacing = scoreFont->getCharSpacing();

	// Lay out the screen locations for high score entries
	int row = 0;
	int col = 0;

	for (int i = 0; i < HIGHCOUNT; i++) {
	    highTexts[i] = new Text();
	    highTexts[i]->setFont(scoreFont);

	    Vect pos;
	    pos.x = (scorePos.x + ((col + 1) * scoreSize.x) -
		     charSpacing.x * NICKMAXLEN);
	    pos.y = scorePos.y + row * scoreSize.y;

	    highTexts[i]->setPos(pos);

	    if (++col == HIGHCOLS) {
		col = 0;
		row++;
	    }
	}
    }

    void term()
    {
	for (int i = 0; i < HIGHCOUNT; i++)
	    delete(highTexts[i]);

	delete scoreFont;
    }
};
