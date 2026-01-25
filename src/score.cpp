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

    double fontScale = (double)SCOREBARH / 14.0;
    font = new Linefont(fontScale, false);

    points = 0;
    pointsText = new Text();

    high = HighList::getBest();
    highText = new Text();

    setText();

    pointsText->setFont(font);
    Vect scoreSize = pointsText->getSize();
    Point scorePos(SCOREX, ((double)SCOREBARH + scoreSize.y) / 2);
    pointsText->setPos(scorePos);

    highText->setFont(font);
    Vect highSize = highText->getSize();
    Point highPos(HIGHX, ((double)SCOREBARH + highSize.y) / 2);
    highText->setPos(highPos);
}

Score::~Score()
{
    delete pointsText;
    delete highText;
    delete font;
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
