#include <iostream>
#include <vector>
#include "Cards.h"
//#include "UnitCard.h"
//#include "SpecialCard.h"
//No longer blows up on compilation.
using namespace std;

class BoardRow
{
	public:
		BoardRow(int pos);
		vector<UnitCard> cards;
		const int rowPosition;
		//void applyModifier(int effect);
	private:
		//void deBuff();
		//void buff();
		bool buffed;
		bool deBuffed;
		int rowStrength;
};
 
BoardRow::BoardRow(int pos)
:rowPosition(pos)
{
	rowStrength = 0;
}

 
class Board 
{
	public:
		//Board(); //Constructor starts entire game and configures board.
		//bool playCard(); //Puts a card on the field and changes player's turn
		//void startGame();
		//void startRound(); //Called at start of each round
		//void endOfRound(); //Called when both players pass/run out of cards. Compares strength
		//void endOfGame(); //Called when one or both player's points hit zero
		//void printBoard();
	private:
		BoardRow playerOneRows[3];
		BoardRow playerTwoRows[3];
		vector<Card> playerOneDeck;
		vector<Card> playerTwoDeck;
		vector<Card> playerOneHand;
		vector<Card> playerTwoHand;
		vector<Card> playerOneGrave;
		vector<Card> playerTwoGrave;
		int p1Points;
		int p2Points;
		int p1TotalStrength;
		int p2TotalStrength;
		int boardMod;
		bool turn;
		//void pullHand(); //Fills each hand with 10 cards at start of game
		//void initializeCards(); //Intializes both player's decks.
		//void killCards(); //Places cards in used pile
		//void changeModifier();
}; 
