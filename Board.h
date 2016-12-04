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

/*
 * This helper method prints out a series of cards from a vector of Card pointers. It prints cards
 * between and including a specified start and end index. It is designed to assist the printHand(int p)
 * and printBoard(int p, int p1Score, int p2Score) methods.
 */
void Board::printRow(vector<Card*> hand, int start, int end)
{
  //Iterates through the "height" of the cards, adding in the appropriate elements per line
  for(int l = 0; l <= 6 ; l++)
  {
	//Prints out a the "l'th" line for each card in the row
    for(int c = start; c < end; c++)
    {
	  //Placeholder variables for the two different card types
      UnitCard* unit = 0;
      SpecialCard* special = 0;

	  //Checks the type of card and assigns it to the appropriate variable
      if(hand.at(c)->isUnit)
        unit = (UnitCard*)hand.at(c);
      else
        special = (SpecialCard*)hand.at(c);
      
      //Prints the top and bottom borders of the cards
      if(l == 1 || l == 6)
      {
        cout << "  ^**********************^";
      }
      //Prints the 2nd line which labels the card "Hero" if it is a Hero card
      //Also lists the card number for selection purposes
      else if(l == 2)
      {
        cout << "  ^";
        if(hand.at(c)->isUnit && unit->isHero)
        {
	      printf(" (Hero)            %2d ^", c + 1);
        }
        else
        {
          printf("                   %2d ^", c + 1);
        }
      }
      //Prints the 3rd line of each card which contains the name
      else if(l == 3)
      {
	    string n = hand.at(c)->name;
        cout << "  ^ ";
        printf("%-20s ^", n.c_str());
      }
      //Prints the 4th line of each card
      //Displays the strength for UnitCard types
      //Displays the effect for SpecialCard types
      else if(l == 4)
      {
        if(hand.at(c)->isUnit)
        {
	      int s = unit->strength;
          cout << "  ^ Strength: ";
          printf("%2d         ^", (int)s);
        }
        else
        {
	      int e = special->effect;
          cout << "  ^ Effect: ";
          printf("%d            ^", (int)e);
        }
      }
      //Prints out the 5th line for each card
      //Contains Ability for UnitCard types (medic, scorch, spy, morale booster)
      //Contains Effected row(s) for SpecialCard types(Both Close/Ranged/Siege, Any Player Row)
      else if(l == 5)
      {
        if(hand.at(c)->isUnit)
        {
	      int a = unit->ability;
          cout << "  ^ Ability: ";
          printf("%d           ^", (int)a); 
        }
        else
        {
	  if(special->effect != 4)
          {
	    cout << "  ^ Row: ";
            if(special->effect == 1)
            {
	       cout << "Both Close      ^";
            }
	    else if(special->effect == 2)
            {
	       cout << "Both Ranged     ^";
            }
            else
            {
	       cout << "Both Siege      ^";
            }
          }
          else
          {
	    cout << "  ^ Rows: Any Player Row ^";
          }
        }
      }
    }
    cout << endl;
  }
}

/*
 * Method prints out the hand of the current player. The formatting displays a
 * player's hand in rows of 5 cards.
 */
void Board::printHand(int p)
{
  //Displays player 1's hand
  if (p == 1)
  {
	//The total number of cards that player 1 has in their hand
    int cardsInHand = playerOneHand.size();
    
    //Prints out a message to the player when there are no more cards left
    if(cardsInHand == 0)
    {
      cout << "You have no more cards to play";
    }
    //When there are still cards in player 1's deck, print out the deck
    else
    {
	  //Determines number of rows
      int indices = (int)(cardsInHand / 5);
      if(cardsInHand % 5 > 0)
      {
        indices++;
      }
      int rowIndices[indices];
      rowIndices[0] = 0;
      
      //Assigns appropriate index values which will be used to populate rows
      for(int i = 1; i <= indices; i++)
      {
        if(i < indices)
        {
          rowIndices[i] = 5 * i;
        }
        if(i == indices)
        {
          rowIndices[i] = cardsInHand;
        }
      }
      //Prints out a header
      for(int c = 1; c <= 2; c++)
      {
        for(int undrScr = 1; undrScr <= 26; undrScr++)
        {
          cout << "_";
        }
      }
      cout << "        YOUR  HAND        ";
      for(int c = 1; c <= 2; c++)
      {
        for(int undrScr = 1; undrScr <= 26; undrScr++)
        {
          cout << "_";
        }
      }
      //Calls the printRow() method for each row of cards to be printed
      for(int i = 0; i < indices; i++)
      {
        printRow(playerOneHand, rowIndices[i], rowIndices[i + 1]);
      }
      //Prints a border for the bottom of the deck
      printRowBorder(5);
    }
  }
    
  //Displays player 2's hand
  else
  {
	//The total number of cards that player 1 has in their hand
    int cardsInHand = playerTwoHand.size();
    
    //Prints out a message to the player when there are no more cards left
    if(cardsInHand == 0)
    {
      cout << "You have no more cards to play";
    }
    //When there are still cards in player 1's deck, print out the deck
    else
    {
      //Determines number of rows
      int indices = (int)(cardsInHand / 5);
      if(cardsInHand % 5 > 0)
      {
        indices++;
      }
      int rowIndices[indices];
      rowIndices[0] = 0;
      
      //Assigns appropriate index values which will be used to populate rows
      for(int i = 1; i <= indices; i++)
      {
        if(i < indices)
        {
          rowIndices[i] = 5 * i;
        }
        if(i == indices)
        {
          rowIndices[i] = cardsInHand;
        }
      }

      //Prints out a header
      for(int c = 1; c <= 2; c++)
      {
        for(int undrScr = 1; undrScr <= 26; undrScr++)
        {
          cout << "_";
        }
      }
      cout << "        YOUR  HAND        ";
      for(int c = 1; c <= 2; c++)
      {
        for(int undrScr = 1; undrScr <= 26; undrScr++)
        {
          cout << "_";
        }
      }
      //Calls the printRow() method for each row of cards to be printed
      for(int i = 0; i < indices; i++)
      {
        printRow(playerTwoHand, rowIndices[i], rowIndices[i + 1]);
      }
      //Prints a border for the bottom of the deck
      printRowBorder(5);
    }
  }
}

/*
 * Finds the maximum number of cards contained in a row on the board
 * This method is used to determine the length of the borders for the
 * printBoard() method
 */
int Board::maxRowSize()
{
  int max = 0;
  
  //Compares the sizes of each row on player 1's side and assigns the largest value to max
  for(int r = 0; r < 3; r++)
  {
    int size = 0;
    size = playerOneRows[r].cards.size();
    if(max < size)
    {
      max = size;
    }
  }
  
  //Compares the sizes of each row on player 2's side and assigns the largest value to max
  for(int r = 0; r < 3; r++)
  {
    int size = 0;
    size = playerOneRows[r].cards.size();
    if(max < size)
    {
      max = size;
    }
  }
  
  return max;
}

/*
 * Prints a border for a row of cards using the '_' character
 */
void Board::printRowBorder(int max)
{
  if(max == 0)
  {
    max = 1;
  }
  for(int c = 1; c <= max; c++)
  {
    for(int undrScr = 1; undrScr <= 26; undrScr++)
    {
      cout << "_";
    }
  }
  cout << endl;
}

/*
 * 
 */
void Board::printBoard(int p, int p1Score, int p2Score)
{
  cout << "  _______________________________" << endl;
  cout << " | Score        Player 1 : " << p1Score << "     |" << endl;
  cout << " |              Player 2 : " << p2Score << "     |" << endl;
  cout << " |_______________________________|" << endl << endl;
  
  //Board Setup for Player 1
  if(p == 1)
  {
    int rowStrength = 0;
    for(int r = 2; r >= 0; r--)
    {
      BoardRow row = playerTwoRows[r];
      rowStrength = row.getRowStr();
      int maxNumCards = 0;
      maxNumCards = maxRowSize();

      printRowBorder(maxNumCards);
      cout << endl;
      cout <<   ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
      if(r == 2)
        cout << "^ (P2) Siege         Row Strength: ";
      else if(r == 1)
        cout << "^ (P2) Ranged        Row Strength: ";
      else
        cout << "^ (P2) Close Combat  Row Strength: ";
         printf("%3d ", (int)rowStrength);
      cout <<   " Weather Effect: ";
      if(r == 2 && row.getDeBuffed())
      {
         cout <<   "Rain  ";
      }
      else if(r == 1 && row.getDeBuffed())
      {
	 cout <<   "Fog   ";
      }
      else if(r == 0 && row.getDeBuffed())
      {
	 cout <<   "Frost ";
      }
      else
      {
         cout <<   "NONE  ";
      }
      
      cout <<   " Buff Effect: ";
      if(row.getBuffed())
      {
         cout <<"X2 Multiplier v\n";
      }
      else
      {
         cout <<"NONE          v\n";
      }
      cout <<   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl;
      
      vector<Card*> printCards;
      for(int c = 0; c < row.cards.size(); c++)
      {
         printCards.push_back(row.cards.at(c));
      }
      printRow(printCards, 0, (int)printCards.size());
      printRowBorder(maxNumCards);
    }
    
    int totalOppStr = 0;
    int totalPlaStr = 0;
    for(int i = 0; i < 3; i++)
    {
      totalOppStr += playerTwoRows[i].getRowStr();
      totalPlaStr += playerOneRows[i].getRowStr();
    }
    cout << "OPPONENT'S TOTAL STRENGTH: " << totalOppStr << endl;
    cout << "PLAYER'S TOTAL STRENGTH:   " << totalPlaStr << endl;

    for(int r = 0; r <= 2; r++)
    {
      BoardRow row = playerOneRows[r];
      rowStrength = row.getRowStr();
      int maxNumCards = 0;
      maxNumCards = maxRowSize();

      printRowBorder(maxNumCards);
      cout << endl;
      cout <<   ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
      if(r == 2)
        cout << "^ (P1) Siege         Row Strength: ";
      else if(r == 1)
        cout << "^ (P1) Ranged        Row Strength: ";
      else
        cout << "^ (P1) Close Combat  Row Strength: ";
         printf("%3d ", (int)rowStrength);
      cout <<   " Weather Effect: ";
      if(r == 2 && row.getDeBuffed())
      {
         cout <<   "Rain  ";
      }
      else if(r == 1 && row.getDeBuffed())
      {
	 cout <<   "Fog   ";
      }
      else if(r == 0 && row.getDeBuffed())
      {
	 cout <<   "Frost ";
      }
      else
      {
         cout <<   "NONE  ";
      }
      
      cout <<   " Buff Effect: ";
      if(row.getBuffed())
      {
         cout <<"X2 Multiplier v\n";
      }
      else
      {
         cout <<"NONE          v\n";
      }
      cout <<   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl;
      
      vector<Card*> printCards;
      for(int c = 0; c < row.cards.size(); c++)
      {
         printCards.push_back(row.cards.at(c));
      }
      printRow(printCards, 0, (int)printCards.size());
      printRowBorder(maxNumCards);
    }
  }
  
  //Board Setup for Player 2
  else
  {
    {
    int rowStrength = 0;
    for(int r = 2; r >= 0; r--)
    {
      BoardRow row = playerOneRows[r];
      rowStrength = row.getRowStr();
      int maxNumCards = 0;
      maxNumCards = maxRowSize();

      printRowBorder(maxNumCards);
      cout << endl;
      cout <<   ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
      if(r == 2)
        cout << "^ (P1) Siege         Row Strength: ";
      else if(r == 1)
        cout << "^ (P1) Ranged        Row Strength: ";
      else
        cout << "^ (P1) Close Combat  Row Strength: ";
         printf("%3d ", (int)rowStrength);
      cout <<   " Weather Effect: ";
      if(r == 2 && row.getDeBuffed())
      {
         cout <<   "Rain  ";
      }
      else if(r == 1 && row.getDeBuffed())
      {
	 cout <<   "Fog   ";
      }
      else if(r == 0 && row.getDeBuffed())
      {
	 cout <<   "Frost ";
      }
      else
      {
         cout <<   "NONE  ";
      }
      
      cout <<   " Buff Effect: ";
      if(row.getBuffed())
      {
         cout <<"X2 Multiplier v\n";
      }
      else
      {
         cout <<"NONE          v\n";
      }
      cout <<   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl;
      
      vector<Card*> printCards;
      for(int c = 0; c < row.cards.size(); c++)
      {
         printCards.push_back(row.cards.at(c));
      }
      printRow(printCards, 0, (int)printCards.size());
      printRowBorder(maxNumCards);
    }
    
    int totalOppStr = 0;
    int totalPlaStr = 0;
    for(int i = 0; i < 3; i++)
    {
      totalOppStr += playerOneRows[i].getRowStr();
      totalPlaStr += playerTwoRows[i].getRowStr();
    }
    cout << "OPPONENT'S TOTAL STRENGTH: " << totalOppStr << endl;
    cout << "PLAYER'S TOTAL STRENGTH:   " << totalPlaStr << endl;

    for(int r = 0; r <= 2; r++)
    {
      BoardRow row = playerTwoRows[r];
      rowStrength = row.getRowStr();
      int maxNumCards = 0;
      maxNumCards = maxRowSize();

      printRowBorder(maxNumCards);
      cout << endl;
      cout <<   ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
      if(r == 2)
        cout << "^ (P2) Siege         Row Strength: ";
      else if(r == 1)
        cout << "^ (P2) Ranged        Row Strength: ";
      else
        cout << "^ (P2) Close Combat  Row Strength: ";
         printf("%3d ", (int)rowStrength);
      cout <<   " Weather Effect: ";
      if(r == 2 && row.getDeBuffed())
      {
         cout <<   "Rain  ";
      }
      else if(r == 1 && row.getDeBuffed())
      {
	 cout <<   "Fog   ";
      }
      else if(r == 0 && row.getDeBuffed())
      {
	 cout <<   "Frost ";
      }
      else
      {
         cout <<   "NONE  ";
      }
      
      cout <<   " Buff Effect: ";
      if(row.getBuffed())
      {
         cout <<"X2 Multiplier v\n";
      }
      else
      {
         cout <<"NONE          v\n";
      }
      cout <<   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl;
      
      vector<Card*> printCards;
      for(int c = 0; c < row.cards.size(); c++)
      {
         printCards.push_back(row.cards.at(c));
      }
      printRow(printCards, 0, (int)printCards.size());
      printRowBorder(maxNumCards);
    }
  }
}
}  
