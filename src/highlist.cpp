#include <SDL2/SDL.h>

#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "highlist.hpp"
#include "filelock.hpp"

#ifdef __unix__
#include <unistd.h>
#endif // __unix__

#define HIGHLIMIT (HIGHCOLS * HIGHROWS)

std::vector<HighList::highEntry> HighList::highs;
std::vector<Text *> HighList::texts;

bool HighList::modified;

std::string HighList::getHighScoreFilename()
{
    char *s;

    // Allow the user to override the exact file location
    if ((s = getenv(ENVVAR_HSFILE)) != NULL && s[0] != '\0')
	return std::string(s);

#ifdef __unix__
    // Use the default system-wide file if it exists and is writable.
    // This would usually be created by a package installer on Linux.
    if (access(HSFILE_UNIXPKG, R_OK) == 0 &&
	access(HSFILE_UNIXPKG, W_OK) == 0)
	return std::string(HSFILE_UNIXPKG);
#endif // __unix__

    // Use a per-user standard file location
    if ((s = SDL_GetPrefPath("Fishlet", "kuiper-ranger")) != NULL) {
	std::string path(s);
	SDL_free(s);
	return path + "scores";
    }

    return "";
}

void HighList::load()
{
    std::string path = getHighScoreFilename();

    if (path.empty()) {
	std::cerr << "Could not determine location of high score file to load\n";
	return;
    }

    FileLock lock(path.c_str());

    std::ifstream file(path);
    if (!file.is_open()) {
	std::cerr << "Could not load high scores\n";
	return;
    }

    highs.clear();

    std::string line;

    while (std::getline(file, line)) {
	std::stringstream ss(line);
	std::string namePart, scorePart, timePart;

	if (std::getline(ss, namePart, ':') &&
            std::getline(ss, scorePart, ':') &&
            std::getline(ss, timePart, ':')) {

            highEntry he;
            strncpy(he.name, namePart.c_str(), NICKMAXLEN);
            he.name[NICKMAXLEN] = '\0';
            he.score = std::stoi(scorePart);
            he.timeStamp = (time_t)std::stoll(timePart);

	    if (he.score >= HIGHSCOREMIN && he.timeStamp <= time(NULL))
                highs.push_back(he);
	}
    }

    // Sort in case they're out of order
    std::sort(highs.begin(), highs.end());

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
	highs.resize(HIGHLIMIT);

    modified = true;
}

void HighList::save()
{
    std::string path = getHighScoreFilename();

    if (path.empty()) {
	std::cerr << "Could not determine location of high score file to save\n";
	return;
    }

    FileLock lock(path.c_str());

    std::ofstream file(path, std::ios::trunc);
    if (!file.is_open()) {
	std::cerr << "Could not save high score file\n";
	return;
    }

    for (const auto& he : highs)
	file << he.name << ":" << he.score << ":" << he.timeStamp << "\n";
}

int HighList::getBest()
{
    return (highs.size() > 0) ? highs[0].score : 0;
}

bool HighList::qualifies(int score)
{
    if (score < HIGHSCOREMIN)
	return false;

    if (highs.size() < HIGHLIMIT)
	return true;

    return (score > highs.back().score);
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

void HighList::init(bool resetAll)
{
    if (resetAll)
	save();
    else
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
