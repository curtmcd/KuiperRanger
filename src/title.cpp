#include "title.hpp"

Text *Title::titleText;
Text *Title::instrText;

void Title::init(const char *title)
{
    Vect screenSize = Plot::getSize();

    titleText = new Text();
    titleText->set(title);
    titleText->setFont(Font::fontBoldItalic);

    Point titleTextPos(screenSize.x / 2, screenSize.y * PERCENT(30));
    titleText->setPos(titleTextPos);
    titleText->setScale(PERCENT(800));

    instrText = new Text();
    instrText->set("Press H for Commands");
    instrText->setFont(Font::fontItalic);

    Point instrTextPos(screenSize.x / 2, screenSize.y * PERCENT(92));
    instrText->setPos(instrTextPos);
    instrText->setScale(PERCENT(200));
}

void Title::term()
{
    delete titleText;
    delete instrText;
}

void Title::on()
{
    titleText->on();
    instrText->on();
}

void Title::off()
{
    titleText->off();
    instrText->off();
}

void Title::update()
{
    titleText->update();
    instrText->update();
}
