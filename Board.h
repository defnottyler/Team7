#include <iostream>
#include <vector>
#include <fstream>
#include <string>
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
		Board(); //Constructor starts entire game and configures board.
		void printCards();
		//bool playCard(); //Puts a card on the field and changes player's turn
		//void startGame();
		//void startRound(); //Called at start of each round
		//void endOfRound(); //Called when both players pass/run out of cards. Compares strength
		//void endOfGame(); //Called when one or both player's points hit zero
		//void printBoard();
	private:
		//BoardRow playerOneRows[3];
		//BoardRow playerTwoRows[3];
		vector<UnitCard> playerOneDeck;
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
		void initializeDecks();
		//void pullHand(); //Fills each hand with 10 cards at start of game
		//void initializeCards(); //Intializes both player's decks.
		//void killCards(); //Places cards in used pile
		//void changeModifier();
}; 

Board::Board()
{
	//playerOneRows = {new BoardRow(0), new BoardRow(1), new BoardRow(2)};
	//playerTwoRows = {new BoardRow(0), new BoardRow(1), new BoardRow(2)};
	//playerOneDeck.reserve(10);
	p1Points = 0;
	p2Points = 0;
	p1TotalStrength = 0;
	p2TotalStrength = 0;
	boardMod = 0;
	initializeDecks();
}

void Board::initializeDecks()
{
	ifstream d_one("deckone.txt");
	string line;
	string name;
	bool hero = false;
	int strength, ability, range;
	int index = 5;
	while (getline(d_one, line))
	{
		if (line.substr(0, 4) == "true")
		{
			index = line.find(" ", 5);
			name = line.substr(5, index - 5);
			index++;
			strength = stoi(line.substr(index, 1));
			index = line.find(" ", index) + 1;
			range = stoi(line.substr(index, 1));
			index =line.find(" ", index) + 1;
			ability = stoi(line.substr(index, 1));
			index = line.find(" ", index) + 1;
			if (line.substr(index, 4) == "true")
				hero = true;
			UnitCard card(range, ability, hero, name, strength);
			//playerOneDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			playerOneDeck.push_back(card);
		}
		else
		{
		}
	}
	d_one.close();
}

void Board::printCards()
{
	for (int i = 0; i < playerOneDeck.size(); i++)
	{
		playerOneDeck.at(i).toString();
	}
}
