#pragma once

#include <iostream>

using namespace std;

class Card
{
	public:
		Card(bool unit, string n);
		const bool isUnit;
		const string name;
};

Card::Card(bool unit, string n)
:isUnit(unit), name(n)
{
}

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
		string toString();
	private:
		int currentStrength;
};

UnitCard::UnitCard(int typ, int abilit, bool hero, string name, int str)
:Card(true, name), isHero(hero), type(typ), ability(abilit), strength(str)
{
	currentStrength = strength;
}

int UnitCard::getStrength()
{
	return currentStrength;
}

void UnitCard::setStrength(int s)
{
	currentStrength = s;
}

string UnitCard::toString()
{
	cout <<name<<" "<<type<<" "<<ability<<" "<<isHero<<" "<<strength<<endl;
}

class SpecialCard: public Card
{
	public:
		SpecialCard(int eff, string name);
		const int effect;
};

SpecialCard::SpecialCard(int eff, string name)
:Card(false, name), effect(eff)
{
}



