#include "param.hpp"
#include "score.hpp"
#include "highlist.hpp"

static int scoreHardLimit;

void Score::setText()
{
    char buf[32];

    sprintf(buf, "%0*d", SCOREDIGITS, points);
    pointsText->set(buf);

    sprintf(buf, "%0*d", SCOREDIGITS, high);
    highText->set(buf);
}

Score::Score()
{
    scoreHardLimit = 1;
    for (int i = 1; i < SCOREDIGITS; i++)
	scoreHardLimit *= 10;
    scoreHardLimit--;

    points = 0;
    pointsText = new Text();
    pointsText->setJustify(Text::LEFT, Text::MIDDLE);

    high = HighList::getBest();
    highText = new Text();
    highText->setJustify(Text::LEFT, Text::MIDDLE);

    setText();

    Point scorePos(SCOREX, (double)SCOREBARH / 2);
    pointsText->setPos(scorePos);
    pointsText->setScale((double)SCOREBARH / 14.0);

    Point highPos(HIGHX, (double)SCOREBARH / 2);
    highText->setPos(highPos);
    highText->setScale((double)SCOREBARH / 14.0);
}

Score::~Score()
{
    delete pointsText;
    delete highText;
}

void Score::inc(int _points)
{
    points += _points;

    if (points > scoreHardLimit)
	points = scoreHardLimit;

    if (points > high)
	high = points;
}

void Score::update()
{
    setText();

    pointsText->update();
    highText->update();
}
