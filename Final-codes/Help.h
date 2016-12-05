#pragma once
#include <iostream>
#include <string>

using namespace std;
//Class acts as static; therefore, these rules cannot be tied to any instance of Board (a change in rules for example)
//Provides ingame help. 
class Help 
{
public:
	//Accessed during any player's turn
	static void help_menu();
private:
	Help();
	static void print_menu_options();
	static void game_rules();
	static void tips();
	static void center_align(string s); 
};
