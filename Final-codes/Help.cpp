#include "Help.h"
#include <iostream>

using namespace std;
Help::Help()
{

}

//Main method
void Help::help_menu()
{
	//clears the console
	cout << "\033[2J\033[1;1H" << endl;

	int userInput;
	do
	{
		print_menu_options();
		cin >> userInput;

		//If user inputs bad data we ask them to reinput
		while (cin.fail() || userInput < 1 || userInput > 4)
		{
			cout << "Invalid Input: please select a valid option" << endl;
			print_menu_options();
			cin.clear();
			cin.ignore(256, '\n');
			
			cin >> userInput;
		}
		//At this point, we have valid menu select input from user

		//GAME RULES
		if (userInput == 1)
		{
			//clear consolve before entering 
			cout << "\033[2J\033[1;1H" << endl;
			game_rules();
		}

		//TIPS
		else if (userInput == 2)
		{
			cout << "\033[2J\033[1;1H" << endl;
			tips();
		}

		//DICTIONARY
		else if (userInput == 3)
		{
			cout << "\033[2J\033[1;1H" << endl;
			//Explanation of Unit Carda abilities
			cout << "There are 5 types of abilities Unit Cards may posess:\n";
			cout << "\tMorale Boost- +1 strength to all cards on same row\n";
			cout << "\tSpy- Card is played on opponent's field and you draw 2 cards\n";
			cout << "\tMedic- Choose a card from either player's grave to play immediataly to the\n";
			cout << "\t       field(no Heroes or Special)\n";
			cout << "\tScorch- Destroys all cards with highest strength value in opponent's \n";
			cout << "\t        corresponding row.\n" << endl;

			//Explanation of Special Card abilities
			cout << "Special Cards can be sorted into two categories:\n";
			cout << "\tRow Effects- These Special Cards apply a x2 strength modifier to one of the\n";
			cout << "\t             user's selected rows\n";
			cout << "\tBoard Effects- These Special Cards cast a negative effect to a single row on\n";
			cout << "\t               both player's field. The row is specified by the card.\n" << endl;

		}

	} while (userInput != 4);
	//Clears the console once we return to the game
	cout << "\033[2J\033[1;1H" << endl;
}

void Help::print_menu_options()
{
	string title = "HELP MENU";
	string s = title;
	center_align(s);
	cout << "1. Game Rules" << endl;
	cout << "2. Tips" << endl;
	cout << "3. Effects and Abilities Dictionary" << endl;
	cout << "4. Close Help\n" << endl;
}

void Help::game_rules()
{
	int userInput = -1;
	do
	{
		cout << "1. Game Setup and Rules For Placement" << endl;
		cout << "2. Objective" << endl;
		cout << "3. Gameplay" << endl;
		cout << "4. Back\n" << endl;

		cin >> userInput;
		while (cin.fail() || userInput < 1 || userInput > 4)
		{
			cout << "Invalid Input: please select a valid option" << endl;
			cout << "1. Game Setup and Rules For Placement" << endl;
			cout << "2. Objectives" << endl;
			cout << "3. Gameplay" << endl;
			cout << "4. Back\n" << endl;

			cin.clear();
			cin.ignore(256, '\n');

			cin >> userInput;
		}
		//At this point, user has selected a valid input
		
		//Game Setup and Rules For Placement
		if (userInput == 1)
		{
			cout << "\033[2J\033[1;1H" << endl;
			string title = "GAME SETUP";
			string s = title;
			center_align(s);

			cout << "\tEach player randomly draws 10 cards from a predeterminded deck. \n";

			cout << "Cards within the deck fall into two categories, Unit Cards and Special Cards\n";

			cout << "Unit Cards are playable in positions on the board and break down into three types:\n";
			cout << "Siege, Ranged, and Close Combat, these types dictate which row of the board the card\n";
			cout << "is playable to. Some cards can have special abilities to modify the current state of\n"; 
			cout << "the game board. For more information see Effects and Abilities Dictionary.\n";

			cout << "\tHeroes are a type of Unit Card that aren't affected by effects or abilites,\n";
			cout << "they also cannot be revived from the discard pile, a special ability of some cards.\n";

			cout << "\n\tSpecial Cards, unlike Unit Cards, are not playable directly to the field.\n";
			cout << "Instead, Special Cards have an effect modifying the entire board.\n";
			cout << "Once a Special Card has been played, its effect replaces the previous effect on the\n";

			cout << "row. Once 10 cards have been dealt to each player, a player is randomly assigned to\n";
			cout << "start the first round. Some Special cards will apply modifiers to multiple. \n" << endl;

			cout << "RULES: " << endl;
			cout << "\tUnit Cards can only be played into the row that matches their type. Special Cards\n ";
			cout << "have no type and are simply played over an entire board. The game runs until a player\n";

			cout << "accumulates 2 points. A point is awarded at the end of each round. If two players\n";
			cout << "tie a round, both players recieve a point. Cards in your hand are  not refreshed\n";

			cout << "between rounds, so conserve them. The player to start each round is the current score leader.\n";
			cout << "\tIf players are tied, the player who has started least recently begins the round\n";

			cout << "Turns in a round exchange until a player elects to pass on their turn, playing no cards\n";
			cout << "in the remainder of the round. At this point, the other player may play as many cards as they like before\n";
			cout << "electing to pass. After this the strengths of each player's field will be evaluated\n";
			cout << "(discussed in Objective) and a winner of the round is declared.\n" << endl;
		}
		//Objectives
		else if (userInput == 2)
		{
			cout << "\033[2J\033[1;1H" << endl;
			string title = "OBJECTIVES";
			string s = title;
			center_align(s);

			cout << "\tAs you accumulate Unit Cards on your field, each row is assigned a strength equal\n";
			cout << "to the sum of strengths across all units in that row. Keep in mind that this sume may be\n";
			cout << "affected by active effects on the row put in place by Special Cards The goal is to have the\n";
			cout << "largest total stregth, which is the sum of strengths across all rows of your field.\n";

			cout << "\tAs mentioned earlier, the player's hand is not refreshed between rounds. Therefore,\n";
			cout << " it is important that a player conserves their cards in their strategy. Once a player passes in\n";

			cout << "a round, the other player is faced with the decision of  placing enough cards to put them in\n";
			cout << "position to win the game, or accepting a loss on the round and saving their cards for an\n";
			cout << "advantage in future rounds of the game.\n" << endl;;

		}
		//Gameplay
		else if (userInput == 3)
		{
			cout << "\033[2J\033[1;1H" << endl;
			string title = "GAMEPLAY";
			string s = title;
			center_align(s);

			cout << "\tA player is randomly assigned to start. At each turn, the player has the option\n";
			cout << " to play one card or pass, implying that the player believes their current positioning of\n ";
			cout << "units on the field is sufficient to win the game. If pass, the other player may play as many\n";
			cout << "cards as they wish before passing signaling the end of the round. Total Strength of each\n";
			cout << "player's field is then evaluated and a point is awarded to the player with the highest total strength\n";
			cout <<  "\tIf strengths are tied, both players recieve a point. All cards are then cleared\n";

			cout << "from the board and sent to the Grave.The player to start the next round is the current score\n ";
			cout << "leader. If players are tied, the player who has started least recently begins the round.\n";

			cout << "Rounds continue until a player accumulates 2 points, winning the game. Tie games are possible.\n" << endl;
		}
	} while (userInput != 4);
	//User has opted to return to Help main menu
	cout << "\033[2J\033[1;1H" << endl;
}

void Help::tips()
{
	string title = "TIPS";
	string s = title;
	center_align(s);

	//Explanation of Unit Carda abilities
	cout << "There are 4 types of abilities Unit Cards may posess:\n";
	cout << "\tMorale Boost- +1 strength to all cards on same row\n";
	cout << "\tSpy- Card is played on opponent's field and you draw 2 cards\n";
	cout << "\tMedic- Choose a card from your  grave to play immediataly to the\n";
	cout << "\t       field(no Heroes or Special)\n";
	cout << "\tScorch- Destroys card(s) with highest strength value in opponent's \n";
	cout << "\t        corresponding row.\n" << endl;

	//Explanation of Special Card abilities
	cout << "Special Cards can be sorted into two categories:\n";
	cout << "\tRow Effects- These Special Cards apply a x2 strength modifier to one of the\n";
	cout << "\t             user's selected rows\n";
	cout << "\tBoard Effects- These Special Cards cast a negative effect to a single row on\n";
	cout << "\t               both player's field. The row is specified by the card.\n" << endl;

	//Tips on Unit and Spec Cards

	//Combine abilities
	cout << "Unit card abilties can often synergize together for more powerful effects. For\n";
	cout << "instance, a medic can bring back a medic which can, in turn, bring back another\n";
	cout << "medic (provided there are more in the Grave). This chaining effect can greatly\n";
	cout << "augment the strength of a player's field, useful to make a large comeback to win\n";
	cout << " a game. Remember, Heroes or Special Cards may not be revived.\n" << endl;

	//First Round strategy
	cout << "In the first round, both players should amplify their focus on conserving cards in\n";
	cout << "their hand. Your strategy should aim at making your opponent play as many cards as\n";
	cout << "possible. your strategy hinges on saving as many cards. If, in the first round,\n";
	cout << "you're in a situation where you must play many cards to win the round, you're best\n";
	cout << "off to pass and save cards for later rounds (in most situations).Sometimes it is\n";
	cout << "even advantageous to throw the first round, wasting your opponent's cards\n" << endl;
}


void Help::center_align(string s)
{
	int l = s.length();
	int pos = (int)((80 - l) / 2);

	for (int i = 0; i < pos; ++i)
	{
		cout << " ";
	}
	cout << s << endl;
}
