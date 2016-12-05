#include "BoardRow.h"
#include "Cards.h"

/*
 * Constructor intializes all values to their defaults
 */
BoardRow::BoardRow() 
{
    buffed = false;
    deBuffed = false;
    morale = 0;
    rowStrength = 0;
}


/*
 * Sets row. Should only be used once.
 */
void BoardRow::setRow(int pos)
{
    rowPosition = pos;
}

/*
 * Adds a card to the row and ensures that any active
 * effects are applied. If statements are used to complete
 * the tests.
 */
void BoardRow::add(UnitCard* card)
{
	if (!card->isHero)
	{
		if (deBuffed)
			card->setStrength(1);
		if (morale >= 1)
			card->setStrength(card->getStrength() + morale);
		if (buffed)
			card->setStrength(card->getStrength() * 2);
	}
	cards.push_back(card);
}

/*
 * Clears all effects from the board and re-applies
 * buffs. This ensures that the net effect is only for
 * negative effects to be wiped.
 */
void BoardRow::clear()
{
    rowStrength = 0;
    for (int i = 0; i < cards.size(); i++)
    {
        if (!cards.at(i)->isHero)
            cards.at(i)->setStrength(cards.at(i)->strength);
        rowStrength += cards.at(i)->getStrength();
    }
    deBuffed = false;
    if (buffed)
		buff();
    if (morale >= 1)
        moraleBoost();
}

/*
 * Reduces the strength of non hero cards to 1 if not buffed
 * and two if buffed. Also invokes moraleBoost() if necessary.
 */
void BoardRow::deBuff()
{
    rowStrength = 0;
    for (int i = 0; i < cards.size(); i++)
    {
        if (!cards.at(i)->isHero && !buffed)
        {
            cards.at(i)->setStrength(1);
		}
        else if (!cards.at(i)->isHero && buffed)
        {
            cards.at(i)->setStrength(2);
		}
        rowStrength += cards.at(i)->getStrength();
    }
    deBuffed = true;
    if (morale != 0)
		moraleBoost();
}

/*
 * Returns status of deBuff
 */
 bool BoardRow::getDeBuffed()
 {
	 return deBuffed;
 }

/*
 * Increases the strength of non hero cards to double their base
 * or 2 if debuffed. Invokes moraleBoost() if necessary
 */
void BoardRow::buff()
{
    rowStrength = 0;
    for (int i = 0; i < cards.size(); i++)
    {
        if (!cards.at(i)->isHero && !deBuffed)
            cards.at(i)->setStrength(cards.at(i)->strength * 2);
        else if (!cards.at(i)->isHero && deBuffed)
        {
            cards.at(i)->setStrength(2);
		}
        rowStrength += cards.at(i)->getStrength();
    }
    buffed = true;
	if (morale >= 1)
		moraleBoost();
}

/*
 * Returns status of buffed
 */
 bool BoardRow::getBuffed()
 {
	 return buffed;
 }

/*
 * Applies moraleBoost effect. Method attempts to address all cases
 * with a boost. Note that cards with the ability do not apply it to
 * themselves. Also, note that a debuff overrides a buff during a boost.
 * Morale boosts can stack with each other. 
 */
void BoardRow::moraleBoost()
{
    for (UnitCard *c : cards)
    {
        if (!c->isHero && (!buffed || deBuffed) && c->ability != 1)
        {
			if (deBuffed)
				c->setStrength(1 + morale);
			else
				c->setStrength(c->strength + morale);
		}
        if (!c->isHero && (!buffed || deBuffed) && c->ability == 1)
        {
			if (deBuffed)
				c->setStrength(1 + (morale - 1));
			else
				c->setStrength(c->strength + (morale - 1));
		}
        if (!c->isHero && buffed && c->ability != 1)
        {
		if (deBuffed)
			c->setStrength(2 + morale);
		else
            		c->setStrength(c->strength * 2 + (2 * morale));
		}
        if (!c->isHero && buffed && c->ability == 1)
        {
		if (deBuffed)
			c->setStrength(2 + (morale - 1));
		else
			c->setStrength(c->strength * 2 + ((2 * morale) - 2));
	}
    }
}

/*
 * Public method called in board to apply new boost.
 */
void BoardRow::addMorale()
{
	morale++;
	moraleBoost();
}

/*
 * Calculates and returns the current strength of the row
 */
int BoardRow::getRowStr()
{
    rowStrength = 0;
    for (UnitCard* c : cards)
        rowStrength += c->getStrength();
    return rowStrength;
}

/*
 * Resets all board values to the default.
 */
void BoardRow::reset()
{
    buffed = false;
    deBuffed = false;
    morale = 0;
    clear();
    rowStrength = 0;
}
