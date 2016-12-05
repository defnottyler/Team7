
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include "Cards.h"
#include "BoardRow.h"
#include "Help.h"
using namespace std;

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
	void playerOneTurn(int &p1, int &p2);
	void playerTwoTurn(int &p1, int &p2);
	void playCard(int index, vector<Card*> &playerHand, bool pl); //Puts a card on the field and changes player's turn
	
	//Printing related functions.
	void displayTurnOptions(int playerTurn);
	void printRow(vector<Card*> hand, int start, int end);
	void printRowBorder(int max);
	void printHand(int p);
	void printBoard(int p, int p1Score, int p2Score);
	int maxRowSize();
private:
	//Gaming board related variables.
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
