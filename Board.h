#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include "Cards.h"
//#include "UnitCard.h"
//#include "SpecialCard.h"
//No longer blows up on compilation.
using namespace std;

class BoardRow
{
	public:
		//BoardRow(int pos);
		BoardRow();
		vector<UnitCard*> cards;
		void setRow(int pos);
		void applyModifier(int effect);
		int getRowStr();
	private:
		void deBuff();
		void buff();
		void clear();
		//void calcStr();
		int rowPosition;
		bool buffed;
		bool deBuffed;
		int rowStrength;
};
 
BoardRow::BoardRow()
{
	rowStrength = 0;
}

void BoardRow::setRow(int pos)
{
	rowPosition = pos;
}

void BoardRow::applyModifier(int effect)
{
	switch (effect)
	{
		case 0:
			clear();
			break;
		case 1:
			buff();
			break;
		case 2:
			deBuff();
			break;
	}
}

void BoardRow::clear()
{
	rowStrength = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		if (!cards.at(i)->isHero)
			cards.at(i)->setStrength(cards.at(i)->strength);
		rowStrength += cards.at(i)->getStrength();
	}
	buffed = false;
	deBuffed = false;
}

void BoardRow::deBuff()
{
	rowStrength = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		if (!cards.at(i)->isHero && !buffed)
			cards.at(i)->setStrength(1);
		else if (!cards.at(i)->isHero && buffed)
			cards.at(i)->setStrength(2);
		rowStrength += cards.at(i)->getStrength();
	}
	deBuffed = true;
}

void BoardRow::buff()
{
	rowStrength = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		if (!cards.at(i)->isHero && !deBuffed)
			cards.at(i)->setStrength(cards.at(i)->strength * 2);
		else if (!cards.at(i)->isHero && deBuffed)
			cards.at(i)->setStrength(2);
		rowStrength += cards.at(i)->getStrength();
	}
}

int BoardRow::getRowStr()
{
	return rowStrength;
}


 
class Board 
{
	public:
		Board(); //Constructor starts entire game and configures board.
		void printCards();
		void handGenerator();
		int chooseTurn();
		void printHand();
		void play();
		void displayTurnOptions();
		int getPlayerOption(); //Input validation
		int playerTurn(vector<*Card> playerDeck, bool playerPass, bool otherPass);//My version of the Turn method. Combines the two.
		//int playerOneTurn();
		//int playerTwoTurn();
		//bool playCard(); //Puts a card on the field and changes player's turn
		//void startGame();
		//void startRound(); //Called at start of each round
		//void endOfRound(); //Called when both players pass/run out of cards. Compares strength
		//void endOfGame(); //Called when one or both player's points hit zero
		//void printBoard();
	private:
		BoardRow playerOneRows[3];
		BoardRow playerTwoRows[3];
		vector<Card*> playerOneDeck;
		vector<Card*> playerTwoDeck;
		vector<Card*> playerOneHand;
		vector<Card*> playerTwoHand;
		vector<Card*> playerOneGrave;
		vector<Card*> playerTwoGrave;
		int p1Points;
		int p2Points;
		int p1TotalStrength;
		int p2TotalStrength;
		int boardMod;
		bool isFirstTurn;
		int firstTurnChoice;
		int playerTurn;
		int roundCount;
		bool p1Pass; //when true, prevents this player from having a turn for the rest of the round
		bool p2Pass; //
		bool pass;
		void initializeDecks(string filename, bool p);
		//void pullHand(); //Fills each hand with 10 cards at start of game
		//void killCards(); //Places cards in used pile
		//void changeModifier();
}; 

Board::Board()
{
	for (int i = 0; i < 3; i++)
	{
		playerOneRows[i].setRow(i);
		playerTwoRows[i].setRow(i);
	}
	p1Points = 0;
	p2Points = 0;
	p1TotalStrength = 0;
	p2TotalStrength = 0;
	boardMod = 0;
	isFirstTurn = TRUE;
	initializeDecks("deckone.txt", true);
	initializeDecks("decktwo.txt", false);
	handGenerator();
	
}

void Board::initializeDecks(string filename, bool p)
{
	ifstream d_one(filename);
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
			strength = stoi(line.substr(index, 2));
			index = line.find(" ", index) + 1;
			range = stoi(line.substr(index, 1));
			index =line.find(" ", index) + 1;
			ability = stoi(line.substr(index, 1));
			index = line.find(" ", index) + 1;
			if (line.substr(index, 4) == "true")
				hero = true;
			//UnitCard card(range, ability, hero, name, strength);
			if (p)
				playerOneDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			else
				playerTwoDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			//playerOneDeck.push_back(card);
		}
		else
		{
			index = line.find(" ", 6);
			name = line.substr(6, index - 6);
			ability = stoi(line.substr(index + 1, 1));
			if (p)
				playerOneDeck.push_back(new SpecialCard(ability, name));
			else
				playerTwoDeck.push_back(new SpecialCard(ability, name));
		}
	}
	d_one.close();
}

void Board::handGenerator()			//puts cards from deck into player hand(s)
{
	srand(time(NULL));
	
	int deckSize1 = playerOneDeck.size();
	int deckSize2 = playerTwoDeck.size();
	
	int index1;
	int index2;
	
	for(int i=0; i<10; i++) {
		
		index1 = rand() % deckSize1;
		index2 = rand() % deckSize2;
		
		playerOneHand.push_back(playerOneDeck.at(index1));
		playerOneDeck.erase(playerOneDeck.begin()+index1-1);
		deckSize1--;
		
		playerTwoHand.push_back(playerTwoDeck.at(index2));
		playerTwoDeck.erase(playerTwoDeck.begin()+index2-1);
		deckSize2--;
		//hands are now generated
	}
}

int Board::chooseTurn() 
{
	srand(time(NULL));
	firstTurnChoice = (rand() % 2) + 1;
	return firstTurnChoice;
}

void Board::play()
{
	if(isFirstTurn) {
		playerTurn = chooseTurn();
	}
	
	do{
		if(playerTurn == 1) {
			playerOneTurn();
		}
		else {
			
		}
	}while(gameIsNOtOver);
}

/*
  Displays what a player can do during their turn.
 */
void Board::displayTurnOptions()
{
	cout << "Choose one of the following options:" << endl;
	cout << "1) Play a card" << endl;
	cout << "2) Pass turn" << endl;
	cout << "Player Option: ";
}

/*
  Makes sure that user inputs a valid input for what they want to do on their turn.
  Returns the option selected when it is valid.
  
  param: int upperBound - the highest int value allowed to perform a selection
  return: int - the valid user option
 */
int Board::getUserOption(int upperBound)
{
	string input;
	int option = -1;
	
	do {
		displayTurnOptions();
		cin >> input;
	
		//assigns value to option for valid input
		if()
		{
			option = input;
		}
		
		//informs user of invalid input
		else
		{
			cout << "Your input is not correct." << endl;
		}
	} while(option < 0); //keep doing this until valid input is obtained
	
	return option;	
}
	

/*
  Allows a given player perform any actions they can during their turn.
  
  param: vector<*Card> playerDeck - the deck of cards associated with the current player
  		       bool &playerPass - once the current player passes, this variable is updated to true
		       bool otherPass - used to check if the other player has already passed
  return: int - what is this int?
 */
int Board::playerTurn(vector<*Card> playerDeck, bool &playerPass, bool otherPass)
{
	int playerOption = getUserOption();
	
	//Player selects a card to play
	if(playerOption == 1)
	{
		//Not sure how to implement this yet
		//Needs to switch turns if the other player has not passed yet
		
		//selects a card to play based on user selection
		playCard(playerDeck);
		
		//if other player has not passed, change turn
		if()
		{
		}
		//if other player has passed, do not switch turn
		else
	}
	
	else if(playerOption == 2)
	{
		playerPass = true;
		
		//code to determine if round ends?
		//determine the first turn for the next round?
	}
	
}

void Board::printCards()
{
	for (int i = 0; i < playerOneDeck.size(); i++)
	{
		playerOneDeck.at(i)->toString();
	}
}

void Board::printHand()
{
	for (int i = 0; i < playerOneHand.size(); i++)
	{
		playerOneHand.at(i)->toString();
	}
}
