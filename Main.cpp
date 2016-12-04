#include <iostream>
#include "Board.h"
#include "Cards.h"

using namespace std;

int main()
{	
	string yesOrNo = "j";
	bool play = true;
	while(play){
	cout << "\033[2J\033[1;1H"; //Magic terminal eraser
	Board *myBoard = new Board();
	
	int p1Score = 0;
	int p2Score = 0;
	
	do
	{
		
		myBoard->playRound(p1Score, p2Score);
		cout << p1Score <<endl;
		cout << p2Score <<endl;
		
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
	//yesOrNo = toLowerCase(yesOrNo));
	
	/*while (yesOrNo != 'y' || yesOrNo != 'n' || cin.fail() )
	{
		cout << yesOrNo << "\n";
		cout << "please enter y for yes or n for no" << endl;
		cin.clear();
		cin.ignore(256,'\n');
		cin >> yesOrNo;
		putchar (tolower(yesOrNo));
	} */
	if (yesOrNo == "n" || yesOrNo == "N")
		{
			play = false;
		}
	
	
}
	
	
		
	return 0;
}
