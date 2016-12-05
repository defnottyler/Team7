#pragma once 

#include <iostream>

using namespace std;

/*
 * Polimorphic base class Card. Card objects can be
 * placed in players' graves, hands, and decks. 
 */
class Card
{
	public:
		//Base constructor
		Card(bool unit, string n);
		
		//Fields
		const bool isUnit;
		const string name;
		
		//Virtual methods to be implemented
		virtual int getStrength() = 0;
		virtual void setStrength(int s) = 0;
		virtual void toString() = 0;
};

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
class UnitCard: public Card
{
	public:
		UnitCard(int typ, int abilit, bool hero, string name, int str);
		int getStrength();
		void setStrength(int s);
		const int type;
		const int ability;
		const bool isHero;
		const int strength; //This is the base
		void toString();
	private:
		int currentStrength;
};

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
class SpecialCard: public Card
{
	public:
		SpecialCard(int eff, string name);
		const int effect;
		int getStrength();
		void setStrength(int s);
		void toString();
};

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



