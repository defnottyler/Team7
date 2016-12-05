#include <iostream>
#include <vector>
#include "Cards.h"

using namespace std;

/*
 * BoardRow objects are used to represent individual rows of cards on the board.
 * this was done in order to make it easier to keep track of individual row effects
 * and the total strength of the row. This class contains several public methods to
 * be accessed by the Board class in order to pass instructions and get information.
 * This is the level at which modifiers are applied to rows. 
 */
class BoardRow
{
public:
	//Constructor
    BoardRow(); 
    
    //Vector that contains the row's cards
    vector<UnitCard*> cards; 
    
    //Set the position of the row
    void setRow(int pos);
    
    //Returns the total strength of the row with modifiers considered
    int getRowStr(); 
    
    //Adds a card and ensures that any modifiers are applied.
    void add(UnitCard *card); 
    
    //Called to deBuff cards in the row
    void deBuff();
    
    //Called to buff cards in a row
    void buff(); 
    
    //Called to boost strength of cards by 1 for each morale boost card (2 if buffed)
    void moraleBoost(); 
    
    //Clears debuffs
    void clear(); 
    
    //resets all board effects
    void reset(); 
    
    //Changes magnitude of morale effect
    void addMorale(); 
    
    //Returns whether or not row is debuffed
    bool getDeBuffed();
    
    //Returns whether or not row is buffed
	bool getBuffed();
private: 
	//position of the row
    int rowPosition; 
    
    //tracks whether or not row is buffed
    bool buffed; 
    
    //tracks whether or not row is debuffed
    bool deBuffed; 
    
    //integer that tracks magnitude of morale effect
    int morale = 0; 
    
    //keeps track of the rows entire strength
    int rowStrength; 
};
