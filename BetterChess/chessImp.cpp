#include "chess.h"
#include <fstream>
#include <iostream>
#include <windows.h>
using namespace std;

bool chessGame::loadGame(char *agamefilename)
{
    ifstream agamefile;
    agamefile.open(agamefilename);
    if (agamefile.good()) // if file opened successfully
    {
	   for (int row = 0; row < 8; row++) // 8 rows
		  for (int col = 0; col < 8; col++) // 8 columns
			 agamefile >> agamesnapshot[row][col];
	   displayGame(); // display board to the screen
	   calcMaterials(0); // sum of calculated values of pieces left on board
	   return true;
    }
    return false;
}

bool chessGame::saveGame()
{
    int fileNum = 1;

    ofstream agamefile;
    cout << "Which save file would you like to save to?[1 - 3] ";
    cin >> fileNum;
    switch (fileNum)
    {
    case 1:
	   agamefile.open("savedgame1.txt");
	   break;
    case 2:
	   agamefile.open("savedgame2.txt");
	   break;
    case 3:
	   agamefile.open("savedgame3.txt");
	   break;
    default:
	   cout << "Not a valid save file. Saving in savedgame1.txt instead.\n";
	   agamefile.open("savedgame1.txt");
    }
    for (int row = 0; row < 8; row++) // 8 rows
    {
	   for (int col = 0; col < 8; col++) // 8 columns
		  agamefile << agamesnapshot[row][col] << "\t";
	   agamefile << endl;
    }
    cout << "Game Saved!\n";
    return true;
}

void chessGame::displayGame() // scan agamesnapshot
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int r = 8;
    char alph[8] = { 'a','b','c','d','e','f','g','h' };
    int l = 0;

    SetConsoleTextAttribute(hConsole, 143);
    cout << "\nCaptured Pieces: ";
    for (int i = 0; i < sizeof(capturedW); i++)
    {
	   cout << capturedW[i]; //translate(capturedW[i]);
	   SetConsoleTextAttribute(hConsole, 143);
	   cout << " ";
    }
    cout << endl;
    SetConsoleTextAttribute(hConsole, 7);
    cout << " ";
    for (int col = 0; col < 8; col++)
	   cout << alph[l++] << " ";
    cout << endl;
    for (int row = 0; row < 8; row++) // 8 rows
    {
	   SetConsoleTextAttribute(hConsole, 7);
	   cout << r;
	   for (int col = 0; col < 7; col++) // 7 + 1 columns
	   {
		  translate(agamesnapshot[row][col]);
		  SetConsoleTextAttribute(hConsole, 143);
	   }
	   translate(agamesnapshot[row][7]);
	   SetConsoleTextAttribute(hConsole, 7);
	   cout << r << endl;
	   square++;
	   r--;
    }
    SetConsoleTextAttribute(hConsole, 7);
    cout << " ";
    l = 0;
    for (int col = 0; col < 8; col++)
	   cout << alph[l++] << " ";
    SetConsoleTextAttribute(hConsole, 128);
    cout << "\nCaptured Pieces: ";
    for (int i = 0; i < sizeof(capturedB); i++)
    {
	   cout << capturedB[i]; //translate(capturedB[i]);
	   SetConsoleTextAttribute(hConsole, 128);
	   cout << " ";
    }
    SetConsoleTextAttribute(hConsole, 7);
    cout << "\n\n";
}

void chessGame::translate(int piece)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (piece)
    {
    case 1:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 143);
	   else
		  SetConsoleTextAttribute(hConsole, 207);
	   cout << "WP"; // light pawn
	   break;
    case -1:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "BP"; // dark pawn
	   break;
    case 2:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 143);
	   else
		  SetConsoleTextAttribute(hConsole, 207);
	   cout << "WR"; // light rook
	   break;
    case -2:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "BR"; // dark rook
	   break;
    case 3:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 143);
	   else
		  SetConsoleTextAttribute(hConsole, 207);
	   cout << "Kn"; // light knight
	   break;
    case -3:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "Kn"; // batman
	   break;
    case 4:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 143);
	   else
		  SetConsoleTextAttribute(hConsole, 207);
	   cout << "WB"; // light bishop
	   break;
    case -4:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "BB"; // dark bishop
	   break;
    case 5:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 143);
	   else
		  SetConsoleTextAttribute(hConsole, 207);
	   cout << "WQ"; // light queen
	   break;
    case -5:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "BQ";  // dark queen
	   break;
    case 6:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 143);
	   else
		  SetConsoleTextAttribute(hConsole, 207);
	   cout << "WK"; // light king
	   break;
    case -6:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "BK"; // dark king
	   break;
    default:
	   if (square % 2 != 0)
		  SetConsoleTextAttribute(hConsole, 128);
	   else
		  SetConsoleTextAttribute(hConsole, 192);
	   cout << "  "; // empty 
	   break;
    }
    square++;
    SetConsoleTextAttribute(hConsole, 7); // standard colors
}

void chessGame::capturedPiece(int target)
{
    switch (target)
    {
    case 1:
	   cout << "Captured Pawn\n";
	   capturedW[w++] = 80;
	   break;
    case -1:
	   cout << "Captured Pawn\n";
	   capturedB[b++] = 80;
	   break;
    case 2:
	   cout << "Captured Rook\n";
	   capturedW[w++] = 82;
	   break;
    case -2:
	   cout << "Captured Rook\n";
	   capturedB[b++] = 82;
	   break;
    case 3:
	   cout << "Captured Knight\n";
	   capturedW[w++] = 78;
	   break;
    case -3:
	   cout << "Captured Knight\n";
	   capturedB[b++] = 78;
	   break;
    case 4:
	   cout << "Captured Bishop\n";
	   capturedW[w++] = 66;
	   break;
    case -4:
	   cout << "Captured Bishop\n";
	   capturedB[b++] = 66;
	   break;
    case 5:
	   cout << "Captured Queen\n ";
	   capturedW[w++] = 81;
	   break;
    case -5:
	   cout << "Captured Queen\n ";
	   capturedB[b++] = 81;
	   break;
    }
}

void chessGame::calcMaterials(int c)
{
    lighttotalmaterials = 0, darktotalmaterials = 0;
    for (int row = 0; row < 8; row++)
	   for (int col = 0; col < 8; col++)
		  switch (agamesnapshot[row][col])
		  {
		  case 1:
			 lighttotalmaterials += 1; // light pawn
			 break;
		  case -1:
			 darktotalmaterials += 1; // dark pawn
			 break;
		  case 2:
			 lighttotalmaterials += 5; // light rook
			 break;
		  case -2:
			 darktotalmaterials += 5; // dark rook
			 break;
		  case 3:
			 lighttotalmaterials += 3; // light knight
			 break;
		  case -3:
			 darktotalmaterials += 3; // batman
			 break;
		  case 4:
			 lighttotalmaterials += 3; // light bishop
			 break;
		  case -4:
			 darktotalmaterials += 3; // dark bishop
			 break;
		  case 5:
			 lighttotalmaterials += 9; // light queen
			 break;
		  case -5:
			 darktotalmaterials += 9;  // dark queen
			 break;
		  }
    if (c == 0)
	   cout << "Total white value: " << lighttotalmaterials << "\t"
	   << "Total black value: " << darktotalmaterials << endl;
}

int chessGame::makeMove()
{
    int fromrow, fromcol, row, col, completedmoves = 0;
    char fltrcol, tltrcol;

    if (isStalemate())
    {
	   cout << "Stalemate!\n";
	   return 1;
    }

    if (isCheckMate())
    {
	   cout << "Checkmate!\n";
	   return 1;
    }

    while (completedmoves == 0)
    {
	   int check = 0;

	   if (kingInCheck())
		  check++;

	   cout << "\nWhich piece would you like to move? ";
	   cin >> fltrcol >> fromrow;

	   if (fltrcol == 's' && fromrow == 9)
		  return saveGame();

	   fromcol = (int)fltrcol - 97; // convert letter col to number
	   fromrow = 8 - fromrow; // convert row

	   int threat = agamesnapshot[fromrow][fromcol];
	   cout << endl;
	   if (threat != 0) // if square is not empty
	   {
		  if (fromrow >= 0 && fromrow <= 7) // if user inputs valid row
		  {
			 if (fromcol >= 0 && fromcol <= 7) // if user inputs valid column
			 {
				if ((player % 2 != 0 && threat > 0) || (player % 2 == 0 && threat < 0))
				{
				    cout << "Where would you like to move this piece? ";
				    cin >> tltrcol >> row;
				    col = (int)tltrcol - 97;
				    row = 8 - row;
				    int target = agamesnapshot[row][col];
				    cout << endl;
				    if (row >= 0 && row <= 7)  // if user inputs valid row
				    {
					   if (col >= 0 && col <= 7) // if user inputs valid column
					   {
						  if (fromrow != row || fromcol != col)
						  {
							 switch (threat)
							 {
							 case 1: // light pawn
							 case -1: // dark pawn
								completedmoves = pawnMove(row, col, fromrow, fromcol, target, threat);
								promotion(row, col, fromrow, fromcol, completedmoves, threat);
								break;
							 case 2: // light rook
							 case -2: // dark rook
								completedmoves = rookMove(row, col, fromrow, fromcol, target, threat);
								if (completedmoves == 1)
								{
								    if (player % 2 != 0)
								    {
									   if (fromrow == 7 && fromcol == 0)
										  wRookMoved1++;
									   else if (fromrow == 7 && fromcol == 7)
										  wRookMoved2++;
								    }
								    else
								    {
									   if (fromrow == 0 && fromcol == 0)
										  bRookMoved1++;
									   else if (fromrow == 0 && fromcol == 7)
										  bRookMoved2++;
								    }
								}
								break;
							 case 3: // light knight
							 case -3: // dark knight
								completedmoves = knightMove(row, col, fromrow, fromcol, target, threat);
								break;
							 case 4: // light bishop
							 case -4: // dark bishop
								completedmoves = bishopMove(row, col, fromrow, fromcol, target, threat);
								break;
							 case 5: // light queen
							 case -5: // dark queen
								completedmoves = queenMove(row, col, fromrow, fromcol, target, threat);
								break;
							 case 6: // light king
							 case -6: // dark king
								completedmoves = kingMove(row, col, fromrow, fromcol, target, threat);
								if (completedmoves == 1)
								{
								    if (player % 2 != 0)
									   wKingMoved++;
								    else
									   bKingMoved++;
								}
								break;
							 }
							 if (completedmoves == 1)
							 {
								agamesnapshot[row][col] = threat; // new row and col have piece from old row and col
								agamesnapshot[fromrow][fromcol] = 0; // old row and col no longer have piece
								bool inCheck = kingInCheck();
								if (inCheck && check == 0 && ((targetK > 0 && player % 2 != 0) || (targetK < 0 && player % 2 == 0)))
								{
								    cout << "You cannot put your King in Check!\n";
								    completedmoves = 0;
								}
								else if (inCheck && check == 1 && ((targetK > 0 && player % 2 != 0) || (targetK < 0 && player % 2 == 0)))
								{
								    cout << "You cannot leave your King in Check!\n";
								    completedmoves = 0;
								}
								if (completedmoves == 1)
								{
								    if (threat * target < 0)
									   capturedPiece(target);
								    displayGame();
								    calcMaterials(0);
								    if (inCheck && completedmoves == 1 && threat % 2 != target % 2)
									   cout << "King is in Check!\n";
								}
								else
								{
								    agamesnapshot[row][col] = target;
								    agamesnapshot[fromrow][fromcol] = threat;
								}
							 }
							 if (completedmoves == 0)
								cout << "Piece cannot be placed here.\n";
						  }
						  else
							 cout << "Cannot move piece to same position it is currently in.\n";
					   }
					   else
						  cout << "Column value out of range.";
				    }
				    else
					   cout << "Row value out of range.";
				}
				else
				    cout << "Thats not your piece!";
			 }
			 else
				cout << "Column value out of range.";
		  }
		  else
			 cout << "Row value out of range.";
	   }
	   else
		  cout << "Cannot select empty square.";
    }
    return 0;
}

int chessGame::pawnMove(int row, int col, int fromrow, int fromcol, int target, int threat)
{
    if (threat > 0 && fromrow - row == 1 && fromcol == col && agamesnapshot[row][col] == 0) // for white pawn moving forward one square
	   return 1;
    else if (threat < 0 && row - fromrow == 1 && fromcol == col && agamesnapshot[row][col] == 0) // for black pawn moving forward one square
	   return 1;
    else if (threat > 0 && fromrow - row == 2 && fromcol == col && agamesnapshot[fromrow - 1][col] == 0 && agamesnapshot[row][col] == 0 && fromrow == 6) // for WP moving forward 2 squares
	   return 1;
    else if (threat < 0 && row - fromrow == 2 && fromcol == col && agamesnapshot[fromrow + 1][col] == 0 && agamesnapshot[row][col] == 0 && fromrow == 1) // for BP moving forward 2 squares
	   return 1;
    else if (threat > 0 && agamesnapshot[row][col] < 0 && fromrow - row == 1 && (fromcol - col == 1 || col - fromcol == 1)) // for WP attacking
	   return 1;
    else if (threat < 0 && agamesnapshot[row][col] > 0 && row - fromrow == 1 && (fromcol - col == 1 || col - fromcol == 1)) // for BP attacking
	   return 1;

    return 0;
}

int chessGame::knightMove(int row, int col, int fromrow, int fromcol, int target, int threat)
{
    if (threat * target <= 0 && ((row - fromrow == 2 && col - fromcol == 1) /*down+right*/ ||
	   (fromrow - row == 2 && col - fromcol == 1) /*up+right*/ || (row - fromrow == 2 && fromcol - col == 1) /*down+left*/ ||
	   (fromrow - row == 2 && fromcol - col == 1) /*up+left*/)) // cannot attack own piece and can only move in an 'L' pattern
	   return 1;

    return 0;
}

int chessGame::rookMove(int row, int col, int fromrow, int fromcol, int target, int threat)
{
    int j;

    if (threat * target <= 0 && (row == fromrow || col == fromcol))
    {
	   if (col == fromcol) // if moving up/down
	   {
		  if (row < fromrow) // if moving up
		  {
			 for (j = fromrow - 1; j > row; j--)
				if (agamesnapshot[j][col] != 0)
				    return 0;
			 return 1;
		  }
		  else // if moving down
		  {
			 for (j = fromrow + 1; j < row; j++)
				if (agamesnapshot[j][col] != 0)
				    return 0;
			 return 1;
		  }
	   }
	   else // if moving left/right
	   {
		  if (col < fromcol) // if moving left
		  {
			 for (j = fromcol - 1; j > col; j--)
				if (agamesnapshot[row][j] != 0)
				    return 0;
			 return 1;
		  }
		  else // if moving right
		  {
			 for (j = fromcol + 1; j < col; j++)
				if (agamesnapshot[row][j] != 0)
				    return 0;
			 return 1;
		  }
	   }
    }
    return 0;
}

int chessGame::bishopMove(int row, int col, int fromrow, int fromcol, int target, int threat)
{
    int i, j;

    if (threat * target <= 0 && row != fromrow && col != fromcol && abs(row - fromrow) == abs(col - fromcol))
    {
	   if (row < fromrow) // if moving up
	   {
		  if (col < fromcol) // if moving up and left
		  {
			 for (i = fromcol - 1, j = fromrow - 1; i > col, j > row; i--, j--)
				if (agamesnapshot[j][i] != 0)
				    return 0;
			 return 1;
		  }
		  else // if moving up and right
		  {
			 for (i = fromcol + 1, j = fromrow - 1; i < col, j > row; i++, j--)
				if (agamesnapshot[j][i] != 0)
				    return 0;
			 return 1;
		  }
	   }
	   else // if moving down
	   {
		  if (col < fromcol) // if moving down and left
		  {
			 for (i = fromrow + 1, j = fromcol - 1; i < row, j > col; i++, j--)
				if (agamesnapshot[i][j] != 0)
				    return 0;
			 return 1;
		  }
		  else // if moving down and right
		  {
			 for (i = fromrow + 1, j = fromcol + 1; i < row, j < col; i++, j++)
				if (agamesnapshot[i][j] != 0)
				    return 0;
			 return 1;
		  }
	   }
    }
    return 0;
}

int chessGame::queenMove(int row, int col, int fromrow, int fromcol, int target, int threat)
{
    int completedmoves = 0;

    completedmoves = rookMove(row, col, fromrow, fromcol, target, threat);
    if (completedmoves == 0)
	   completedmoves = bishopMove(row, col, fromrow, fromcol, target, threat);
    return completedmoves;
}

int chessGame::kingMove(int row, int col, int fromrow, int fromcol, int target, int threat)
{
    if (threat * target <= 0)
    {
	   if ((fromrow - row == 1 || row - fromrow == 1) && fromcol - col == 0) // moving up/down
		  return 1;
	   else if ((fromcol - col == 1 || col - fromcol == 1) && fromrow - row == 0) // moving left/right
		  return 1;
	   else if ((fromrow - row == 1 || row - fromrow == 1) && (fromcol - col == 1 || col - fromcol == 1)) // moving diagonally
		  return 1;
	   else if ((fromcol - col == 2 || col - fromcol == 2) && fromrow - row == 0) // if castling
		  if (canCastle(threat, col, fromrow, fromcol))
			 return 1;
    }
    return 0;
}

bool chessGame::isStalemate()
{
    if (lighttotalmaterials == 0 && darktotalmaterials == 0)
	   return true;
    else
	   return false;
}

bool chessGame::kingInCheck()
{
    int row, col, kingsFound = 0, target;
    bool threatened = false;

    for (row = 0; row < 8; row++)
	   for (col = 0; col < 8; col++)
		  if (agamesnapshot[row][col] == 6 || agamesnapshot[row][col] == -6)
		  {
			 kingsFound++;
			 baseRow = row; // to be used in isCheckMate()
			 baseCol = col;
			 target = agamesnapshot[row][col];
			 if (threatFromPawn(row, col, target))
				threatened = true;
			 else if (threatFromRook(row, col, target))
				threatened = true;
			 else if (threatFromKnight(row, col, target))
				threatened = true;
			 else if (threatFromBishop(row, col, target))
				threatened = true;
			 else if (threatFromQueen(row, col, target))
				threatened = true;
			 if (threatened)
			 {
				targetK = agamesnapshot[row][col];
				return true;
			 }
			 if (kingsFound == 2)
				return false;
		  }
    return false;
}

bool chessGame::isCheckMate()
{
    int target, threat, holder;

    if ((lighttotalmaterials == 0 || darktotalmaterials == 0) ||
	   (kingInCheck() && (lighttotalmaterials != 0 || darktotalmaterials != 0)))
	   // if king is only piece left (doesn't have to be in check) or if not only piece but in check
    {
	   threat = agamesnapshot[baseRow][baseCol];

	   if (baseRow < 7)
	   {
		  target = agamesnapshot[baseRow + 1][baseCol];
		  if (kingMove(baseRow + 1, baseCol, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow + 1][baseCol] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow + 1][baseCol] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow + 1][baseCol] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseRow > 0)
	   {
		  target = agamesnapshot[baseRow - 1][baseCol];
		  if (kingMove(baseRow - 1, baseCol, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow - 1][baseCol] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow - 1][baseCol] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow - 1][baseCol] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseCol < 7)
	   {
		  target = agamesnapshot[baseRow][baseCol + 1];
		  if (kingMove(baseRow, baseCol + 1, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow][baseCol + 1] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow][baseCol + 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow][baseCol + 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseCol > 0)
	   {
		  target = agamesnapshot[baseRow][baseCol - 1];
		  if (kingMove(baseRow, baseCol - 1, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow][baseCol - 1] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow][baseCol - 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow][baseCol - 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseRow < 7 && baseCol < 7)
	   {
		  target = agamesnapshot[baseRow + 1][baseCol + 1];
		  if (kingMove(baseRow + 1, baseCol + 1, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow + 1][baseCol + 1] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow + 1][baseCol + 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow + 1][baseCol + 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseRow < 7 && baseCol > 0)
	   {
		  target = agamesnapshot[baseRow + 1][baseCol - 1];
		  if (kingMove(baseRow + 1, baseCol - 1, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow + 1][baseCol - 1] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow + 1][baseCol - 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow + 1][baseCol - 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseRow > 0 && baseCol < 7)
	   {
		  target = agamesnapshot[baseRow - 1][baseCol + 1];
		  if (kingMove(baseRow - 1, baseCol + 1, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow - 1][baseCol + 1] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow - 1][baseCol + 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow - 1][baseCol + 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (baseRow > 0 && baseCol > 0)
	   {
		  target = agamesnapshot[baseRow - 1][baseCol - 1];
		  if (kingMove(baseRow - 1, baseCol - 1, baseRow, baseCol, target, threat) == 1)
		  {
			 agamesnapshot[baseRow - 1][baseCol - 1] = threat;
			 agamesnapshot[baseRow][baseCol] = 0;
			 if (kingInCheck())
			 {
				agamesnapshot[baseRow - 1][baseCol - 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
			 }
			 else
			 {
				agamesnapshot[baseRow - 1][baseCol - 1] = target;
				agamesnapshot[baseRow][baseCol] = threat;
				return false;
			 }
		  }
	   }
	   if (lighttotalmaterials == 0 || darktotalmaterials == 0)
		  return true;
	   else // if there are still other pieces left to defend king
	   {	   // this is where to check all possible moves left on board to see if king can be defended.
		  for (int row = 0; row < 8; row++)
			 for (int col = 0; col < 8; col++)
				if (agamesnapshot[row][col] != threat)
				{
				    holder = agamesnapshot[row][col];
				    if (threatFromPawn(row, col, -(threat))) // -(threat) is used so that when pawnMove() is called in threatFromPawn() there
				    {								 // are no red flags raised (white piece cannot 'attack' another white piece).
					   agamesnapshot[row][col] = (threat / 6);
					   if (!kingInCheck()) // if king is no longer in check return false
					   {
						  agamesnapshot[row][col] = holder;
						  return false;
					   }
					   agamesnapshot[row][col] = holder;
				    }
				    if (threatFromRook(row, col, -(threat)))
				    {
					   agamesnapshot[row][col] = (threat / 6) * 2;
					   if (!kingInCheck())
					   {
						  agamesnapshot[row][col] = holder;
						  return false;
					   }
					   agamesnapshot[row][col] = holder;
				    }
				    if (threatFromKnight(row, col, -(threat)))
				    {
					   agamesnapshot[row][col] = (threat / 6) * 3;
					   if (!kingInCheck())
					   {
						  agamesnapshot[row][col] = holder;
						  return false;
					   }
					   agamesnapshot[row][col] = holder;
				    }
				    if (threatFromBishop(row, col, -(threat)))
				    {
					   agamesnapshot[row][col] = (threat / 6) * 4;
					   if (!kingInCheck())
					   {
						  agamesnapshot[row][col] = holder;
						  return false;
					   }
					   agamesnapshot[row][col] = holder;
				    }
				    if (threatFromQueen(row, col, -(threat)))
				    {
					   agamesnapshot[row][col] = (threat / 6) * 5;
					   if (!kingInCheck())
					   {
						  agamesnapshot[row][col] = holder;
						  return false;
					   }
					   agamesnapshot[row][col] = holder;
				    }
				}
		  return true;
	   }
    }
    return false;
}

bool chessGame::threatFromPawn(int row, int col, int target)
{
    int fromrow, fromcol, threat, pawnsFound = 0, capturedPawns = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -1)
		  capturedPawns++;
    for (int i = 0; i < 15; i++)
	   if (capturedW[i] == 1)
		  capturedPawns++;
    for (fromrow = 0; fromrow < 8; fromrow++)
	   for (fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == 1 || agamesnapshot[fromrow][fromcol] == -1)
		  {
			 pawnsFound++;
			 threat = agamesnapshot[fromrow][fromcol];
			 if (pawnMove(row, col, fromrow, fromcol, target, threat) == 1)
				return true;
			 if (pawnsFound == 16 - capturedPawns)
				return false;
		  }
    return false;
}

bool chessGame::threatFromRook(int row, int col, int target)
{
    int fromrow, fromcol, threat, rooksFound = 0, capturedRooks = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -2)
		  capturedRooks++;
    for (int i = 0; i < 15; i++)
	   if (capturedW[i] == 2)
		  capturedRooks++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == 2 || addedPieces[i] == -2)
		  plusOne++;
    for (fromrow = 0; fromrow < 8; fromrow++)
	   for (fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == 2 || agamesnapshot[fromrow][fromcol] == -2)
		  {
			 rooksFound++;
			 threat = agamesnapshot[fromrow][fromcol];
			 if (rookMove(row, col, fromrow, fromcol, target, threat) == 1)
				return true;
			 if (rooksFound == 4 - capturedRooks + plusOne)
				return false;
		  }
    return false;
}

bool chessGame::threatFromKnight(int row, int col, int target)
{
    int fromrow, fromcol, threat, knightsFound = 0, capturedKnights = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -3)
		  capturedKnights++;
    for (int i = 0; i < 15; i++)
	   if (capturedW[i] == 3)
		  capturedKnights++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == 3 || addedPieces[i] == -3)
		  plusOne++;
    for (fromrow = 0; fromrow < 8; fromrow++)
	   for (fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == 3 || agamesnapshot[fromrow][fromcol] == -3)
		  {
			 knightsFound++;
			 threat = agamesnapshot[fromrow][fromcol];
			 if (knightMove(row, col, fromrow, fromcol, target, threat) == 1)
				return true;
			 if (knightsFound == 4 - capturedKnights + plusOne)
				return false;
		  }
    return false;
}

bool chessGame::threatFromBishop(int row, int col, int target)
{
    int fromrow, fromcol, threat, bishopsFound = 0, capturedBishops = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -4)
		  capturedBishops++;
    for (int i = 0; i < 15; i++)
	   if (capturedW[i] == 4)
		  capturedBishops++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == 4 || addedPieces[i] == -4)
		  plusOne++;
    for (fromrow = 0; fromrow < 8; fromrow++)
	   for (fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == 4 || agamesnapshot[fromrow][fromcol] == -4)
		  {
			 bishopsFound++;
			 threat = agamesnapshot[fromrow][fromcol];
			 if (bishopMove(row, col, fromrow, fromcol, target, threat) == 1)
				return true;
			 if (bishopsFound == 4 - capturedBishops + plusOne)
				return false;
		  }
    return false;
}

bool chessGame::threatFromQueen(int row, int col, int target)
{
    int fromrow, fromcol, threat, queensFound = 0, capturedQueens = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -5)
		  capturedQueens++;
    for (int i = 0; i < 15; i++)
	   if (capturedW[i] == 5)
		  capturedQueens++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == 5 || addedPieces[i] == -5)
		  plusOne = 1;
    for (fromrow = 0; fromrow < 8; fromrow++)
	   for (fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == 5 || agamesnapshot[fromrow][fromcol] == -5)
		  {
			 queensFound++;
			 threat = agamesnapshot[fromrow][fromcol];
			 if (queenMove(row, col, fromrow, fromcol, target, threat) == 1)
				return true;
			 if (queensFound == 2 - capturedQueens + plusOne)
				return false;
		  }
    return false;
}

void chessGame::promotion(int row, int col, int fromrow, int fromcol, int completedmoves, int threat)
{
    int subComplete = 0, sub;

    if (((threat == 1 && row == 0) || (threat == -1 && row == 7)) && completedmoves == 1)
    {
	   if (threat == 1)
		  cout << "What would you like to trade this pawn for?[5 for Queen, 4 for Bishop, 3 for Knight, or 2 for Rook]: ";
	   else
		  cout << "What would you like to trade this pawn for?[-5 for Queen, -4 for Bishop, -3 for Knight, or -2 for Rook]: ";
	   while (subComplete == 0)
	   {
		  cin >> sub;
		  if (sub == 5 || sub == -5 || sub == 4 || sub == -4 || sub == 3 || sub == -3 || sub == 2 || sub == -2)
		  {
			 agamesnapshot[row][col] = sub;
			 agamesnapshot[fromrow][fromcol] = 0;
			 addedPieces[ap++] = sub;
			 subComplete = 1;
		  }
		  else
			 cout << "Not a valid substitution.\nPlease enter a valid option: ";
	   }
    }
    return;
}

bool chessGame::canCastle(int thisKing, int targetcol, int fromrow, int fromcol)
{
    int col, threat, target;
    threat = agamesnapshot[fromrow][fromcol];

    if (!kingInCheck() && ((thisKing > 0 && wKingMoved == 0) || (thisKing < 0 && bKingMoved == 0)))
    {
	   if (targetcol == 2 && ((thisKing > 0 && wRookMoved1 == 0) || (thisKing < 0 && bRookMoved1 == 0))) // moving left
	   {
		  for (col = fromcol - 1; col > targetcol; col--)
		  {
			 target = agamesnapshot[fromrow][col];
			 if (target != 0)
				return false;
			 if (kingMove(fromrow, col, fromrow, fromcol, target, threat))
			 {
				if (threatFromPawn(fromrow, col, threat))
				    return false;
				else if (threatFromRook(fromrow, col, threat))
				    return false;
				else if (threatFromKnight(fromrow, col, threat))
				    return false;
				else if (threatFromBishop(fromrow, col, threat))
				    return false;
				else if (threatFromQueen(fromrow, col, threat))
				    return false;
				fromcol = col;
			 }
			 if (threat > 0)
			 {
				agamesnapshot[7][3] = 2;
				agamesnapshot[7][0] = 0;
			 }
			 else
			 {
				agamesnapshot[0][3] = -2;
				agamesnapshot[0][0] = 0;
			 }
			 return true;
		  }
	   }
	   else if (targetcol == 6 && ((thisKing > 0 && wRookMoved2 == 0) || (thisKing < 0 && bRookMoved2 == 0))) // moving right
	   {
		  for (col = fromcol + 1; col < targetcol; col++)
		  {
			 target = agamesnapshot[fromrow][col];
			 if (target != 0)
				return false;
			 if (kingMove(fromrow, col, fromrow, fromcol, target, threat))
			 {
				if (threatFromPawn(fromrow, col, threat))
				    return false;
				else if (threatFromRook(fromrow, col, threat))
				    return false;
				else if (threatFromKnight(fromrow, col, threat))
				    return false;
				else if (threatFromBishop(fromrow, col, threat))
				    return false;
				else if (threatFromQueen(fromrow, col, threat))
				    return false;
				fromcol = col;
			 }
			 if (threat > 0)
			 {
				agamesnapshot[7][5] = 2;
				agamesnapshot[7][7] = 0;
			 }
			 else
			 {
				agamesnapshot[0][5] = -2;
				agamesnapshot[0][7] = 0;
			 }
			 return true;
		  }
	   }
    }
    return false;
}

void chessGame::printTitle()
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);

    cout << "                                                      .::." << endl;
    cout << "                                           _()_       _::_" << endl;
    cout << "                                 _O      _/____\\_   _/____\\_" << endl;
    cout << "          _  _  _     ^^__      / //\\    \\      /   \\      /" << endl;
    cout << "         | || || |   /  - \\_   {     }    \\____/     \\____/" << endl;
    cout << "         |_______| <|    __<    \\___/     (____)     (____)" << endl;
    cout << "   _     \\__ ___ / <|    \\      (___)      |  |       |  |" << endl;
    cout << "  (_)     |___|_|  <|     \\      |_|       |__|       |__|" << endl;
    cout << " (___)    |_|___|  <|______\\    /   \\     /    \\     /    \\" << endl;
    cout << " _|_|_    |___|_|   _|____|_   (_____)   (______)   (______)" << endl;
    cout << "(_____)  (_______) (________) (_______) (________) (________)" << endl;
    cout << "/_____\\  /_______\\ /________\\ /_______\\ /________\\ /________\\" << endl;

    SetConsoleTextAttribute(hConsole, 7);

    for (int i = 0; i < 19; i++)
	   cout << (char)176;
    cout << char(177) << char(177) << (char)178;
    SetConsoleTextAttribute(hConsole, 112);
    cout << " WELCOME TO CHESS ";
    SetConsoleTextAttribute(hConsole, 7);
    cout << (char)178 << (char)177 << (char)177;
    for (int i = 0; i < 19; i++)
	   cout << (char)176;
    cout << endl;
    for (int i = 0; i < 19; i++)
	   cout << (char)176;
    cout << char(177) << char(177) << (char)178;
    SetConsoleTextAttribute(hConsole, 112);
    cout << "  by Luke Thomas  ";
    SetConsoleTextAttribute(hConsole, 7);
    cout << (char)178 << (char)177 << (char)177;
    for (int i = 0; i < 19; i++)
	   cout << (char)176;
    cout << endl;
    cout << "\nnewgame.txt = new game";
    cout << "\nsavedgame[1 - 3].txt = saved games\n";
}

void chessGame::printIntro()
{
    cout << "\nWhen choosing a piece to move or providing a\n"
	   << "location, use a lower case letter followed by\n"
	   << "a number, separated by a space.\n";
    cout << "\nIf you would like to save your game to come\n"
	   << "back to later, type 's 9' and press [ENTER].\n";
}

void chessGame::printHorse()
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9);

    cout << "       ,....," << endl;
    cout << "     ,::::::<" << endl;
    cout << "    ,::/ ^\\\"``." << endl;
    cout << "   ,::/,  `   e`." << endl;
    cout << "  ,::; |         '." << endl;
    cout << "  ,::|  \\___,-.  c)" << endl;
    cout << "  ;::|     \\   '-'";
    SetConsoleTextAttribute(hConsole, 7);
    cout << " < Thanks For Playing!\n";
    SetConsoleTextAttribute(hConsole, 9);
    cout << "  ;::|      \\" << endl;
    cout << "  ;::|   _.=`\\" << endl;
    cout << "  `;:|.=` _.=`\\" << endl;
    cout << "    '|_.=`   __\\" << endl;
    cout << "    `\\_..==`` /" << endl;
    cout << "     .'.___.-'." << endl;
    cout << "    /          \\" << endl;
    cout << "   ('--......--')" << endl;
    cout << "   /'--......--'\\" << endl;
    cout << "   `\"--......--\"`" << endl;
}

int chessGame::playGame(int mode)
{
    int win = 0;

    printIntro();
    do {
	   cout << "Total moves made: " << movesmade << endl;
	   if (player % 2 != 0)
	   {
		  cout << "\nWhite goes.\n";
		  win = makeMove();
	   }
	   else if (player % 2 == 0 && mode == 1)
		  compMove();
	   else
	   {
		  cout << "\nBlack goes.\n";
		  win = makeMove();
	   }
	   movesmade += 1;
	   player++;
    } while (win == 0);
    printHorse();

    return 1;
}

int chessGame::compMove()
{
    // possible pawn moves
    compPawnMoves();
    // possible rook moves
    compRookMoves();
    // possible knight moves
    compKnightMoves();
    // possible bishop moves
    compBishopMoves();
    // possible queen moves
    compQueenMoves();
    // possible king moves
    compKingMoves();

    int threat = agamesnapshot[mvfr][mvfc];
    int target = agamesnapshot[mvr][mvc];
    agamesnapshot[mvr][mvc] = agamesnapshot[mvfr][mvfc];
    agamesnapshot[mvfr][mvfc] = 0;

    if (threat * target < 0)
	   capturedPiece(target);
    displayGame();
    calcMaterials(0);
    return 1;
}

int chessGame::testValofMove(int fromrow, int fromcol, int row, int col, int target, int threat)
{
    agamesnapshot[row][col] = threat;
    agamesnapshot[fromrow][fromcol] = 0;
    if (!kingInCheck() && !threatFromPawn(row, col, threat) && !threatFromRook(row, col, threat)
	   && !threatFromKnight(row, col, threat) && !threatFromBishop(row, col, threat) &&
	   !threatFromQueen(row, col, threat) && threat != -1) // does not include threat from king! what if it puts their king in check................................
    {
	   calcMaterials(1);
	   agamesnapshot[row][col] = target;
	   agamesnapshot[fromrow][fromcol] = threat;
	   return lighttotalmaterials;
    }
    else if (threat == -1 && !kingInCheck())
    {
	   calcMaterials(1);
	   agamesnapshot[row][col] = target;
	   agamesnapshot[fromrow][fromcol] = threat;
	   return lighttotalmaterials;
    }
    agamesnapshot[row][col] = target;
    agamesnapshot[fromrow][fromcol] = threat;
    return 40;
}

void chessGame::compPawnMoves() // have to think about answering to promotion()
{
    int row, col, newVal = 39, pawnsFound = 0, capturedPawns = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -1)
		  capturedPawns++;
    for (int fromrow = 0; fromrow < 7; fromrow++)
	   for (int fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == -1)
		  {
			 row = fromrow + 1;
			 col = fromcol;
			 newVal = compPawnSeq(fromrow, fromcol, row, col); // down one
			 if (newVal < grtstVal)
			 {
				grtstVal = newVal;
				mvfr = fromrow;
				mvfc = fromcol;
				mvr = row;
				mvc = col;
			 }
			 else if (newVal == grtstVal)
			 {
				grtstVal = newVal;
				mvfr = fromrow;
				mvfc = fromcol;
				mvr = row;
				mvc = col;
			 }
			 row = fromrow + 2;
			 col = fromcol;
			 newVal = compPawnSeq(fromrow, fromcol, row, col); // down two
			 if (newVal < grtstVal)
			 {
				grtstVal = newVal;
				mvfr = fromrow;
				mvfc = fromcol;
				mvr = row;
				mvc = col;
			 }
			 else if (newVal == grtstVal)
			 {
				grtstVal = newVal;
				mvfr = fromrow;
				mvfc = fromcol;
				mvr = row;
				mvc = col;
			 }
			 if (fromcol < 7)
			 {
				row = fromrow + 1;
				col = fromcol + 1;
				newVal = compPawnSeq(fromrow, fromcol, row, col); // down/right
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
				else if (newVal == grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromcol > 0)
			 {
				row = fromrow + 1;
				col = fromcol - 1;
				newVal = compPawnSeq(fromrow, fromcol, row, col); // down/left
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
				else if (newVal == grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (pawnsFound == 8 - capturedPawns)
				return;
		  }
}

int chessGame::compPawnSeq(int fromrow, int fromcol, int row, int col)
{
    int target = agamesnapshot[row][col];
    int threat = -1;

    if (pawnMove(row, col, fromrow, fromcol, target, threat) == 1)
	   return testValofMove(fromrow, fromcol, row, col, target, threat);
    return 40;
}

void chessGame::compRookMoves()
{
    int row, col, newVal = 39, rooksFound = 0, capturedRooks = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -2)
		  capturedRooks++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == -2)
		  plusOne;
    for (int fromrow = 0; fromrow < 8; fromrow++)
	   for (int fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == -2)
		  {
			 for (row = fromrow + 1; row < 8; row++) // moving down
			 {
				newVal = compRookSeq(fromrow, fromcol, row, fromcol);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = fromcol;
				}
			 }
			 for (row = fromrow - 1; row >= 0; row--) // moving up
			 {
				newVal = compRookSeq(fromrow, fromcol, row, fromcol);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = fromcol;
				}
			 }
			 for (col = fromcol + 1; col < 8; col++) // moving right
			 {
				newVal = compRookSeq(fromrow, fromcol, fromrow, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = fromrow;
				    mvc = col;
				}
			 }
			 for (col = fromcol - 1; col >= 0; col--) // moving left
			 {
				newVal = compRookSeq(fromrow, fromcol, fromrow, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = fromrow;
				    mvc = col;
				}
			 }
			 if (rooksFound == 2 - capturedRooks + plusOne)
				return;
		  }
}

int chessGame::compRookSeq(int fromrow, int fromcol, int row, int col)
{
    int target = agamesnapshot[row][col];
    int threat = -2;

    if (rookMove(row, col, fromrow, fromcol, target, threat) == 1)
	   return testValofMove(fromrow, fromcol, row, col, target, threat);
    return 40;
}

void chessGame::compKnightMoves()
{
    int row, col, newVal = 39, knightsFound = 0, capturedKnights = 0, plusOne = 0;;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -1)
		  capturedKnights++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == -3)
		  plusOne++;
    for (int fromrow = 0; fromrow < 8; fromrow++)
	   for (int fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == -3)
		  {
			 if (fromrow > 1 && fromcol < 7)
			 {
				// up/right
				row = fromrow - 2;
				col = fromcol + 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow > 1 && fromcol > 0)
			 {
				// up/left
				row = fromrow - 2;
				col = fromcol - 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow < 6 && fromcol < 7)
			 {
				// down/right
				row = fromrow + 2;
				col = fromcol + 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow < 6 && fromcol > 0)
			 {
				// down/left
				row = fromrow + 2;
				col = fromcol - 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow > 0 && fromcol > 1)
			 {
				// left/up
				row = fromrow - 1;
				col = fromcol - 2;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow < 7 && fromcol > 1)
			 {
				// left/down
				row = fromrow + 1;
				col = fromcol - 2;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow > 0 && fromcol < 6)
			 {
				// right/up
				row = fromrow - 1;
				col = fromcol + 2;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow < 7 && fromcol < 6)
			 {
				// right/down
				row = fromrow + 1;
				col = fromcol + 2;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (knightsFound == 2 - capturedKnights + plusOne)
				return;
		  }
}

int chessGame::compKnightSeq(int fromrow, int fromcol, int row, int col)
{
    int target = agamesnapshot[row][col];
    int threat = -3;

    if (knightMove(row, col, fromrow, fromcol, target, threat) == 1)
	   return testValofMove(fromrow, fromcol, row, col, target, threat);
    return 40;
}

void chessGame::compBishopMoves()
{
    int row, col, newVal = 39, bishopsFound = 0, capturedBishops = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -4)
		  capturedBishops++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == -4)
		  plusOne++;
    for (int fromrow = 0; fromrow < 8; fromrow++)
	   for (int fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == -4)
		  {
			 for (row = fromrow + 1, col = fromcol + 1; row < 8 && col < 8; row++, col++) // moving down/right
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 for (row = fromrow + 1, col = fromcol - 1; row < 8 && col >= 0; row++, col--) // moving down/left
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 for (row = fromrow - 1, col = fromcol + 1; row >= 0 && col < 8; row--, col++) // moving up/right
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 for (row = fromrow - 1, col = fromcol - 1; row >= 0 && col >= 0; row--, col--) // moving up/left
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (bishopsFound == 2 - capturedBishops + plusOne)
				return;
		  }
}

int chessGame::compBishopSeq(int fromrow, int fromcol, int row, int col)
{
    int target = agamesnapshot[row][col];
    int threat = -4;

    if (bishopMove(row, col, fromrow, fromcol, target, threat) == 1)
	   return testValofMove(fromrow, fromcol, row, col, target, threat);
    return 40;
}

void chessGame::compQueenMoves()
{
    int row, col, newVal = 39, queensFound = 0, capturedQueens = 0, plusOne = 0;

    for (int i = 0; i < 15; i++)
	   if (capturedB[i] == -1)
		  capturedQueens++;
    for (int i = 0; i < 17; i++)
	   if (addedPieces[i] == -5)
		  plusOne++;
    for (int fromrow = 0; fromrow < 8; fromrow++)
	   for (int fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == -5)
		  {
			 for (row = fromrow + 1; row < 8; row++) // moving down
			 {
				newVal = compRookSeq(fromrow, fromcol, row, fromcol);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = fromcol;
				}
			 }
			 for (row = fromrow - 1; row >= 0; row--) // moving up
			 {
				newVal = compRookSeq(fromrow, fromcol, row, fromcol);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = fromcol;
				}
			 }
			 for (col = fromcol + 1; col < 8; col++) // moving right
			 {
				newVal = compRookSeq(fromrow, fromcol, fromrow, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = fromrow;
				    mvc = col;
				}
			 }
			 for (col = fromcol - 1; col >= 0; col--) // moving left
			 {
				newVal = compRookSeq(fromrow, fromcol, fromrow, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = fromrow;
				    mvc = col;
				}
			 }
			 for (row = fromrow + 1, col = fromcol + 1; row < 8 && col < 8; row++, col++) // moving down/right
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 for (row = fromrow + 1, col = fromcol - 1; row < 8 && col >= 0; row++, col--) // moving down/left
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 for (row = fromrow - 1, col = fromcol + 1; row >= 0 && col < 8; row--, col++) // moving up/right
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 for (row = fromrow - 1, col = fromcol - 1; row >= 0 && col >= 0; row--, col--) // moving up/left
			 {
				newVal = compRookSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (queensFound == 1 - capturedQueens + plusOne)
				return;
		  }
}

int chessGame::compQueenSeq(int fromrow, int fromcol, int row, int col)
{
    int target = agamesnapshot[row][col];
    int threat = -5;

    if (queenMove(row, col, fromrow, fromcol, target, threat) == 1)
	   return testValofMove(fromrow, fromcol, row, col, target, threat);
    return 40;
}

void chessGame::compKingMoves()
{
    int row, col, newVal = 39;

    for (int fromrow = 0; fromrow < 8; fromrow++)
	   for (int fromcol = 0; fromcol < 8; fromcol++)
		  if (agamesnapshot[fromrow][fromcol] == -6)
		  {
			 if (fromrow < 7)
			 {
				row = fromrow + 1; // moving one down
				col = fromcol;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow < 7 && fromcol < 7)
			 {
				row = fromrow + 1; // moving down/right
				col = fromcol + 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow < 7 && fromcol > 0)
			 {
				row = fromrow + 1; // moving down/left
				col = fromcol - 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow > 0)
			 {
				row = fromrow - 1; // moving one up
				col = fromcol;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow > 0 && fromcol < 7)
			 {
				row = fromrow - 1; // moving up/right
				col = fromcol + 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromrow > 0 && fromcol > 0)
			 {
				row = fromrow - 1; // moving up/left
				col = fromcol - 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromcol > 0)
			 {
				row = fromrow; // moving left
				col = fromcol - 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 if (fromcol < 7)
			 {
				row = fromrow; // moving right
				col = fromcol + 1;
				newVal = compKingSeq(fromrow, fromcol, row, col);
				if (newVal < grtstVal)
				{
				    grtstVal = newVal;
				    mvfr = fromrow;
				    mvfc = fromcol;
				    mvr = row;
				    mvc = col;
				}
			 }
			 return;
		  }
}

int chessGame::compKingSeq(int fromrow, int fromcol, int row, int col)
{
    int target = agamesnapshot[row][col];
    int threat = -6;

    if (kingMove(row, col, fromrow, fromcol, target, threat) == 1)
	   return testValofMove(fromrow, fromcol, row, col, target, threat);
    return 40;
}