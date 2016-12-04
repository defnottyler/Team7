#include <iostream>
#include "Board.h"
#include "Cards.h"

using namespace std;

int main()
{	
	//Main variables
	//checks for input validation for new game
	string yesOrNo = "j";
	
	//checks for new game
	bool play = true;
	
	while(play){
		
	//Magic terminal eraser
	//ANSI escape codes:
	//\033[2J clears screen
	//\033[1;1H replace cursor
	cout << "\033[2J\033[1;1H"; 
	
	//Construct new gaming board
	Board *myBoard = new Board();
	
	//Initializing scores
	int p1Score = 0;
	int p2Score = 0;
	
	//Output Score
	do
	{
		myBoard->playRound(p1Score, p2Score);
		cout << p1Score <<endl;
		cout << p2Score <<endl;
	} 
	
	//check score
	while (p1Score != 2 && p2Score != 2);
	
	//ouput winner
	if (p1Score > p2Score)
	{	
		//Output for player One winner
		cout <<"Player One is the winner!!" << endl;
	}
	else if (p1Score < p2Score)
	{
		//Output for player Two winner
		cout <<"Player Two is the winner!!" << endl;
	}
	else
	{
		//Output for draw
		cout <<"The game is a draw."<<endl;
	}
	
	//Prompt for players to play again
	cout << "Play again?\n" << "y/n" << endl;
	
	cin >> yesOrNo;
	
	//Input validation loop for yesOrno input	
	while ((yesOrNo != "y" && yesOrNo != "Y" && yesOrNo != "N" && yesOrNo != "n") || cin.fail() )
	{
		//promt the player to re-enter input
		cout << "please enter y for yes or n for no" << endl;
		//clears the cin stream
		cin.clear();
		//no clue
		cin.ignore(256,'\n');
		//take in new input for user
		cin >> yesOrNo;
	} 
	//Game stops
	if (yesOrNo == "n" || yesOrNo == "N")
		{
			play = false;
		}
	
}	
	//program terminates succesfully
	return 0;
}
