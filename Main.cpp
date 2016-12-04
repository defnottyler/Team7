#include <iostream>
#include "Board.h"
#include "Cards.h"

using namespace std;

//prints formatted welcoming text
void center_print(string s, int formatOption)
{
	//formatOption =0 : space padding
	//formatOption = 1 : "-" padding
	//there are 130 horizontal character on the full screen
	if (s.length() > 130)
	{
		cout << "error [" << s[0] << s[1] << s[2] << s[3] << s[4] << s[4] << "...] too large to center" << endl;
	}
	
	int I;
	int CDMSIZE = 130;
	//I is the start index of the center-justified string
	I = (int)(130 - s.length())/2;
	
	for (int i=0; i < CMDSIZE; ++i)
	{
		if (i >= I && i < I + s.length())
		{
			cout << s[I-i];
		}
		else
		{
			if (formatOption == 0)
			{
				cout << " ";	
			}
			else if (formatOption == 1)
			{
				cout << "-";	
			}
		}
	}
	cout << "\n";
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
	int gamePointKeeper();
	
	center_print("WELCOME TO CARDS AND BARDS", 1);
	center_print("Cards and Bards is a classy game for lads and lasses alike.", 0);
	center_print("Cards and Bards is a two player game (Grab your partner) putting you in the place of a medievel king commanding his forces", 0);
	center_print("on the field of battle", 0);
	center_print("Each card is another weapon in your arsenal to defeat your opponent and conquer!", 0);
	center_print("-", 1);
	
	int userInput;
	
	do
	{
		
	center_print("MAIN MENU",0);
	print_menu();
	
	
	Board *myBoard = new Board();
	
	int p1Score = 0;
	int p2Score = 0;
	
	do
	{
		myBoard->playRound(p1Score, p2Score);
		cout << p1Score <<endl;
		cout << p2Score <<endl;
		
	} while (p1Score != 2 && p2Score != 2);
	
	while (userInput != 4);
	return 0;
}
