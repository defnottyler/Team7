#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include "Cards.h"
#include "BoardRow.h"

using namespace std;

//Board class that contains all the variables and functions
//which are needed to construct the gaming sequence.
class Board
{
public:
	
	//Constructor starts entire game and configures board.
	Board(); 
	
	//Randomly assign 10 cards to player's hand.
	void handGenerator();
	
	//Gaming logic variables and functions.
	bool chooseTurn();
	void playRound(int &p1Score, int &p2Score);
	void playerOneTurn();
	void playerTwoTurn();
	void playCard(int index, vector<Card*> &playerHand, bool pl); //Puts a card on the field and changes player's turn
	
	//Printing ralated functions.
	void displayTurnOptions(int playerTurn);
	void printRow(vector<Card*> hand, int start, int end);	
	void printHand(int p);
	void printBoard(int p);
	
private:

	//Gaming board ralated variables.
	BoardRow playerOneRows[3];
	BoardRow playerTwoRows[3];
	
	//Card pointers that point to the deck, hand, and grave.
	vector<Card*> playerOneDeck;
	vector<Card*> playerTwoDeck;
	vector<Card*> playerOneHand;
	vector<Card*> playerTwoHand;
	vector<Card*> playerOneGrave;
	vector<Card*> playerTwoGrave;
	
	//Gaming logic variables.
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
	
	//Gaming logic functions.
	void initializeDecks(string filename, bool p);
	void ability4(bool pl);
    void moraleBoost(bool pl, int row);
    void spy(bool pl);
    void medic(bool pl);
    void scorch(bool pl, int row);
    void clearBoard();
};

//Board constructor.
Board::Board()
{

	//Construct the gaming rows.
	for (int i = 0; i < 3; i++)
	{
		playerOneRows[i].setRow(i);
		playerTwoRows[i].setRow(i);
	}

	//Set all the variables.
	p1TotalStrength = 0;
	p2TotalStrength = 0;
	boardMod = 0;
	isFirstRound = true;
	
	//Loads card text file.
	initializeDecks("PlayerOneDeck.txt", true);
	initializeDecks("PlayerTwoDeck.txt", false);
	cout << "Loading Decks from Cards.txt" << endl;
	
	//Randomly assign 10 cards to player's hand.
	handGenerator();
}

//Reading from text file and construct the deck.
void Board::initializeDecks(string filename, bool p)
{
	
	//Function variables.
	ifstream d_one(filename);
	string line;
	string name;
	bool hero = false;
	int strength, ability, range;
	int index = 5;
	
	//Reading file.
	while (getline(d_one, line))
	{ 
		
		if (line.substr(0, 4).compare("true") == 0)
		{
			index = line.find(" ", 5);
			name = line.substr(5, index - 5);
			replace(name.begin(), name.end(), '_', ' ');
			index++;
			strength = stoi(line.substr(index, 2));
			index = line.find(" ", index) + 1;
			range = stoi(line.substr(index, 1));
			index = line.find(" ", index) + 1;
			ability = stoi(line.substr(index, 1));
			index = line.find(" ", index) + 1;
			
			if (stoi(line.substr(index, 1)) == 1)
			{
				hero = true;
			}
			
			if (p)
				playerOneDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			
			else {
				playerTwoDeck.push_back(new UnitCard(range, ability, hero, name, strength));
			}
		}
		
		else
		{
			index = line.find(" ", 6);
			name = line.substr(6, index - 6);
			replace(name.begin(), name.end(), '_', ' ');
			ability = stoi(line.substr(index + 1, 1));
			
			if (p)
				playerOneDeck.push_back(new SpecialCard(ability, name)); 

			else {
				playerTwoDeck.push_back(new SpecialCard(ability, name)); 
			}
		}
	}
	
	//Close the input stream.
	d_one.close();
}

//Puts cards from deck into player hand(s).
void Board::handGenerator()			
{
	//Random variable.
	srand(time(NULL));

	//Function variables.
	int deckSize1 = playerOneDeck.size();
	int deckSize2 = playerTwoDeck.size();
	int index1;
	int index2;
	
	//Pushing cards into player hand
	for (int i = 0; i<10; i++) {
		index1 = (int)(rand() % deckSize1);
		index2 = (int)(rand() % deckSize2);
		playerOneHand.push_back(playerOneDeck.at(index1));
		playerOneDeck.erase(playerOneDeck.begin() + index1);
		deckSize1--;;
		playerTwoHand.push_back(playerTwoDeck.at(index2));
		playerTwoDeck.erase(playerTwoDeck.begin() + index2);
		deckSize2--;
		//hands are now generated
	}
}

//Decide the turn.
bool Board::chooseTurn()
{
	srand(time(NULL));
	firstTurnChoice = (rand() % 2) + 1;
    if (firstTurnChoice == 1)
        return true;
    else
        return false;
}

//Prints the options.
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

//Gaming sequence.
void Board::playRound(int &p1Score, int &p2Score)
{
	//function variables.
	int roundWinner;
	p1Pass = false;
	p2Pass = false;
	
	//Decide turn when game starts.
    if (isFirstRound)
        playerTurn = chooseTurn();
	
	//Going back and forth.
	do {
		if (playerTurn) {
			playerOneTurn();
		}
		else 
		{
			playerTwoTurn();
		}
		if (p1Pass && !p2Pass) 
		{
			if (playerTwoHand.size() == 0)
				p2Pass = true;
			while (!p2Pass)
				playerTwoTurn();
		}
		if (p2Pass && !p1Pass) 
		{
			if (playerOneHand.size() == 0)
				p1Pass = true;
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
	
	//Counting scores
	if (p1TotalStrength > p2TotalStrength)
	{
		p1Score++;
		cout <<"Player one wins the round." <<endl;
	}
	else if (p1TotalStrength < p2TotalStrength)
	{
		p2Score++;
		cout <<"Player two wins the round." <<endl;
	}
	else
	{
		cout <<"Draw\n";
		p1Score++;
		p2Score++;
	}
	
	//Clear the gaming board.
	clearBoard();
    //round has terminated
}

//Clear the current gaming board.
void Board::clearBoard()
{
	//function variables.
    int p1cards;
    int p2cards;
    
    //Clearing the board.
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

//Playing options for player one.
void Board::playerOneTurn()
{
	//Force pass.
	if (playerOneHand.size() == 1)
	{
		p1Pass = true;
		return;
	}
	//Function variables.
	int cardIndex;
	
	displayTurnOptions(1);
	cin >> turnOption;
	
	//Playing option 1, play a card.
	if (turnOption == 1) {
		cout << "Select Card to play: ";
		cin >> cardIndex;
		while( cin.fail() || cardIndex < 1 || cardIndex > playerTwoHand.size())
		{
			cout <<"Please choose a card from the numbered list " << endl;
			printHand(1);
			cin.clear();
			cin.ignore(256,'\n');
			cin >> cardIndex;
		}
		playCard(cardIndex - 1, playerOneHand, false);
		playerTurn = !playerTurn;
		//cout << "\033[2J\033[1;1H";
	}
	
	//Playing option 2, pass.
	else if (turnOption == 2){
		p1Pass = true;
		playerTurn = !playerTurn;
		//cout << "\033[2J\033[1;1H";
	}
	
	//Playing option 3, print board.
	else if (turnOption == 3)
		printBoard(1);
		
	else
		printHand(1);
	
}

//Playing options for player two.
void Board::playerTwoTurn()
{
	//Force pass.
	if (playerTwoHand.size() == 1)
	{
		p2Pass = true;
		return;
	}
	
	//Function variables.
	int cardIndex;
	
	displayTurnOptions(2);
	cin >> turnOption;
	
	////Playing option 1, play a card.
	if (turnOption == 1) {
		cout << "Select Card to play: ";
		cin >> cardIndex;
		while( cin.fail() || cardIndex < 1 || cardIndex > playerTwoHand.size())
		{
			cout <<"Please choose a card from the numbered list " << endl;
			printHand(2);
			cin.clear();
			cin.ignore(256,'\n');
			cin >> cardIndex;
		}
		
		playCard(cardIndex - 1, playerTwoHand, true);
		playerTurn = !playerTurn;
		//cout << "\033[2J\033[1;1H";
	}
	
	//Playing option 2, pass.
	else if (turnOption == 2){
		p2Pass = true;
		playerTurn = !playerTurn;
		//cout << "\033[2J\033[1;1H";
	}
	
	//Playing option 3, print board.
	else if (turnOption == 3){
		printBoard(2);
		
	}
	
	else{
		printHand(2);
	}
}

//Function to "play" a card.
void Board::playCard(int index, vector<Card*> &playerHand, bool pl) {
	
	//Function variables
	int cardRow;
    int ability;
	UnitCard *uCard;
    SpecialCard *sCard;
    
    //Check if is unit card/
	if(playerHand.at(index) -> isUnit)
    {
		uCard = (UnitCard*)playerHand.at(index);
        cardRow = uCard->type;
        ability = uCard->ability;
        
        if (!pl && ability != 2) //player one
            playerOneRows[cardRow].add(uCard);
        
        else if (pl && ability != 2)
            playerTwoRows[cardRow].add(uCard);
        
        switch (ability)
        {
            case 1:
                moraleBoost(pl, cardRow);
                break;
            case 2:
                spy(pl);
                if (!pl)
                    playerTwoRows[cardRow].add(uCard);
                else
                    playerOneRows[cardRow].add(uCard);
                break;
            case 3:
				medic(pl);
                break;
            case 4:
                scorch(pl, cardRow);
                break;
        }
        
        //playerHand.at(index) = NULL;
        playerHand.erase(playerHand.begin() + index);
	}
	
	//Special card.
    else
    {
        sCard = (SpecialCard*)playerHand.at(index);
        ability = sCard->effect;
        playerHand.erase(playerHand.begin() + index);
        
        if (!pl)
            playerOneGrave.push_back(sCard);
        
        else
            playerTwoGrave.push_back(sCard);
        
        switch (ability)
        {
            case 0:
				cout <<"Bleach\n";
                for (int i = 0; i < 3; i++)
                {
                    playerOneRows[i].clear();
                    playerTwoRows[i].clear();
                }
                break;
            case 1:
				cout <<"Diabetes\n";
                playerOneRows[0].deBuff();
                playerTwoRows[0].deBuff();
                break;
            case 2:
				cout <<"Waffles\n";
                playerTwoRows[1].deBuff();
                playerOneRows[1].deBuff();
                break;
            case 3:
				cout <<"Soap\n";
                playerOneRows[2].deBuff();
                playerTwoRows[2].deBuff();
                break;
            case 4:
                ability4(pl);
                break;
        }
    }
}

//Implement ability
void Board::ability4(bool pl)
{
	//Function variable.
    int row;
    
    //Choose row.
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

//Morale booster.
void Board::moraleBoost(bool pl, int row)
{
    if (!pl)
        playerOneRows[row].moraleBoost();
        
    else
        playerTwoRows[row].moraleBoost();
}

//Spy function.
void Board::spy(bool pl)
{
	
	//Function variable.
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
            playerOneDeck.erase(playerOneDeck.begin() + index);
        }
        
        else
        {
            if (playerTwoDeck.size() == 0)
                break;
            index = rand() % playerTwoDeck.size();
            playerTwoHand.push_back(playerTwoDeck.at(index));
            playerTwoDeck.erase(playerTwoDeck.begin() + index);
        }
    }
}

//Medic function.
void Board::medic(bool pl) 
{
	//Fnction variable.
	UnitCard *card;
	int choice;
	
    if (!pl)
    {
		if (playerOneGrave.size() == 0)
			return;
		
		while (true)
		{
			cout <<"Choose a card to play (can't be hero or special):\n";
			for (int i = 0; i < playerOneGrave.size(); i++)
			{
				cout <<i+1<<". "<<playerOneGrave.at(i)->name<<endl;
			}
			cin >>choice; choice--;
			if (choice < 0 || choice > playerOneGrave.size() - 1)
			{
				cout <<"Invalid choice, try again.\n";
				continue;
			}
			if (playerOneGrave.at(choice)->isUnit)
			{
				card = (UnitCard*)playerOneGrave.at(choice);
				if (!card->isHero)
				{
					playCard(choice, playerOneGrave, pl);
					break;
				}
				else
				{
					cout <<"Play a non hero card.\n";
					continue;
				}
			}
			else
			{
				cout <<"Select a unit card.\n";
				continue;
			}
		}
	}
	
	else
	{
		if (playerTwoGrave.size() == 0)
			return;
		while (true)
		{
			cout <<"Choose a card to play (can't be hero or special):\n";
			for (int i = 0; i < playerTwoGrave.size(); i++)
			{
				cout <<i+1<<". "<<playerTwoGrave.at(i)->name<<endl;
			}
			cin >>choice; choice--;
			if (choice < 0 || choice > playerTwoGrave.size() - 1)
			{
				cout <<"Invalid choice, try again.\n";
				continue;
			}
			if (playerTwoGrave.at(choice)->isUnit)
			{
				card = (UnitCard*)playerTwoGrave.at(choice);
				if (!card->isHero)
				{
					playCard(choice, playerTwoGrave, pl);
					break;
				}
				else
				{
					cout <<"Play a non hero card.\n";
					continue;
				}
			}
			else
			{
				cout <<"Select a unit card.\n";
				continue;
			}
		}
	}
}

//Scorch function.
void Board::scorch(bool pl, int row)
{
	//Function variable.
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
                playerTwoRows[row].cards.erase(playerTwoRows[row].cards.begin() + i);
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
                playerOneRows[row].cards.erase(playerOneRows[row].cards.begin() + i);
            }
        }
    }
}

//Print board function.
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

/*
void Board::printRow(vector<Card*> hand, int start, int end)
{
  for(int l = 0; l <= 8 ; l++)
  {
    for(int c = start; c <= end; c++)
    {
      if(l == 1 || l == 8)
      {
        cout << "  ^**********************^";
      }
      else if(l == 2)
      {
        cout << "  ^";
        printf("                      %2d^", c);
      }
      else if(l == 3)
      {
        cout << "  ^ ";
        printf("%-20s                    ^", hand.at(c)->name);
      }
      else if(l == 4)
      {
        cout << "  ^ Strength: ";
        printf("%2d          ^", hand.at(c)->strength);
      }
      else if(l == 5)
      {
        cout << "  ^ Ability: ";
        printf("%d           ^", hand.at(c)->ability);
      }
      else if(l == 6)
      {
        if(hand.at(c)->isHero)
        {
          cout << "  ^ Hero Card            ^";
        }
        else
        {
          cout << "  ^                      ^";
        }
      }
      else if(l == 7)
      {
        cout << "  ^                      ^";
      }
    }
    cout << endl;
  }
  cout << endl;
}
*/
void Board::printHand(int p)
{
	cout << "-------HAND-------" << endl; 
	int cardNumberOne = 0;
	int cardNumberTwo = 0;
	if (p == 1) 
	{
		
		
		for (int i = 0; i < playerOneHand.size(); ++i) {
			cardNumberOne++;
			cout << cardNumberOne << " ";
			playerOneHand.at(i)->toString();
		}
	}
	else
	{
		for (int i = 0; i < playerTwoHand.size(); ++i) {
			cardNumberTwo++;
			cout << cardNumberTwo << " ";
			playerTwoHand.at(i)->toString();
		}
	}
	cout << "------------------" << endl; 
}
