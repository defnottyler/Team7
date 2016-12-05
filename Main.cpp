#include <iostream>
#include "Board.h"
#include "Cards.h"
#include "Help.h"
#include <string>
using namespace std;


//prints formatted welcoming text
void center_print(string s)
{
	int l = s.length();
	int pos = (int)((80 - l) / 2);

	for (int i = 0; i < pos; ++i)
	{
		cout << " ";
	}
	cout << s << endl;


}

void print_menu()
{
	cout << "Please select an option:"<< endl;
	cout << "1. Play Game!" << endl;
	cout << "2. Help" << endl;
	cout << "3. Game Score" << endl;
	cout << "4. Quit" << endl;
}

int main()
{
	int p1GamePoints = 0;
	int p2GamePoints = 0;
	
	//clear console from any previous things
	cout << "\033[2J\033[1;1H" << endl;
	
	center_print("WELCOME TO CARDS AND BARDS");
	center_print("Cards and Bards is a classy game for lads and lasses alike.");
	center_print("Cards and Bards is a two player game (Grab your partner) putting you");
	center_print("in the place of a medievel kind commanding his forces on the field of battle");
	center_print("Each card is another weapon in your arsenal to defeat your opponent and conquer!");
	center_print("-");
	
	int userInput;
	
	do
	{
		
		center_print("MAIN MENU");
		print_menu();
		cin >> userInput;
		while (cin.fail() || userInput < 0 || userInput > 4)
		{
			cout << "Invalid Input. Please choose from following options:" << endl;
			print_menu();
			cin.clear();
			cin.ignore(256, '\n');
			
			cin >>userInput;
		}
		
		//if user wants to play a game
		if (userInput == 1)
		{
			Board *myBoard = new Board();
	
			int p1Score = 0;
			int p2Score = 0;
	
			do
			{
				myBoard->playRound(p1Score, p2Score);
				cout << "Player 1 has won " << p1Score << " rounds" << endl;
				cout << "Player 2 has won " << p2Score << " rounds" << endl;
		
			} while (p1Score != 2 && p2Score != 2);
			
			//Awards game points to winner of game
			if (p1Score == 2)
			{
				++p1GamePoints;
			}
			if (p2Score == 2)
			{
				++p2GamePoints;	
			}	
		}
		
		//Help File
		else if (userInput == 2)
		{
			Help::help_menu();
		}
		
		//Game Score
		else if (userInput == 3)
		{
			cout << "\033[2J\033[1;1H" << endl;
			cout << "Player 1 has won " << p1GamePoints << endl;
			cout << "Player 2 has won " << p2GamePoints << endl;
		}
		
	} while (userInput != 4);
	
	return 0;
}

/*
int main()
{	
	string yesOrNo = "";
	bool play = true;
	while(play){
	cout << "\033[2J\033[1;1H"; //Magic terminal eraser
	Board *myBoard = new Board();
	
	int p1Score = 0;
	int p2Score = 0;
	
	do
	{
		
		myBoard->playRound(p1Score, p2Score);
		
	} while (p1Score != 2 && p2Score != 2);
	
	if (p1Score > p2Score)
	{
		cout <<"Player One is the winner!!" << endl;
	}
	else if (p1Score < p2Score)
	{
		cout <<"Player Two is the winner!!" << endl;
	}
	else
	{
		cout <<"The game is a draw."<<endl;
	}
	cout << "Play again?\n" << "y/n" << endl;
	
	
	cin >> yesOrNo;
	
	while ((yesOrNo != "y" && yesOrNo != "Y" && yesOrNo != "N" && yesOrNo != "n") || cin.fail() )
	{
		cout << "please enter y for yes or n for no" << endl;
		cin.clear();
		cin.ignore(256,'\n');
		cin >> yesOrNo;
} 
	if (yesOrNo == "n" || yesOrNo == "N")
		{
			play = false;
		}
	
	
}
	
	
		
	return 0;
}
* */
