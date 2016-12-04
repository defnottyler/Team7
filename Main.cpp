#include <iostream>
#include "Board.h"
#include "Cards.h"

using namespace std;

int main()
{
	Board *myBoard = new Board();
	
	int p1Score = 0;
	int p2Score = 0;
	
	do
	{
		myBoard->playRound(p1Score, p2Score);
		cout << p1Score <<endl;
		cout << p2Score <<endl;
		
	} while (p1Score != 2 && p2Score != 2);
	
		
	return 0;
}
