#include "Board.h"
#include "Cards.h"

int main() {

	Board *board = new Board();
	
	int p1Score = 0;
	int p2Score = 0;

	do {
		board->playRound(&p1Score, &p2Score);
	} while (p1Score != 2 || p2Score != 2);

	return 0;
}
