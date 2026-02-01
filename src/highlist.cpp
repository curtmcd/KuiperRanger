#include "highlist.hpp"
#include "persist.hpp"

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#define HIGHLIMIT (HIGHCOLS * HIGHROWS)

std::vector<HighList::highEntry> HighList::highs;
std::vector<Text *> HighList::texts;

bool HighList::modified;

void HighList::load()
{
    std::string buf;

    if (!Persist::load(buf)) {
	fprintf(stderr,
		"Unable to save high scores (see kuiper-ranger(6))\n");
    }

    std::istringstream iss(buf);
    std::string line;

    while (std::getline(iss, line)) {
	const char *s = line.c_str();

	highEntry he;

	size_t i;
	for (i = 0; *s != ':' && *s != '\0'; )
	    if (i < NICKMAXLEN)		// buffer size is +1
		he.name[i++] = *s++;
	he.name[i] = '\0';

	if (*s++ != ':')
	    continue;	// ignore bad line

	he.score = 0;
	while (*s >= '0' && *s <= '9')
	    he.score = he.score * 10 + (*s++ - '0');

	if (*s++ != ':' || he.score < 1)
	    continue;

	he.timeStamp  = (time_t)0;
	while (*s >= '0' && *s <= '9')
	    he.timeStamp = he.timeStamp * 10 + (*s++ - '0');

	if (*s != '\0')
	    continue;

	highs.push_back(he);
    }

    // Sort in case they're out of order
    std::sort(highs.begin(), highs.end());

    // Delete scores from the future
    for (auto it = highs.begin(); it != highs.end(); ) {
	if (it->timeStamp > time(NULL))
	    it = highs.erase(it);
	else
	    it++;
    }

    // Delete timed-out scores, except for the top HIGHPERMANENT scores
    if (highs.size() > HIGHPERMANENT)
	for (auto it = highs.begin() + HIGHPERMANENT; it != highs.end(); ) {
	    if (it->timeStamp + HIGHTIMEOUT < time(NULL))
		it = highs.erase(it);
	    else
		it++;
	}

    // Limit total number
    if (highs.size() > HIGHLIMIT)
	highs.erase(highs.begin() + HIGHLIMIT,
		    highs.end());

    modified = true;
}

void HighList::save()
{
    std::ostringstream buf;

    for (const auto& he : highs)
	buf << he.name
	    << ":"
	    << he.score
	    << ":"
	    << he.timeStamp
	    << std::endl;

    if (!Persist::save(buf.str()))
	printf("Unable to save high scores (see kuiper-ranger(6)\n");
}

int HighList::getBest()
{
    return (highs.size() > 0) ? highs[0].score : 0;
}

void HighList::record(const char *name, int score)
{
    if (score <= 0)
	return;

    highEntry he;

    strncpy(he.name, name, NICKMAXLEN);
    he.name[NICKMAXLEN] = '\0';
    he.score = score;
    he.timeStamp = time(NULL);

    auto insert_pos = std::upper_bound(highs.begin(),
				       highs.end(),
				       he);

    highs.insert(insert_pos, he);

    // If there are now too many high scores, delete one to make room.
    // Try to delete the lowest score whose user has more than one
    // entry. That will allow up to HIGHLIMIT users to be represented
    // while still allowing the better players to have multiple entries
    // if there's room.
    for (int i = (int)highs.size() - 1; i >= 1; i--)
	for (int j = i - 1; j >= 0; j--)
	    if (highs.size() > HIGHLIMIT &&
		strcmp(highs[i].name, highs[j].name) == 0) {
		highs.erase(highs.begin() + i);
		break;
	    }

    // Failing that, delete the lowest scoring entry.
    if (highs.size() > HIGHLIMIT)
	highs.pop_back();

    save();

    modified = true;
}

// Routines dealing with the graphics

void HighList::on()
{
    for (int i = 0; i < HIGHLIMIT; i++)
	texts[i]->on();
}

void HighList::off()
{
    for (int i = 0; i < HIGHLIMIT; i++)
	texts[i]->off();
}

void HighList::update()
{
    if (modified) {
	// Update the content for each high score Text

	for (int i = 0; i < HIGHLIMIT; i++) {
	    char buf[80];

	    if (i < (int)highs.size()) {
		highEntry &he = highs[i];

		char scorebuf[SCOREDIGITS + 1];
		sprintf(scorebuf, "%d", he.score);

		sprintf(buf,
			"%*.*s%.*s%s",
			(int)NICKMAXLEN, (int)NICKMAXLEN, he.name,
			(int)(SCOREDIGITS + 1 - strlen(scorebuf)),
			".........................",
			scorebuf);
	    } else
		buf[0] = 0;

	    texts[i]->set(buf);
	}

	modified = false;
    }

    for (int i = 0; i < HIGHLIMIT; i++)
	texts[i]->update();
}

void HighList::init()
{
    load();

    // Lay out high score texts in rows and columns
    Vect screenSize = Plot::getSize();
    Point basePos(screenSize.x * PERCENT(12),
		  screenSize.y * PERCENT(50));
    Vect spacingX(screenSize.x / (HIGHCOLS + 1), 0.0);
    Vect spacingY(0.0, (screenSize.y / 3) / HIGHROWS);

    int row = 0;
    int col = 0;

    for (int i = 0; i < HIGHLIMIT; i++) {
	Text *t = new Text();

	t->setJustify(Text::LEFT, Text::BOTTOM);
	t->setPos(basePos + row * spacingY + col * spacingX);
	t->setScale(PERCENT(200));

	if (++col == HIGHCOLS) {
	    col = 0;
	    row++;
	}

	texts.push_back(t);
    }
}

void HighList::term()
{
    for (int i = 0; i < HIGHLIMIT; i++)
	delete texts[i];
}
