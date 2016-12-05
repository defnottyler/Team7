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


class SpecialCard: public Card
{
	public:
		SpecialCard(int eff, string name);
		const int effect;
		int getStrength();
		void setStrength(int s);
		void toString();
};
