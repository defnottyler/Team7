#include "Cards.h"

/*
 * Constructor establishes name and whether or
 * not card is unit or special
 */
Card::Card(bool unit, string n)
:isUnit(unit), name(n)
{
}

/*
 * UnitCard class represents cards that are actually
 * "placed" onto the board via BoardRow objects.
 * They have atributes such as strength and special 
 * ability. They can also be hero cards as denoted
 * by the isHero field. The currentStrength field
 * takes into consideration active modifiers on the
 * card. Extends Card class.
 */

/*
 * UnitCard constructor assigns all values.
 */
UnitCard::UnitCard(int typ, int abilit, bool hero, string name, int str)
:Card(true, name), isHero(hero), type(typ), ability(abilit), strength(str)
{
	currentStrength = strength;
}

//Returns currentStrength
int UnitCard::getStrength()
{
	return currentStrength;
}

//Sets current strength
void UnitCard::setStrength(int s)
{
	currentStrength = s;
}

//Simple method that prints card. Mostly used for debugging
void UnitCard::toString()
{
	cout <<name<<" "<<type<<" "<<ability<<" "<<isHero<<" "<<currentStrength<<endl;
}

/*
 * SpecialCard class represents cards that are not
 * played directly on the board, but influence it.
 * They contain special abilities that influence
 * the strength of rows.
 */


//Constructor sets all values
SpecialCard::SpecialCard(int eff, string name)
:Card(false, name), effect(eff)
{
}

//Polymorphic placeholder
int SpecialCard::getStrength()
{
	return -1;
}

void SpecialCard::setStrength(int s)
{
	//Polymorphic placeholder
}

//Prints card's name
void SpecialCard::toString()
{
	cout <<name<<endl;
}



