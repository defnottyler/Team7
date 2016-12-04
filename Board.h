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
	//void applyModifier(int effect);
	int getRowStr();
    void deBuff();
    void buff();
    void clear();
private:
	//void calcStr();
	int rowPosition;
	bool buffed;
	bool deBuffed;
	int rowStrength;
};

BoardRow::BoardRow()
{
    buffed = false;
    deBuffed = false;
	rowStrength = 0;
}

void BoardRow::setRow(int pos)
{
	rowPosition = pos;
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
	//buffed = false;
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
    buffed = true;
}

int BoardRow::getRowStr()
{
    rowStrength = 0;
    for (UnitCard* c : cards)
        rowStrength += c->getStrength();
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
	void playRound(int &p1Score, int &p2Score);
	void playerOneTurn();
	void playerTwoTurn();
	void playCard(int index, vector<Card*> &playerHand, bool pl); //Puts a card on the field and changes player's turn
	void displayTurnOptions(int playerTurn);
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
int n = 0;
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
			//cout << " p is" << p << endl;
			//cout << "Grabbed data from text file: " << range << ability << hero << name << strength << endl;
			if (p)
				playerOneDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			else {
				playerTwoDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			//playerOneDeck.push_back(card);
			//cout <<"Pushing_back a card to PlayerTwoDeck" << endl;
			}

		}
		else
		{

			index = line.find(" ", 6);
			name = line.substr(6, index - 6);
			ability = stoi(line.substr(index + 1, 1));
			
			if (p)
				playerOneDeck.push_back(new SpecialCard(ability, name)); 

			else {
				playerTwoDeck.push_back(new SpecialCard(ability, name)); 
			}

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
	//cout <<"Before loop\n";
	for (int i = 0; i<10; i++) {
		//cout <<"Start of loop\n";
		//cout << deckSize1 << endl;
		index1 = (int)(rand() % deckSize1);
		//cout << "too fast" << endl;
		//index2 = (int)(rand() % deckSize2);
		//cout <<"After rand\n";
		index2 = index1;
		//cout << "index is " << index1 << endl;
		//cout << "playerOneHand.size() is " << playerOneHand.size() << endl;
		//cout << "playerOneDeck.size() is " << playerOneDeck.size() << endl;
		playerOneHand.push_back(playerOneDeck.at(index1));
		playerOneDeck.erase(playerOneDeck.begin() + index1 - 1);
		deckSize1--;
		//cout << "index is " << index1 << endl;
		//cout << "playerTwoHand.size() is " << playerTwoHand.size() << endl;
		//cout << "playerTwoDeck.size() is " << playerTwoDeck.size() << endl;
		playerTwoHand.push_back(playerTwoDeck.at(index2));
		playerTwoDeck.erase(playerTwoDeck.begin() + index2 - 1);
		deckSize2--;
		//cout <<"End of loop\n";
		//hands are now generated
	}
	//cout << "Hand generator is fine!" << endl; 
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

void Board::displayTurnOptions(int playerTurn)
{
	cout << "Player " << playerTurn << " turn" << endl;
	cout << "Choose one of the following options:" << endl;
	cout << "1) Play a card" << endl;
	cout << "2) Pass turn" << endl;
	cout << "3) Display game board" << endl;
	cout << "4) Display hand" << endl;
	cout << "Player Option: ";
}


void Board::playRound(int &p1Score, int &p2Score)
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

		//Calculate current strength
		p1TotalStrength = 0; p2TotalStrength = 0;
		for (int i = 0; i < 3; i++)
		{
			p1TotalStrength += playerOneRows[i].getRowStr();
			p2TotalStrength += playerTwoRows[i].getRowStr();
		}
	} while(!p1Pass && !p2Pass);

	if (p1TotalStrength > p2TotalStrength)
	{
		p1Score++;
		cout <<"Player one wins the round." <<endl;
	}
	else
	{
		p2Score++;
		cout <<"Player two wins the round." <<endl;
	}
    //round has terminated
}

void Board::playerOneTurn()
{
	int cardIndex;
	displayTurnOptions(1);
	cin >> turnOption;
	if (turnOption == 1) {
		cout << "Select Card to play: ";
		cin >> cardIndex;
		playCard(cardIndex - 1, playerOneHand, false);
	}
	else if (turnOption == 2)
		p1Pass = true;
	else if (turnOption == 3)
		printBoard(1);
	else
		printHand(1);
	playerTurn = !playerTurn;
}

void Board::playerTwoTurn()
{
	int cardIndex;
	displayTurnOptions(2);
	cin >> turnOption;
	if (turnOption == 1) {
		cout << "Select Card to play: ";
		cin >> cardIndex;
		playCard(cardIndex - 1, playerTwoHand, true);
	}
	else if (turnOption == 2)
		p2Pass = true;
	else if (turnOption == 3)
		printBoard(2);
	else
		printHand(2);
	playerTurn = !playerTurn;
}

//Now compiles without error
void Board::playCard(int index, vector<Card*> &playerHand, bool pl) {
	int cardRow;
    int ability;
	UnitCard *uCard;
    SpecialCard *sCard;
	if(playerHand.at(index) -> isUnit)
    {
	uCard = (UnitCard*)playerHand.at(index);
        cardRow = uCard->type;
        ability = uCard->ability;
        if (!pl)
            playerOneRows[cardRow].cards.push_back(uCard);
        else
            playerTwoRows[cardRow].cards.push_back(uCard);
        //TODO: implement ability
        playerHand.erase(playerHand.begin() + index - 1);
	}
    else
    {
        sCard = (SpecialCard*)playerHand.at(index);
        ability = sCard->effect;
        switch (ability)
        {
            case 0:
                for (int i = 0; i < 3; i++)
                {
                    playerOneRows[i].clear();
                    playerTwoRows[i].clear();
                }
                break;
            case 1:
                playerOneRows[0].deBuff();
                playerTwoRows[0].deBuff();
                break;
            case 2:
                playerTwoRows[1].deBuff();
                playerOneRows[1].deBuff();
                break;
            case 3:
                playerOneRows[2].deBuff();
                playerTwoRows[2].deBuff();
                break;
            case 4:
                break;
        }
    }
    
}

void Board::printBoard(int p)
{
	if(p==1){
		cout<<"~~~~~~~~~~~~GAMING BOARD~~~~~~~~~~~~\n";
		cout<<"~~~~Other player's Siege cards: ~~~~\n";
		//display playerTwoRows[2]
		for (int k=0; k < playerTwoRows[2].cards.size(); k++){
		playerTwoRows[2].cards.at(k)->toString();
		}
		cout<<"~~~~Other player's Ranged cards: ~~~~\n";
		//display playerTwoRows[1]
		for (int k=0; k < playerTwoRows[1].cards.size(); k++){
		playerTwoRows[1].cards.at(k)->toString();
		}
		cout<<"~Other player's Close Combat cards: ~\n";
		//display playerTwoRows[0]
		for (int k=0; k < playerTwoRows[0].cards.size(); k++){
		playerTwoRows[0].cards.at(k)->toString();
		}
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your Close Combat cards: \n";
		//display playerOneRows[0]
		for (int k=0; k < playerOneRows[0].cards.size(); k++){
		playerOneRows[0].cards.at(k)->toString();
		}
		cout<<"Your Ranged cards: \n";
		//display playerOneRows[1]
		for (int k=0; k < playerOneRows[1].cards.size(); k++){
		playerOneRows[1].cards.at(k)->toString();
		}
		cout<<"Your Siege cards: \n";
		//display playerOneRows[2]
		for (int k=0; k < playerOneRows[2].cards.size(); k++){
		playerOneRows[2].cards.at(k)->toString();
		}
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your hand: (Name/Type/Abilit/isHero/Strength)\n";
		
	}
	else{
		cout<<"~~~~~~~~~~~~GAMING BOARD~~~~~~~~~~~~\n";
		cout<<"~~~~Other player's Siege cards: ~~~~\n";
		//display playerOneRows[2]
		for (int k=0; k < playerOneRows[2].cards.size(); k++){
		playerOneRows[2].cards.at(k)->toString();
		}
		cout<<"~~~~Other player's Ranged cards: ~~~~\n";
		//display playerOneRows[1]
		for (int k=0; k < playerOneRows[1].cards.size(); k++){
		playerOneRows[1].cards.at(k)->toString();
		}
		cout<<"~Other player's Close Combat cards: ~\n";
		//display playerOneRows[0]
		for (int k=0; k < playerOneRows[0].cards.size(); k++){
		playerOneRows[0].cards.at(k)->toString();
		}
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"~~~~~Your Close Combat cards: ~~~~~~\n";
		//display playerTwoRows[0]
		for (int k=0; k < playerTwoRows[0].cards.size(); k++){
		playerTwoRows[0].cards.at(k)->toString();
		}
		cout<<"~~~~~~~~Your Ranged cards: ~~~~~~~~~\n";
		//display playerTwoRows[1]
		for (int k=0; k < playerTwoRows[1].cards.size(); k++){
		playerTwoRows[1].cards.at(k)->toString();
		}
		cout<<"~~~~~~~~~Your Siege cards: ~~~~~~~~~\n";
		//display playerTwoRows[2]
		for (int k=0; k < playerTwoRows[2].cards.size(); k++){
		playerTwoRows[2].cards.at(k)->toString();
		}
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
		cout<<"Your hand: (Name/Type/Abilit/isHero/Strength)\n";
		
	}
}

void Board::printHand(int p)
{
	cout << "-------HAND-------" << endl; 
	if (p == 1) 
	{
		for (int i=0; i < playerOneHand.size(); ++i) {
			playerOneHand.at(i)->toString();
		}
	}
	else
	{
		for( Card* myCard : playerTwoHand)
			myCard->toString();
	}
	cout << "------------------" << endl; 
}
