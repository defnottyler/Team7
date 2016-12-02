//#ifndef
//#def SOMENAME

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
	public:
	Board(); //Constructor starts entire game and configures board.
	void handGenerator();
	bool chooseTurn();
	void printHand(int p);
	void printBoard(int p);
	void playRound(int *p1Score, int *p2Score);
	void playerOneTurn();
	void playerTwoTurn();
	void playCard(int index, vector<Card*> playerHand, bool pl); //Puts a card on the field and changes player's turn
	void displayTurnOptions();
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
	int p1TotalStrength;
	int p2TotalStrength;
	int boardMod;
	bool isFirstRound;
	int firstTurnChoice;
	bool playerTurn;
	int roundCount;
	bool p1Pass;
	bool p2Pass;
	int turnOption;
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

	p1TotalStrength = 0;
	p2TotalStrength = 0;
	boardMod = 0;
	isFirstRound = true;

	initializeDecks("PlayerOneDeck.txt", true);
	initializeDecks("PlayerTwoDeck.txt", false);
	cout << "Loading Decks from Cards.txt" << endl;
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
			index = line.find(" ", index) + 1;
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

	for (int i = 0; i<10; i++) {

		index1 = rand() % deckSize1;
		index2 = rand() % deckSize2;

		playerOneHand.push_back(playerOneDeck.at(index1));
		playerOneDeck.erase(playerOneDeck.begin() + index1 - 1);
		deckSize1--;

		playerTwoHand.push_back(playerTwoDeck.at(index2));
		playerTwoDeck.erase(playerTwoDeck.begin() + index2 - 1);
		deckSize2--;
		//hands are now generated
	}
}
//bools and stuff
bool Board::chooseTurn()
{
	srand(time(NULL));
	firstTurnChoice = (rand() % 2) + 1;
    if (firstTurnChoice == 1)
        return true;
    else
        return false;
	//return firstTurnChoice;
}

void Board::displayTurnOptions()
{
	cout << "Choose one of the following options:" << endl;
	cout << "1) Play a card" << endl;
	cout << "2) Pass turn" << endl;
	cout << "Player Option: ";
}

void Board::playRound(int *p1Score, int *p2Score)
{
	int roundWinner;
	p1Pass = false;
	p2Pass = false;
    if (isFirstRound)
        playerTurn = chooseTurn();
	/*if (isFirstRound) {
		playerTurn = chooseTurn();
		if (playerTurn == 1) {
			playerOneTurn();
			playerTurn = 2;
		}
		else {
			playerTwoTurn();
			playerTurn = 1;
		}
		isFirstRound = false;
	} */

	do {
		if (playerTurn) {
			playerOneTurn();
		}
		else {
			playerTwoTurn();
		}
		if (p1Pass && !p2Pass) {
			while (!p2Pass)
				playerTwoTurn();
		}
		if (p2Pass && !p1Pass) {
			while (!p1Pass)
				playerOneTurn();
		}
	} while(!p1Pass && !p2Pass);

	if (p1TotalStrength > p2TotalStrength)
		p1Score++;
	else
		p2Score++;
    //round has terminated?
}

void Board::playerOneTurn()
{
	int cardIndex;
	displayTurnOptions();
	cin >> turnOption;
	if (turnOption == 1) {
		cout << "Select Card to play: ";
		cin >> cardIndex;
		playCard(cardIndex, playerOneHand, false);
	}
	else 
		p1Pass = true;

	playerTurn = !playerTurn;
}

void Board::playerTwoTurn()
{
	int cardIndex;
	displayTurnOptions();
	cin >> turnOption;
	if (turnOption == 1) {
		cout << "Select Card to play: ";
		cin >> cardIndex;
		playCard(cardIndex, playerTwoHand, true);
	}
	else
		p2Pass = true;

	playerTurn = !playerTurn;
}

//Now compiles without error
void Board::playCard(int index, vector<Card*> playerHand, bool pl) {
	int cardRow;
    int ability;
	UnitCard *currentCard;
	if(playerHand.at(index) -> isUnit)
    {
		currentCard = (UnitCard*)playerHand.at(index);
        cardRow = currentCard->type;
        ability = currentCard->ability;
        switch (cardRow)
        {
                if (!pl)
                    playerOneRows[cardRow].cards.push_back(currentCard);
                else
                    playerTwoRows[cardRow].cards.push_back(currentCard);
                //TODO: implement ability
        }
	}
    else
    {
            //Special card procedure
    }
    
}

void Board::printBoard(int p)
{
	if(p==1){
		cout<<"~~~~~~~~~~~~GAMING BOARD~~~~~~~~~~~~\n";
		cout<<"Other player's Siege cards: \n";
		//display playerTwoRows[2]
		cout<<"Other player's Ranged cards: \n";
		//display playerTwoRows[1]
		cout<<"Other player's Close Combat cards: \n";
		//display playerTwoRows[0]
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your Close Combat cards: \n";
		//display playerOneRows[0]
		cout<<"Your Ranged cards: \n";
		//display playerOneRows[1]
		cout<<"Your Siege cards: \n";
		//display playerOneRows[2]
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your hand: (Name/Type/Abilit/isHero/Strength)\n";
		for (int i = 0; i < playerOneHand.size(); i++)
	{
		playerOneHand.at(i)->toString();
	}
	}
	else{
		cout<<"~~~~~~~~~~~~GAMING BOARD~~~~~~~~~~~~\n";
		cout<<"Other player's Siege cards: \n";
		//display playerTwoRows[2]
		cout<<"Other player's Ranged cards: \n";
		//display playerTwoRows[1]
		cout<<"Other player's Close Combat cards: \n";
		//display playerTwoRows[0]
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your Close Combat cards: \n";
		//display playerOneRows[0]
		cout<<"Your Ranged cards: \n";
		//display playerOneRows[1]
		cout<<"Your Siege cards: \n";
		//display playerOneRows[2]
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your hand: (Name/Type/Abilit/isHero/Strength)\n";
		for (int i = 0; i < playerTwoHand.size(); i++)
	{
		playerTwoHand.at(i)->toString();
	}
	}
}
