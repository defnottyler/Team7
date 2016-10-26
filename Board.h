#include <iostream>
#include "Cards.h"
//WARNING: EXPLODES ON COMPILATION
using namespace std;

class BoardRow
{
	public:
		BoardRow(int pos);
		UnitCard cardList[15];
		const int rowPosition;
		void applyModifier(int effect);
	private:
		void deBuff();
		void buff();
		bool buffed;
		bool deBuffed;
		int rowStrength;
};

BoardRow::BoardRow(int pos)
:rowPosition(pos)
{
	for (int i = 0; i < 15; i++)
	{
		cardList[i] = new UnitCard(0, 0, 0, false, "EEHH", 10);
	}
	rowStrength = 0;
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
		default:
			break;
	}
}

void BoardRow::deBuff()
{
	for (int i = 0; i < 15; i++)
	{
		if (cardList[i] == NULL)
			continue;
		if (!cardList[i].isHero)
			cardList[i].setStrength(1);
	}
}

void BoardRow::buff()
{
	for (int i = 0; i < 15; i++)
	{
		if (cardList[i] == NULL)
			continue;
		if (!cardList[i].isHero)
			cardList[i].setStrength(cardList[i].getStrength() * 2);
	}
}
 
class Board 
{
	public:
		Board(); //Constructor starts entire game and configures board.
		bool playCard(); //Puts a card on the field and changes player's turn
		void startGame();
		void startRound(); //Called at start of each round
		void endOfRound(); //Called when both players pass/run out of cards. Compares strength
		void endOfGame(); //Called when one or both player's points hit zero
		void printBoard();
	private:
		BoardRow playerOneRows[3];
		BoardRow playerTwoRows[3];
		Card playerOneDeck[24];
		Card playerTwoDeck[24];
		Card playerOneHand[15];
		Card playerTwoHand[15];
		Card playerOneDiscard[24];
		Card playerTwoDiscard[24];
		int p1Points;
		int p2Points;
		int p1TotalStrength;
		int p2TotalStrength;
		int boardMod;
		bool turn;
		void pullHand(); //Fills each hand with 10 cards at start of game
		void initializeCards(); //Intializes both player's decks.
		void killCards(); //Places cards in used pile
		void changeModifier;
};
