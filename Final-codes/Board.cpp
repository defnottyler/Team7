#include "Board.h"

/*
 * Constructor initializes all class objects/values.
 * This includes loading the decks from text files
 * and creating player hands.
 */
Board::Board()
{
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
	handGenerator();
}

/*
 * Reads the text files containing the cards to be used in the game for
 * each player. The decks do not need to be identical in contents, but
 * they must be identical in size. Also note that the input from the text
 * files is not considered user input and not validated. If there is something
 * wrong in the text files, the program will not run. For information on on the
 * format of cards in the text file, see card_example.txt
 */
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
		if (line.substr(0, 4).compare("true") == 0) //Unit Card
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
			else
				hero = false;
			//cout<<name<<" "<<line.substr(index, 1)<<" "<<hero<<endl;
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
		else //Special Card
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
	d_one.close();
}

/*
 * Called in the constructor to randomly assign 10 cards from the decks to 
 * the players' hands at the beginning of the match. 
 */
void Board::handGenerator()			
{
	srand(time(NULL));

	int deckSize1 = playerOneDeck.size();
	int deckSize2 = playerTwoDeck.size();

	int index1;
	int index2;
	for (int i = 0; i<10; i++) {
		index1 = (int)(rand() % deckSize1);
		index2 = (int)(rand() % deckSize2);
		playerOneHand.push_back(playerOneDeck.at(index1));
		playerOneDeck.erase(playerOneDeck.begin() + index1);
		deckSize1--;
		playerTwoHand.push_back(playerTwoDeck.at(index2));
		playerTwoDeck.erase(playerTwoDeck.begin() + index2);
		deckSize2--;
	}
	
}

/*
 * Decides which player will go first. This can be 
 * equated to a coin toss. 
 */
bool Board::chooseTurn()
{
	srand(time(NULL));
	firstTurnChoice = (rand() % 2) + 1;
    if (firstTurnChoice == 1)
    {
		cout <<"Player One has won the coin toss and will go first.\n";
		sleep(1);
        return true;
	}
    else
    {
		cout <<"Player Two has won the coin toss and will go first.\n";
		sleep(1);
        return false;
	}
}

/*
 * Prints a prompt for the player
 */
void Board::displayTurnOptions(int playerTurn)
{
	
	cout << "Player " << playerTurn << " turn" << endl;
	cout << "Choose one of the following options:" << endl;
	cout << "1) Play a card" << endl;
	cout << "2) Pass round" << endl;
	cout << "3) Display game board" << endl;
	cout << "4) Display hand" << endl;
	cout << "5) Display help" << endl;
	cout << "Player Option: ";
}

/*
 * Main game loop is more or less in this method.
 * Loops ensures that turns are given to players
 * when appropriate, considering such factors as
 * passing a round. Method also clears the board
 * when the round ends.
 */
void Board::playRound(int &p1Score, int &p2Score)
{
	int roundWinner;
	p1Pass = false;
	p2Pass = false;
    if (isFirstRound)
    {
        playerTurn = chooseTurn();
        isFirstRound = false;
	}

	do {
		if (playerTurn) {
			playerOneTurn(p1Score, p2Score);
		}
		else 
		{
			playerTwoTurn(p1Score, p2Score);
		}
		if (p1Pass && !p2Pass) 
		{
			if (playerTwoHand.size() == 0)
				p2Pass = true;
			while (!p2Pass)
				playerTwoTurn(p1Score, p2Score);
		}
		if (p2Pass && !p1Pass) 
		{
			if (playerOneHand.size() == 0)
				p1Pass = true;
			while (!p1Pass)
				playerOneTurn(p1Score, p2Score);
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
		playerTurn = true;
	}
	else if (p1TotalStrength < p2TotalStrength)
	{
		p2Score++;
		cout <<"Player two wins the round." <<endl;
		playerTurn = false;
	}
	else
	{
		cout <<"Draw\n";
		p1Score++;
		p2Score++;
		playerTurn = !playerTurn;
	}
	clearBoard();
    //round has terminated
}

/*
 * Clears the board of all cards, moving them
 * to grave piles. Also clears all active effects.
 */
void Board::clearBoard()
{
    int p1cards;
    int p2cards;
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

/*
 * Executes player one's turn
 */
void Board::playerOneTurn(int &p1, int &p2)
{
	//Auto pass if out of cards
	 if (playerOneHand.size() == 0)
	{
		p1Pass = true;
		return;
	} 
	int cardIndex;
	displayTurnOptions(1);
	cin >> turnOption;
	if (turnOption == 1) {
		cout << "Select Card to play (0 to see hand): ";
		cin >> cardIndex;
		while( cin.fail() || cardIndex < 1 || cardIndex > playerOneHand.size())
		{
			cout <<"Please choose a card from the numbered list " << endl;
			printHand(1);
			cin.clear();
			cin.ignore(256,'\n');
			cin >> cardIndex;
		}
		playCard(cardIndex - 1, playerOneHand, false);
		playerTurn = !playerTurn;
		cout << "\033[2J\033[1;1H";
	}
	else if (turnOption == 2){
		p1Pass = true;
		playerTurn = !playerTurn;
		cout << "\033[2J\033[1;1H";
	}
	else if (turnOption == 3)
		printBoard(1, p1, p2);
	else if (turnOption == 4)
		printHand(1);
	else
		Help::help_menu();
}

/*
 * Executes player two's turn
 */
void Board::playerTwoTurn(int &p1, int &p2)
{
	//Auto pass if out of cards
	if (playerTwoHand.size() == 0)
	{
		p2Pass = true;
		return;
	} 
	int cardIndex;
	displayTurnOptions(2);
	cin >> turnOption;
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
		cout << "\033[2J\033[1;1H";
	}
	else if (turnOption == 2){
		p2Pass = true;
		playerTurn = !playerTurn;
		cout << "\033[2J\033[1;1H";
	}
	else if (turnOption == 3){
		printBoard(2, p1, p2);
		
	}
	else if (turnOption == 4)
	{
		printHand(2);
	}
	else
		Help::help_menu();
}

/*
 * Plays a selected card, ensuring that the appropriate action
 * (ability, row placement, etc.) is undertaken when it is played.
 */
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
        if (!pl && ability != 2) //a pl value of false indicates player one
            playerOneRows[cardRow].add(uCard);
        else if (pl && ability != 2)
            playerTwoRows[cardRow].add(uCard);
        playerHand.erase(playerHand.begin() + index);
        
        //Switch for unit card abilities. 
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
                cout<<"Scorched!\n";
                sleep(3);
                break;
        }
        //playerHand.at(index) = NULL;
        //playerHand.erase(playerHand.begin() + index);
	}
    else //Special card detected
    {
        sCard = (SpecialCard*)playerHand.at(index);
        ability = sCard->effect;
        playerHand.erase(playerHand.begin() + index);
        if (!pl)
            playerOneGrave.push_back(sCard);
        else
            playerTwoGrave.push_back(sCard);
            
        //Switch statement for special card abilities
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
                ability4(pl);
                break;
        }
    }
    
}

/*
 * Applies 4th special ability, which is the doubling
 * of non hero row strength
 */
void Board::ability4(bool pl)
{
    int row;
    while (true)
    {
        cout <<"Which row do you want to rally (1, 2, or 3)?\n";
        cin >>row;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(256,'\n');
		continue;
	}
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

/*
 * Activates morale boost in appropriate BoardRow
 */
void Board::moraleBoost(bool pl, int row)
{
    if (!pl)
        playerOneRows[row].addMorale();
    else
        playerTwoRows[row].addMorale();
}

/*
 * Activates spy ability, placing a card in the
 * other player's row and randomly drawing two cards
 * from the player's deck to be placed in their hand.
 */
void Board::spy(bool pl)
{
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

/*
 * Activates the medic ability, resurrecting a non-hero,
 * non special card of the player's choice.
 */
void Board::medic(bool pl) 
{
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
			cin >>choice; 
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(256,'\n');
				continue;
			}
			choice--;
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

/*
 * Activates the scorch ability, destroying the highest strength
 * card(s) in the corresponding row of the other player. Heroes
 * are not considered.
 */
void Board::scorch(bool pl, int row)
{
    int maxStrength = 0;
    if (!pl)
    {
        for (UnitCard *c : playerTwoRows[row].cards)
        {
            if (c->getStrength() > maxStrength && !c->isHero)
                maxStrength = c->getStrength();
        }
        for (int i = 0; i < playerTwoRows[row].cards.size(); i++)
        {
            if (playerTwoRows[row].cards.at(i)->getStrength() == maxStrength && !playerTwoRows[row].cards.at(i)->isHero)
            {
                playerTwoGrave.push_back(playerTwoRows[row].cards.at(i));
                playerTwoRows[row].cards.erase(playerTwoRows[row].cards.begin() + i);
		i--;
            }
        }
    }
    else
    {
        for (UnitCard *c : playerOneRows[row].cards)
        {
            if (c->getStrength() > maxStrength && !c->isHero)
                maxStrength = c->getStrength();
        }
        for (int i = 0; i < playerOneRows[row].cards.size(); i++)
        {
            if (playerOneRows[row].cards.at(i)->getStrength() == maxStrength && !playerOneRows[row].cards.at(i)->isHero)
            {
                playerOneGrave.push_back(playerOneRows[row].cards.at(i));
                playerOneRows[row].cards.erase(playerOneRows[row].cards.begin() + i);
		i--;
            }
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
	      if(unit->type == 0)
              {
	         printf(" (Hero)  Close     %2d ^", c + 1);
              }
	      if(unit->type == 1)
	      {
	         printf(" (Hero)  Ranged    %2d ^", c + 1);
	      }
	      if(unit->type == 2)
	      {
		 printf(" (Hero)  Siege     %2d ^", c + 1);
	      }
        }
        else if(hand.at(c)->isUnit)
        {
	      if(unit->type == 0)
	      {
          	 printf("         Close     %2d ^", c + 1);
	      }
	      if(unit->type == 1)
	      {
          	 printf("         Ranged    %2d ^", c + 1);
	      }
	      if(unit->type == 2)
	      {
          	 printf("         Siege     %2d ^", c + 1);
	      }
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
	      int s = unit->getStrength();
          cout << "  ^ Strength: ";
          printf("%2d         ^", (int)s);
        }
        else
        {
	      int e = special->effect;
          cout << "  ^ Effect: ";
          if(e != 4)
          {
	       if(e == 0)
	       {
		   cout << "Clear        ^";
               }
               else
	       {
	           cout << "Debuff       ^";
	       }
          }
          else
          {
	       cout << "X2 Strength  ^";
          }
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
          if(a == 1)
          {
			  cout << "Morale      ^";
		  }
		  else if(a == 2)
          {
			  cout << "Spy         ^";
		  }
		  else if(a == 3)
          {
			  cout << "Medic       ^";
		  }
		  else if(a == 4)
          {
			  cout << "Scorch      ^";
		  }
		  else
		  {
			  cout << "NONE        ^";
		  } 
        }
        else
        {
	  if(special->effect != 4)
          {
	    cout << "  ^ Row: ";
	    if(special->effect == 0)
	    {
	       cout << "Board Wide      ^";
	    }
            else if(special->effect == 1)
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

