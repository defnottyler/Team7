#include <iostream>
#include "Cards.h"

using namespace std;

class BoardRow
{
public:
    BoardRow();
    vector<UnitCard*> cards;
    void setRow(int pos);
    int getRowStr();
    void deBuff();
    void buff();
    void moraleBoost();
    void clear();
    void reset();
private:
    int rowPosition;
    bool buffed;
    bool deBuffed;
    bool morale;
    int rowStrength;
};

BoardRow::BoardRow()
{
    buffed = false;
    deBuffed = false;
    morale = false;
    rowStrength = 0;
}

void BoardRow::setRow(int pos)
{
    rowPosition = pos;
}

void BoardRow::clear()
{
    rowStrength = 0;
    for (int i = 0; i < cards.size(); i++)
    {
        if (!cards.at(i)->isHero)
            cards.at(i)->setStrength(cards.at(i)->strength);
        rowStrength += cards.at(i)->getStrength();
    }
    //buffed = false;
    deBuffed = false;
    if (morale)
        moraleBoost();
}

void BoardRow::deBuff()
{
    rowStrength = 0;
    for (int i = 0; i < cards.size(); i++)
    {
        if (!cards.at(i)->isHero && !buffed)
            cards.at(i)->setStrength(1);
        else if (!cards.at(i)->isHero && buffed)
            cards.at(i)->setStrength(2);
        rowStrength += cards.at(i)->getStrength();
    }
    deBuffed = true;
}

void BoardRow::buff()
{
    rowStrength = 0;
    for (int i = 0; i < cards.size(); i++)
    {
        if (!cards.at(i)->isHero && !deBuffed)
            cards.at(i)->setStrength(cards.at(i)->strength * 2);
        else if (!cards.at(i)->isHero && deBuffed)
            cards.at(i)->setStrength(2);
        rowStrength += cards.at(i)->getStrength();
    }
    buffed = true;
}

void BoardRow::moraleBoost()
{
    for (UnitCard *c : cards)
    {
        if (!c->isHero && !buffed)
            c->setStrength(c->getStrength() + 1);
        else if (!c->isHero && buffed)
            c->setStrength(c->getStrength() + 2);
    }
    morale = true;
}

int BoardRow::getRowStr()
{
    rowStrength = 0;
    for (UnitCard* c : cards)
        rowStrength += c->getStrength();
    return rowStrength;
}

void BoardRow::reset()
{
    buffed = false;
    deBuffed = false;
    morale = false;
    rowStrength = 0;
}
