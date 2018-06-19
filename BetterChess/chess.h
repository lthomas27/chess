#pragma once
class chessGame {
private:
    int agamesnapshot[8][8]; // 8x8 chessboard
    int movesmade = 0;
    bool saveGame(); // save state of game to file
    void displayGame(); // scan agamesnapshot
    // output aggregated materials of both sides
    void calcMaterials(int);
    int lighttotalmaterials, darktotalmaterials;

    void translate(int); // translate numerical values to piece initials
    int square = 1; // used to make board
    
    void capturedPiece(int);
    char capturedB[15]; // arrays of captured pieces
    char capturedW[15]; // "
    int w = 0; // index for capturedW[]
    int b = 0; // index for capturedB[]
    // piece movements ------------------------
    int pawnMove(int, int, int, int, int, int);
    int rookMove(int, int, int, int, int, int);
    int knightMove(int, int, int, int, int, int);
    int bishopMove(int, int, int, int, int, int);
    int queenMove(int, int, int, int, int, int);
    int kingMove(int, int, int, int, int, int);
    // see if king is threatened --------------
    bool kingInCheck();
    bool threatFromPawn(int, int, int);
    bool threatFromRook(int, int, int);
    bool threatFromKnight(int, int, int);
    bool threatFromBishop(int, int, int);
    bool threatFromQueen(int, int, int);
    int targetK = 6;
    // see if endgame -------------------------
    bool isStalemate(); // only kings are left
    bool isCheckMate(); // if someone wins
    int baseRow = 0; // kept for ref in isCheckMate()
    int baseCol = 0; // "
    // pawn that reaches its eighth rank is immediately changed
    // into the player's choice of a queen, knight, rook, or bishop
    void promotion(int, int, int, int, int, int);
    int addedPieces[16]; // subbed in pieces
    int ap = 0; // index for addedPieces
    // see if able to castle -------------------
    bool canCastle(int, int, int, int);
    int wKingMoved = 0;
    int bKingMoved = 0;
    int wRookMoved1 = 0;
    int wRookMoved2 = 0;
    int bRookMoved1 = 0;
    int bRookMoved2 = 0;
    // formerly public -------------------------
    int makeMove();
    int player = 1;
    void printIntro();
    void printHorse();
    // CPU Player ------------------------------
    int compMove();
    int testValofMove(int, int, int, int, int, int);
    int grtstVal = 39;
    // location and target square of mvp
    int mvfr; // fromrow
    int mvfc; // fromcol
    int mvr; // row
    int mvc; // col

    void compPawnMoves();
    int compPawnSeq(int, int, int, int);
    void compRookMoves();
    int compRookSeq(int, int, int, int);
    void compKnightMoves();
    int compKnightSeq(int, int, int, int);
    void compBishopMoves();
    int compBishopSeq(int, int, int, int);
    void compQueenMoves();
    int compQueenSeq(int, int, int, int);
    void compKingMoves();
    int compKingSeq(int, int, int, int);
public:
    bool loadGame(char *); // populate agamesnapshot
    int playGame(int);
    int mode = 0;
    void printTitle();
};