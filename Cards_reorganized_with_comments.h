#pragma once

#include <iostream>

using namespace std;

//Card calss
class Card
{
	
	public:
		
		//Card constructor
		Card(bool unit, string n);
		
		//Card variables
		const bool isUnit;
		const string name;
		
		//Card functions
		virtual int getStrength() = 0;
		virtual void setStrength(int s) = 0;
		virtual void toString() = 0;
};

//Card constructor.
Card::Card(bool unit, string n)
:isUnit(unit), name(n)
{
}

//Unit card calss.
class UnitCard: public Card
{
	public:
		
		//Unitcard constructor.
		UnitCard(int typ, int abilit, bool hero, string name, int str);
		
		//UnitCard variables.
		const int type;
		const int ability;
		const bool isHero;
		const int strength; //This is the base
		
		//UnitCard functions.
		void toString();
		int getStrength();
		void setStrength(int s);
	
	private:
		
		//CurrentStrength changes during the game play.
		int currentStrength;
};

//Unitcard constructor.
UnitCard::UnitCard(int typ, int abilit, bool hero, string name, int str)
:Card(true, name), isHero(hero), type(typ), ability(abilit), strength(str)
{
	currentStrength = strength;
}

//Function to get strength.
int UnitCard::getStrength()
{
	return currentStrength;
}

//Function to set new strength
void UnitCard::setStrength(int s)
{
	currentStrength = s;
}

//toString(Unitcard)
void UnitCard::toString()
{
	cout <<name<<" "<<type<<" "<<ability<<" "<<isHero<<" "<<strength<<endl;
}

//Special card class.
class SpecialCard: public Card
{
	public:
		
		//Special card constructor.
		SpecialCard(int eff, string name);
		
		//Special card variable.
		const int effect;
		
		//Special card functions.
		int getStrength();
		void setStrength(int s);
		void toString();
};

//Special card constructor.
SpecialCard::SpecialCard(int eff, string name)
:Card(false, name), effect(eff)
{
}

//Function to get special card strength.(None)
int SpecialCard::getStrength()
{
	return -1;
}

//Function to set special card strength.
void SpecialCard::setStrength(int s)
{
	//Polymorphic placeholder
}

//toString(Specialcard)
void SpecialCard::toString()
{
	cout <<name<<endl;
}



