#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include "Cards.h"
#include "BoardRow.h"
using namespace std;

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
	bool isFirstRound;
	int firstTurnChoice;
	bool playerTurn;
	int roundCount;
	bool p1Pass;
	bool p2Pass;
	int turnOption;
	void initializeDecks(string filename, bool p);
    void ability4(bool pl);
    void moraleBoost(bool pl, int row);
    void spy(bool pl);
    void medic(bool pl);
    void scorch(bool pl, int row);
    void clearBoard();
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

//Remember to update this method when we merge.
void Board::playRound(int &p1Score, int &p2Score)
{
	int roundWinner;
	p1Pass = false;
	p2Pass = false;
    if (isFirstRound)
        playerTurn = chooseTurn();
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
    //code for clearing board needed
    clearBoard();
}

void Board::clearBoard()
{
    int p1cards;
    int p2cards;
    for (int i = 0; i < 3; i++)
    {
        p1cards = playerOneRows[i].cards.size();
        for (int j = 0; j < p1cards; j++)
        {
            playerOneGrave.push_back(playerOneRows[i].cards.front());
            playerOneRows[i].cards.erase(playerOneRows[i].cards.begin());
        }
        p2cards = playerTwoRows[i].cards.size();
        for (int j = 0; j < p2cards; j++)
        {
            playerTwoGrave.push_back(playerTwoRows[i].cards.front());
            playerTwoRows[i].cards.erase(playerTwoRows[i].cards.begin());
        }
        playerOneRows[i].reset();
        playerTwoRows[i].reset();
    }
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
        if (!pl && ability != 2) //player one
            playerOneRows[cardRow].cards.push_back(uCard);
        else if (pl && ability != 2)
            playerTwoRows[cardRow].cards.push_back(uCard);
        switch (ability)
        {
            case 1:
                moraleBoost(pl, cardRow);
                break;
            case 2:
                spy(pl);
                if (!pl)
                    playerTwoRows[cardRow].cards.push_back(uCard);
                else
                    playerOneRows[cardRow].cards.push_back(uCard);
                break;
            case 3:
                break;
            case 4:
                scorch(pl, cardRow);
                break;
            case 5:
                break;
        }
        playerHand.erase(playerHand.begin() + index - 1);
	}
    else
    {
        sCard = (SpecialCard*)playerHand.at(index);
        ability = sCard->effect;
        playerHand.erase(playerHand.begin() + index - 1);
        if (!pl)
            playerOneGrave.push_back(sCard);
        else
            playerTwoGrave.push_back(sCard);
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
                ability4(pl);
                break;
        }
    }
    
}

void Board::ability4(bool pl)
{
    int row;
    while (true)
    {
        cout <<"Which row do you want to rally (1, 2, or 3)?\n";
        cin >>row;
        if (row < 1 || row > 3)
            cout <<"Invalid input, try again.\n";
        else
            break;
    }
    row--;
    if (!pl)
        playerOneRows[row].buff();
    else
        playerTwoRows[row].buff();
}

void Board::moraleBoost(bool pl, int row)
{
    if (!pl)
        playerOneRows[row].moraleBoost();
    else
        playerTwoRows[row].moraleBoost();
}

void Board::spy(bool pl)
{
    srand(time(NULL));
    int index = 0;
    for (int i = 0; i < 2; i++)
    {
        if (!pl)
        {
            if (playerOneDeck.size() == 0)
                break;
            index = rand() % playerOneDeck.size();
            playerOneHand.push_back(playerOneDeck.at(index));
            playerOneDeck.erase(playerOneDeck.begin() + index - 1);
        }
        else
        {
            if (playerTwoDeck.size() == 0)
                break;
            index = rand() % playerTwoDeck.size();
            playerTwoHand.push_back(playerTwoDeck.at(index));
            playerTwoDeck.erase(playerTwoDeck.begin() + index - 1);
        }
    }
}

void Board::medic(bool pl) //account for empty grave
{
    srand(time(NULL));
    int index = 0;
    if(pl)
    {
        if (playerOneGrave.size() == 0)
            break;
        index = rand() % playerOneGrave.size();
        playerOneHand.push_back(playerOneGrave.at(index));
        playerOneGrave.erase(playerOneGrave.begin() + index - 1);
    }
    else
    {
        if(playerTwoGrave.size() == 0)
            break;
        index = rand() % playerTwoGrave.size();
        playerTwoHand.push_back(playerTwoGrave.at(index));
        playerTwoGrave.erase(playerTwoGrave.begin() + index - 1);
    }
}

void Board::scorch(bool pl, int row)
{
    int maxStrength = 0;
    if (!pl)
    {
        for (UnitCard *c : playerTwoRows[row].cards)
        {
            if (c->getStrength() > maxStrength)
                maxStrength = c->getStrength();
        }
        for (int i = 0; i < playerTwoRows[row].cards.size(); i++)
        {
            if (playerTwoRows[row].cards.at(i)->getStrength() == maxStrength)
            {
                playerTwoGrave.push_back(playerTwoRows[row].cards.at(i));
                playerTwoRows[row].cards.erase(playerTwoRows[row].cards.begin() + i - 1);
            }
        }
    }
    else
    {
        for (UnitCard *c : playerOneRows[row].cards)
        {
            if (c->getStrength() > maxStrength)
                maxStrength = c->getStrength();
        }
        for (int i = 0; i < playerOneRows[row].cards.size(); i++)
        {
            if (playerOneRows[row].cards.at(i)->getStrength() == maxStrength)
            {
                playerOneGrave.push_back(playerOneRows[row].cards.at(i));
                playerOneRows[row].cards.erase(playerOneRows[row].cards.begin() + i - 1);
            }
        }
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
