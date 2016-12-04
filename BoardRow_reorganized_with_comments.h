#include <iostream>
#include "Cards.h"

using namespace std;

//BoardRow class.
class BoardRow
{
	
public:
	
    //BoardRow constructor.
    BoardRow();
    
    //BoardRow variables.
    vector<UnitCard*> cards;
    
    //BoardRow functions.
    void setRow(int pos);
    int getRowStr();
    void add(UnitCard *card);
    void deBuff();
    void buff();
    void moraleBoost();
    void clear();
    void reset();
    
private:
    
    //BoardRow variables.
    int rowPosition;
    bool buffed;
    bool deBuffed;
    bool morale;
    int rowStrength;
};

//BoardRow constructor.
BoardRow::BoardRow()
{
    buffed = false;
    deBuffed = false;
    morale = false;
    rowStrength = 0;
}

//function to set the row position.
void BoardRow::setRow(int pos)
{
    rowPosition = pos;
}

//function to add a card.
void BoardRow::add(UnitCard* card)
{
	if (!card->isHero)
	{
		if (deBuffed)
			card->setStrength(1);
		if (morale)
			card->setStrength(card->getStrength() + 1);
		if (buffed)
			card->setStrength(card->getStrength() * 2);
	}
	cards.push_back(card);
}

//function to clear the effects.
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
    if (buffed)
		buff();
    if (morale)
        moraleBoost();
}

//function to deBuff the row.
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

//function to buff the row.
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

//function for moraleBoost.
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

//function to get the strength of the row.
int BoardRow::getRowStr()
{
    rowStrength = 0;
    for (UnitCard* c : cards)
        rowStrength += c->getStrength();
    return rowStrength;
}

//fucntion to reset the row.
void BoardRow::reset()
{
    buffed = false;
    deBuffed = false;
    morale = false;
    clear();
    rowStrength = 0;
}
