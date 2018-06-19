// Better chess
// Author: Luke Thomas
// Date: 3/8/16
// NOTE: giant knight at end by "jgs"
// and title pieces by "Alefith"

#include <iostream>
#include "chess.h"
using namespace std;

int main()
{
    char filename[128];
    chessGame G1;

    G1.printTitle();
    cout << "\nPlease enter a game file: ";
    cin >> filename;
    cout << "\nEnter [1] for Single Player Mode or [2] for Two Player Mode: ";
    while (G1.mode != 1 && G1.mode != 2)
    {
	   cin >> G1.mode;
	   if (G1.mode != 1 && G1.mode != 2)
		  cout << "Not a valid input. Please enter either [1] or [2] players: ";
    }
    if (G1.loadGame(filename)) // if game is successfully opened
	   return G1.playGame(G1.mode);
    else // if game failed to open
    {
	   cout << "Unable to load game.\n";
	   return -1;
    }
}