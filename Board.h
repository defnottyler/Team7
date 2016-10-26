#include <iostream>
#include "Cards.h"

using namespace std;

class BoardRow
{
	public:
		BoardRow(int pos);
		Card cardList[15];
		const int rowPosition;
		void applyModifier(int effect);
	private:
		void deBuff();
		void buff();
		bool buffed;
		bool deBuffed;
};

BoardRow::BoardRow(int pos)
:rowPosition(pos)
{
	cardList = {NULL};
}

void BoardRow::applyModifier(int effect)
{
	switch (effect)
	{
		case 0:
			deBuff();
			break;
		case 1:
			buff();
			break;
	}
}

void BoardRow::deBuff()
{
	for (int i = 0; i < 15; i++)
	{
		if (cardList[i] == NULL)
			break;
	}
}

class Board 
{
	public:
	private:
};
