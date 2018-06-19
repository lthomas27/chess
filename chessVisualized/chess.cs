using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace chessVisualized
{
    class scoredmove
    {
        public int score;
        public piece bestpiece;
        public move bestmove;

        public scoredmove(int ps, piece bp, move bm)
        { score = ps; bestpiece = bp; bestmove = bm; }
    }
    class possibleMove
    {
        public List<move> listofmoves;
        public piece piece;

        public possibleMove(List<move> plist, piece p)
        { listofmoves = plist; piece = p; }
    }
    class move
    {
        public int row, col;

        public move(int prow, int pcol)
        { row = prow; col = pcol; }
    }
    class piece
    {
        public int row, col;
        public piece(int r, int c)
        {
            row = r;
            col = c;
        }
    }
    class chess
    {
        int[,] board;
        int boardheight, boardwidth;
        int sw, sh;
        Form1 f;
        piece piecetomove;
        int player;
        List<int> capturedWhitePieces;
        List<int> capturedBlackPieces;
        piece checkSrc;
        //int threatenedKing;
        int wKingMoved, wQueenRookMoved,
            wKingRookMoved, bKingMoved,
            bQueenRookMoved, bKingRookMoved;
        int whiteTotalMaterials;
        int blackTotalMaterials;
        public char promotionChoice;
        int moverow, movecol;
        public int mode;

        public void passform(Form1 pf)
        {
            f = pf;
        }
        public chess(Graphics g, int bw, int bh)
        {
            board = new int[8,8];
            boardheight = bh;
            boardwidth = bw;
            sw = bw / bw;
            sh = bh / bh;
            player = 1;
            capturedWhitePieces = new List<int>();
            capturedBlackPieces = new List<int>();
            wKingMoved = 0;
            wQueenRookMoved = 0;
            wKingRookMoved = 0;
            bKingMoved = 0;
            bQueenRookMoved = 0;
            bKingRookMoved = 0;
            calcMaterials();
        }
        public void initializeBoard()
        {
            f.Invoke(f.delCaptured, capturedWhitePieces, capturedBlackPieces);
            board[0,0] = -2;
            board[0, 1] = -3;
            board[0, 2] = -4;
            board[0, 3] = -5;
            board[0, 4] = -6;
            board[0, 5] = -4;
            board[0, 6] = -3;
            board[0, 7] = -2;
            for (int i = 0; i < 8; i++)
                board[1, i] = -1;
            for (int row = 2; row < 6; row++)
                for (int col = 0; col < 8; col++)
                    board[row, col] = 0;
            for (int i = 0; i < 8; i++)
                board[6, i] = 1;
            board[7, 0] = 2;
            board[7, 1] = 3;
            board[7, 2] = 4;
            board[7, 3] = 5;
            board[7, 4] = 6;
            board[7, 5] = 4;
            board[7, 6] = 3;
            board[7, 7] = 2;
            
            player = 1;
            wKingMoved = 0;
            wQueenRookMoved = 0;
            wKingRookMoved = 0;
            bKingMoved = 0;
            bQueenRookMoved = 0;
            bKingRookMoved = 0;
            capturedWhitePieces.Clear();
            capturedBlackPieces.Clear();
            calcMaterials();
            f.Invoke(f.updatestat, "", whiteTotalMaterials, blackTotalMaterials);
        }
        private void calcMaterials()
        {
            whiteTotalMaterials = 0;
            blackTotalMaterials = 0;

            for (int row = 0; row < 8; row++)
                for (int col = 0; col < 8; col++)
                {
                    switch (board[row, col])
                    {
                        case -1:
                            blackTotalMaterials += 1;
                            break;
                        case 1:
                            whiteTotalMaterials += 1;
                            break;
                        case -2:
                            blackTotalMaterials += 5;
                            break;
                        case 2:
                            whiteTotalMaterials += 5;
                            break;
                        case -3:
                            blackTotalMaterials += 3;
                            break;
                        case 3:
                            whiteTotalMaterials += 3;
                            break;
                        case -4:
                            blackTotalMaterials += 3;
                            break;
                        case 4:
                            whiteTotalMaterials += 3;
                            break;
                        case -5:
                            blackTotalMaterials += 9;
                            break;
                        case 5:
                            whiteTotalMaterials += 9;
                            break;
                    }
                }
        }
        public void displayBoard()
        {
            for (int row = 0; row < 8; row++)
                for (int col = 0; col < 8; col++)
                    f.Invoke(f.updatebuttons, row, col, board[row,col]);
            displayCaptured();
        }
        private void displayCaptured()
        {
            f.Invoke(f.captured, capturedWhitePieces, capturedBlackPieces);
        }
        public bool selectPiece(int row, int col)
        {
            if (row < 0 || row >= boardheight
                || col < 0 || col >= boardwidth
                || board[row, col] == 0)
                return false;
            else if (!playersPiece(row, col))
                return false;
            piecetomove = new piece(row, col);
            return true;
        }
        private bool playersPiece(int row, int col)
        {
            if (player % 2 == 0 && board[row, col] < 0 && mode == 1)
                return true;
            else if (player % 2 != 0 && board[row, col] > 0)
                return true;
            return false;
        }
        public bool humanMove(int row, int col)
        {
            bool wantstopromote = false;
            if (player % 2 != 0 && wpawnDoubleJump) wpawnDoubleJump = false;
            else if (player % 2 == 0 && bpawnDoubleJump) bpawnDoubleJump = false;
            f.Invoke(f.delCaptured, capturedWhitePieces, capturedBlackPieces);
            bool valid = false;
            moverow = row;
            movecol = col;
            if (row < 0 || row >= boardheight || col < 0 || col >= boardwidth)
                valid = false;
            else if (board[row, col] * board[piecetomove.row, piecetomove.col] > 0)
                valid = false; // attacking own piece
            else
            {
                switch (board[piecetomove.row, piecetomove.col])
                {
                    case -1:
                    case 1:
                        valid = movePawn(row, col, false);
                        if (valid && (row == 0 || row == 7))
                            wantstopromote = true;
                        break;
                    case -2:
                    case 2:
                        valid = moveRook(row, col);
                        break;
                    case -3:
                    case 3:
                        valid = moveKnight(row, col);
                        break;
                    case -4:
                    case 4:
                        valid = moveBishop(row, col);
                        break;
                    case -5:
                    case 5:
                        valid = moveQueen(row, col);
                        break;
                    case -6:
                    case 6:
                        if (Math.Abs(piecetomove.col - col) == 2)
                            valid = canCastle(row, col);
                        else valid = moveKing(row, col);
                        break;
                }
                if (valid)
                {
                    int target = board[row, col];
                    makemove(row, col); // make the move
                    if (kingCheck(false))
                        valid = false; // cannot put your king in check
                    if (!valid) undomove(row, col, target);
                    else
                    {
                        // has a piece been captured?
                        if (target < 0) capturedBlackPieces.Add(target);
                        else if (target > 0) capturedWhitePieces.Add(target);
                        // have kings or rooks moved?
                        if (piecetomove.row == 0 && piecetomove.col == 4)
                            bKingMoved = 1;
                        else if (piecetomove.row == 7 && piecetomove.col == 4)
                            wKingMoved = 1;
                        else if (piecetomove.row == 0 && piecetomove.col == 0)
                            bQueenRookMoved = 1;
                        else if (piecetomove.row == 0 && piecetomove.col == 7)
                            bKingRookMoved = 1;
                        else if (piecetomove.row == 7 && piecetomove.col == 0)
                            wQueenRookMoved = 1;
                        else if (piecetomove.row == 7 && piecetomove.col == 7)
                            wKingRookMoved = 1;
                        if (wantstopromote)
                            providePromotionOptions();
                        player++; // change turns
                        calcMaterials();
                        if (isCheckmate())
                            f.Invoke(f.updatestat, "Checkmate", whiteTotalMaterials, blackTotalMaterials);
                        else if (isStalemate())
                            f.Invoke(f.updatestat, "Stalemate", whiteTotalMaterials, blackTotalMaterials);
                        else { 
                            if (kingCheck(false))
                                f.Invoke(f.updatestat, "Check", whiteTotalMaterials, blackTotalMaterials);
                            else
                                f.Invoke(f.updatestat, "", whiteTotalMaterials, blackTotalMaterials);
                            if (mode == 0 && !wantstopromote)
                            {
                                displayBoard();
                                f.Invoke(f.delCaptured, capturedWhitePieces, capturedBlackPieces);
                                aiMove();
                            }
                        }
                    }
                }
            }            
            return valid;
        }
        int defaultlevel = 4;
        public void aiMove()
        {
            piece bp;
            move bm;
            scoredmove sm = maxchoice(defaultlevel);//, int.MinValue + 1, int.MaxValue);
            bp = new piece(sm.bestpiece.row, sm.bestpiece.col);
            bm = new move(sm.bestmove.row, sm.bestmove.col);
            if (board[bm.row, bm.col] * board[bp.row, bp.col] < 0)
            {
                if (player % 2 == 0)
                    capturedWhitePieces.Add(board[bm.row, bm.col]);
                else
                    capturedBlackPieces.Add(board[bm.row, bm.col]);
            }
            piecetomove.row = bp.row;
            piecetomove.col = bp.col;
            makemove(bm.row, bm.col);
            player++;
            calcMaterials();
            if (isCheckmate())
                f.Invoke(f.updatestat, "Checkmate", whiteTotalMaterials, blackTotalMaterials);
            else if (isStalemate())
                f.Invoke(f.updatestat, "Stalemate", whiteTotalMaterials, blackTotalMaterials);
            else if (kingCheck(false))
                f.Invoke(f.updatestat, "Check", whiteTotalMaterials, blackTotalMaterials);
            else
                f.Invoke(f.updatestat, "", whiteTotalMaterials, blackTotalMaterials);
            return;
        }
        private int AIscore()
        {
            int gamescore = 0;
            int blackTotalInCenter = 0;
            int whiteTotalInCenter = 0;
            
            // material difference?
            calcMaterials();
            gamescore += blackTotalMaterials - whiteTotalMaterials;
            // developed pieces?
            for (int row = 2; row < 6; row++)
                for (int col = 2; col < 6; col++)
                {
                    if (board[row, col] < 0)
                    {
                        if (col == 3 || col == 4)
                            blackTotalInCenter += 3;
                        else blackTotalInCenter += 2;
                    }
                    else if (board[row, col] > 0)
                    {
                        if (col == 3 || col == 4)
                            whiteTotalInCenter += 3;
                        else whiteTotalInCenter += 2;
                    }
                }
            gamescore += blackTotalInCenter;
            gamescore -= whiteTotalInCenter;

            return gamescore;
        }
        int WINSCORE = 1000;
        int LOSESCORE = -1000;
        private scoredmove maxchoice(int level) //, int alpha, int beta)  // on behalf of AI (machine)
        {
            int maxscore = int.MinValue;
            piece bestpiece = new piece(-1, -1);
            move bestmove = new move(-1, -1);

            if (level == 0) // if you reached predefined level
                return new scoredmove(AIscore(), new piece(-1, -1), new move(-1, -1));

            List<possibleMove> possiblemoves = collectMoves();
            scoredmove currentmove;
            int currentscore;
            for (int i = 0; i < possiblemoves.Count(); i++)
            {
                possibleMove lastmove = possiblemoves[i];
                piecetomove.row = lastmove.piece.row;
                piecetomove.col = lastmove.piece.col;
                for (int j = 0; j < lastmove.listofmoves.Count; j++)
                {
                    int target = board[lastmove.listofmoves[j].row, lastmove.listofmoves[j].col];
                    makemove(lastmove.listofmoves[j].row, lastmove.listofmoves[j].col); // pretend every possible move
                    if (isCheckmate())
                        currentscore = WINSCORE;
                    else
                    {
                        piece tmp = new piece(piecetomove.row, piecetomove.col);
                        currentmove = minchoice(level - 1);//, alpha, beta);
                        currentscore = currentmove.score;
                        piecetomove = tmp;
                    }
                    undomove(lastmove.listofmoves[j].row, lastmove.listofmoves[j].col, target);

                    if (currentscore > maxscore)
                    {
                        maxscore = currentscore;
                        bestpiece = lastmove.piece;
                        bestmove = lastmove.listofmoves[j];
                        /*if (currentscore > alpha)
                        {
                            alpha = currentscore;
                            if (alpha >= beta)
                                break;
                        }*/
                    }
                }
            }
            return new scoredmove(maxscore, bestpiece, bestmove);
        }
        scoredmove minchoice(int level)//, int alpha, int beta)
        {
            int minscore = int.MaxValue;
            piece bestpiece = new piece(-1, -1);
            move bestmove = new move(-1, -1);

            if (level == 0) // if you reached predefined level
                return new scoredmove(AIscore(), new piece(-1, -1), new move(-1, -1));

            List<possibleMove> possiblemoves = collectMoves();
            scoredmove currentmove;
            int currentscore;
            for (int i = 0; i < possiblemoves.Count(); i++)
            {
                possibleMove lastmove = possiblemoves[i];
                piecetomove.row = lastmove.piece.row;
                piecetomove.col = lastmove.piece.col;
                for (int j = 0; j < lastmove.listofmoves.Count; j++)
                {
                    int target = board[lastmove.listofmoves[j].row, lastmove.listofmoves[j].col];
                    makemove(lastmove.listofmoves[j].row, lastmove.listofmoves[j].col); // pretend every possible move
                    if (isCheckmate())
                        currentscore = LOSESCORE;
                    else
                    {
                        piece tmp = new piece(piecetomove.row, piecetomove.col);
                        currentmove = maxchoice(level - 1);//, alpha, beta);
                        currentscore = currentmove.score;
                        piecetomove = tmp;
                    }
                    undomove(lastmove.listofmoves[j].row, lastmove.listofmoves[j].col, target);

                    if (currentscore < minscore)
                    {
                        minscore = currentscore;
                        bestpiece = lastmove.piece;
                        bestmove = lastmove.listofmoves[j];
                        /*if (currentscore < beta)
                            beta = currentscore;
                        if (alpha >= beta)
                            break;*/
                    }
                }
            }
            return new scoredmove(minscore, bestpiece, bestmove);
        }
        private List<possibleMove> collectMoves()
        {
            List<possibleMove> possiblemoves = new List<possibleMove>();
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    if (board[row, col] < 0)
                    {
                        piecetomove.row = row;
                        piecetomove.col = col;
                        piece possiblepiece = new piece(row, col);
                        List<move> movelist = new List<move>();
                        for (int r = 0; r < 8; r++)
                        {
                            for (int c = 0; c < 8; c++)
                            {
                                bool valid = false;
                                if (board[r, c] * board[piecetomove.row, piecetomove.col] <= 0)
                                {
                                    switch (board[piecetomove.row, piecetomove.col])
                                    {
                                        case -1:
                                            valid = movePawn(r, c, true);
                                            break;
                                        case -2:
                                            valid = moveRook(r, c);
                                            break;
                                        case -3:
                                            valid = moveKnight(r, c);
                                            break;
                                        case -4:
                                            valid = moveBishop(r, c);
                                            break;
                                        case -5:
                                            valid = moveQueen(r, c);
                                            break;
                                        case -6:
                                            valid = moveKing(r, c);
                                            break;
                                    }
                                    if (valid)
                                    {
                                        int target = board[r, c];
                                        makemove(r, c); // make the move
                                        if (kingCheck(false))
                                            valid = false; // cannot put your king in check
                                        undomove(r, c, target);
                                        if (valid)
                                        {
                                            move nm = new move(r, c);
                                            movelist.Add(nm);
                                        }
                                    }
                                }
                            }
                        }
                        // add possiblemove to list
                        if (movelist.Count != 0)
                        {
                            possibleMove pm = new possibleMove(movelist, possiblepiece);
                            possiblemoves.Add(pm);
                        }
                    }
                }
            }
            return possiblemoves;
        }
        public void makemove(int row, int col)
        {
            board[row, col] = board[piecetomove.row,piecetomove.col];
            board[piecetomove.row, piecetomove.col] = 0;
        }
        private void undomove(int row, int col, int target)
        {
            board[piecetomove.row, piecetomove.col] = board[row, col];
            board[row, col] = target;
        }
        bool wpawnDoubleJump;
        bool bpawnDoubleJump;
        private bool movePawn(int row, int col, bool testingCheck)
        {
            bool valid = false;
            if ((piecetomove.row - row) * board[piecetomove.row, piecetomove.col] > 0)
            { // moving right direction
                if (Math.Abs(piecetomove.row - row) == 1 && piecetomove.col == col && // moving forward one
                    board[row, col] == 0)
                    valid = true;
                else if (Math.Abs(piecetomove.row - row) == 2 && // moving forward two
                    board[(piecetomove.row + row) / 2, col] == 0 &&
                    board[row, col] == 0 && piecetomove.col == col &&
                    ((piecetomove.row == 6 && player % 2 != 0) ||
                    (piecetomove.row == 1 && player % 2 == 0)))
                {
                    if (!testingCheck)
                    {
                        if (player % 2 != 0) wpawnDoubleJump = true;
                        else bpawnDoubleJump = true;
                    }
                    valid = true;
                }
                else if (Math.Abs(piecetomove.row - row) == 1 && // attacking
                    Math.Abs(piecetomove.col - col) == 1)
                {
                    if ((board[row, col] * board[piecetomove.row, piecetomove.col] < 0))
                        valid = true;
                    else
                    {
                        if (board[piecetomove.row, col] *
                            board[piecetomove.row, piecetomove.col] < 0 &&
                            ((player % 2 != 0 && bpawnDoubleJump == true) ||
                            (player % 2 == 0 && wpawnDoubleJump == true)) && !testingCheck)
                        { // en Passant
                            if (player % 2 != 0)
                                capturedBlackPieces.Add(-1);
                            else
                                capturedWhitePieces.Add(1);
                            board[piecetomove.row, col] = 0;
                            valid = true;
                        }
                    }
                }
            }
            return valid;
        }
        private bool moveRook(int row, int col)
        {
            bool valid = true;
            
            if (piecetomove.row == row)
            { // moving horizontally
                int offset = col - piecetomove.col;
                int range = Math.Abs(offset);

                for (int i = 1; i < range; i++)
                    if (board[piecetomove.row, piecetomove.col + i * offset / range] != 0)
                    {
                        valid = false;
                        break;
                    }
            }
            else if (piecetomove.col == col)
            { // moving vertically
                int offset = row - piecetomove.row;
                int range = Math.Abs(offset);

                for (int i = 1; i < range; i++)
                    if (board[piecetomove.row + i * offset / range, piecetomove.col] != 0)
                    {
                        valid = false;
                        break;
                    }
            }
            else valid = false;

            return valid;
        }
        private bool moveKnight(int row, int col)
        {
            bool valid = false;

            if (piecetomove.row != row && piecetomove.col != col &&
                (Math.Abs(row - piecetomove.row) + Math.Abs(col - piecetomove.col) == 3))
                valid = true;

            return valid;
        }
        private bool moveBishop(int row, int col)
        {
            bool valid = true;

            if (Math.Abs(piecetomove.row - row) == Math.Abs(piecetomove.col - col))
            {
                int offsetcol = col - piecetomove.col;
                int offsetrow = row - piecetomove.row;
                int range = Math.Abs(offsetcol);

                for (int i = 1; i < range; i++)
                    if (board[piecetomove.row + i * offsetrow / range,
                        piecetomove.col + i * offsetcol / range] != 0)
                    {
                        valid = false;
                        break;
                    }
            }
            else valid = false;

            return valid;
        }
        private bool moveQueen(int row, int col)
        {
            bool valid = false;
            
            if (piecetomove.row == row || piecetomove.col == col)
                valid = moveRook(row, col);
            else if (Math.Abs(piecetomove.row - row) == Math.Abs(piecetomove.col - col))
                valid = moveBishop(row, col);

            return valid;
        }
        private bool moveKing(int row, int col)
        {
            bool valid = false;
            
            if ((Math.Abs(piecetomove.row - row) == 1 && piecetomove.col == col)
                || (Math.Abs(piecetomove.col - col) == 1 && piecetomove.row == row)
                || (piecetomove.row != row && piecetomove.col != col
                && Math.Abs(piecetomove.row - row) + Math.Abs(piecetomove.col - col) == 2))
                valid = true;

            return valid;
        }
        private bool kingCheck(bool testingCheckMate)
        {
            piece king = new piece(-1, -1);
            piece tmp = new piece(piecetomove.row, piecetomove.col); // save piecetomove

            for (int row = 0; row < 8; row++)
            { // find king
                for (int col = 0; col < 8; col++)
                {
                    if (((board[row, col] == 6) && player % 2 != 0) ||
                        ((board[row, col] == -6) && player % 2 == 0))
                    {
                        king.row = row;
                        king.col = col;
                        break;
                    }
                }
                if (king.row != -1) break;
            }
            for (int r = 0; r < 8; r++)
            {
                for (int c = 0; c < 8; c++)
                {
                    piecetomove.row = r;
                    piecetomove.col = c;
                    if (board[king.row, king.col] * board[r, c] < 0)
                    {
                        switch (board[r, c])
                        {
                            case -1:
                            case 1:
                                if (movePawn(king.row, king.col, true))
                                {
                                    if (testingCheckMate)
                                        checkSrc = piecetomove;
                                    piecetomove = tmp; // restore piecetomove
                                    return true;
                                }
                                break;
                            case -2:
                            case 2:
                                if (moveRook(king.row, king.col))
                                {
                                    if (testingCheckMate)
                                        checkSrc = piecetomove;
                                    piecetomove = tmp; // restore piecetomove
                                    return true;
                                }
                                break;
                            case -3:
                            case 3:
                                if (moveKnight(king.row, king.col))
                                {
                                    if (testingCheckMate)
                                        checkSrc = piecetomove;
                                    piecetomove = tmp; // restore piecetomove
                                    return true;
                                }
                                break;
                            case -4:
                            case 4:
                                if (moveBishop(king.row, king.col))
                                {
                                    if (testingCheckMate)
                                        checkSrc = piecetomove;
                                    piecetomove = tmp; // restore piecetomove
                                    return true;
                                }
                                break;
                            case -5:
                            case 5:
                                if (moveQueen(king.row, king.col))
                                {
                                    if (testingCheckMate)
                                        checkSrc = piecetomove;
                                    piecetomove = tmp; // restore piecetomove
                                    return true;
                                }
                                break;
                        }
                    }
                }
            }
            piecetomove = tmp; // restore piecetomove
            return false;
        }
        private bool canCastle(int row, int col)
        {
            bool cc = false;
            if (!kingCheck(false))
            {
                if (player % 2 == 0)
                {
                    if (bKingMoved == 0)
                    {
                        if (col == 2 && bQueenRookMoved == 0 && board[row, 0] == -2)
                            cc = true;
                        else if (col == 6 && bKingRookMoved == 0 && board[row, 7] == -2)
                            cc = true;
                    }
                }
                else
                {
                    if (wKingMoved == 0)
                    {
                        if (col == 2 && wQueenRookMoved == 0 && board[row, 0] == 2)
                            cc = true;
                        else if (col == 6 && wKingRookMoved == 0 && board[row, 0] == 2)
                            cc = true;
                    }
                }
                if (cc == true)
                {
                    int offset;
                    int range;
                    if (col == 2)
                    {
                        offset = -4;
                        range = 4;
                    }
                    else
                    {
                        offset = 3;
                        range = 3;
                    }
                    for (int i = 1; i < range; i++) // see if path is clear
                        if (board[piecetomove.row, piecetomove.col + i * offset / range] != 0)
                            cc = false;
                    if (cc == true) // if path is clear
                        for (int i = 1; i < 3; i++)
                        { // see if path is threatened
                            piece trialDest = new piece(row, piecetomove.col + i * offset / range);
                            makemove(row, trialDest.col);
                            if (kingCheck(false)) cc = false;
                            undomove(row, trialDest.col, 0);
                            if (cc == false) break;
                        }
                }
                if (cc == true)
                {
                    if (row == 0 && col == 2)
                    {
                        board[0, 0] = 0;
                        board[0, 3] = -2;
                    }
                    else if (row == 0 && col == 6)
                    {
                        board[0, 7] = 0;
                        board[0, 5] = -2;
                    }
                    else if (row == 7 && col == 2)
                    {
                        board[7, 0] = 0;
                        board[7, 3] = 2;
                    }
                    else if (row == 7 && col == 6)
                    {
                        board[7, 7] = 0;
                        board[7, 5] = 2;
                    }
                }
            }
            return cc;
        }
        public bool isStalemate()
        {
            if (whiteTotalMaterials == 0 && blackTotalMaterials == 0)
                return true; // only kings left on board
            else if (whiteTotalMaterials == 0 || blackTotalMaterials == 0)
                if (insufficientMaterial())
                    return true; // only kings and one minor piece left on board
            else if (!kingCheck(false) && ((whiteTotalMaterials == 0 && player % 2 != 0)
                || (blackTotalMaterials == 0 && player % 2 == 0)))
                if (!kingCanMove())
                    return true; // lone king has no move and it's his turn
            return false;
        }
        private bool insufficientMaterial()
        {
            if (whiteTotalMaterials == 3 || blackTotalMaterials == 3)
            {
                for (int row = 0; row < 8; row++)
                    for (int col = 0; col < 8; col++)
                    {
                        if ((board[row, col] == 1 && whiteTotalMaterials == 3)
                            || (board[row, col] == -1 && blackTotalMaterials == 3))
                            return false;
                    }
                return true;
            }
            return false;
        }
        piece king;
        private bool kingCanMove()
        {
            piece dest;
            piece tmp = new piece(piecetomove.row, piecetomove.col); // save piecetomove
            int[,] offset = new int[,] {
                {1,0},
                {0,1},
                {-1,0},
                {0,-1},
                {1,1},
                {1,-1},
                {-1,1},
                {-1,-1}
            };
            bool kingFound = false;
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    if ((board[row, col] == 6 && player % 2 != 0) ||
                        (board[row, col] == -6 && player % 2 == 0))
                    {
                        piecetomove.row = row;
                        piecetomove.col = col;
                        king = new piece(row, col);
                        kingFound = true;
                        break;
                    }
                }
                if (kingFound) break;
            }
            for (int i = 0; i < 8; i++)
            {
                dest = new piece(piecetomove.row + offset[i, 0], piecetomove.col + offset[i, 1]);
                if (dest.row > -1 && dest.row < 8 && dest.col > -1 && dest.col < 8)
                {
                    if ((board[piecetomove.row, piecetomove.col] * board[dest.row, dest.col] <= 0)
                        && moveKing(dest.row, dest.col))
                    { // if king has way out
                        int target = board[dest.row, dest.col];
                        makemove(dest.row, dest.col);
                        if (!kingCheck(false))
                        {
                            undomove(dest.row, dest.col, target);
                            piecetomove = tmp; // restore piecetomove
                            return true;
                        }
                        undomove(dest.row, dest.col, target);
                    }
                }
            }
            piecetomove = tmp; // restore piecetomove
            return false;
        }
        public bool isCheckmate()
        {
            if (!kingCheck(true))
                return false;
            if (kingCanMove())
                return false;
            // get coordinates of squares in between king and attacker as well as of attacker
            int offsetInt, range;
            int offsetcol, offsetrow;
            piece[] threatArray;

            if (king.row == checkSrc.row)
            {
                offsetInt = checkSrc.col - king.col;
                range = Math.Abs(offsetInt);

                threatArray = new piece[range];

                for (int i = 0; i < range; i++)
                    threatArray[i] = new piece(king.row, king.col + (i+1) * offsetInt / range);
            }
            else if (king.col == checkSrc.col)
            {
                offsetInt = checkSrc.row - king.row;
                range = Math.Abs(offsetInt);

                threatArray = new piece[range];

                for (int i = 0; i < range; i++)
                    threatArray[i] = new piece(king.row + (i+1) * offsetInt / range, king.col);
            }
            else
            {
                offsetcol = checkSrc.col - king.col;
                offsetrow = checkSrc.row - king.row;
                range = Math.Abs(offsetcol);

                threatArray = new piece[range];

                for (int i = 0; i < range; i++)
                    threatArray[i] = new piece(king.row + (i+1) * offsetrow / range,
                        king.col + (i+1) * offsetcol / range);
            }
            piece tmp = new piece(piecetomove.row, piecetomove.col); // save piecetomove
            for (int r = 0; r < 8; r++) // see if check can be blocked by another piece
                for (int c = 0; c < 8; c++)
                {
                    if (board[r, c] * board[king.row, king.col] > 0)
                    {
                        piecetomove.row = r;
                        piecetomove.col = c;
                        for (int i = 0; i < range; i++)
                        {
                            switch (board[r, c])
                            {
                                case -1:
                                case 1:
                                    if (movePawn(threatArray[i].row, threatArray[i].col, true))
                                    {
                                        piecetomove = tmp; // restore piecetomove
                                        return false;
                                    }
                                    break;
                                case -2:
                                case 2:
                                    if (moveRook(threatArray[i].row, threatArray[i].col))
                                    {
                                        piecetomove = tmp; // restore piecetomove
                                        return false;
                                    }
                                    break;
                                case -3:
                                case 3:
                                    if (moveKnight(threatArray[i].row, threatArray[i].col))
                                    {
                                        piecetomove = tmp; // restore piecetomove
                                        return false;
                                    }
                                    break;
                                case -4:
                                case 4:
                                    if (moveBishop(threatArray[i].row, threatArray[i].col))
                                    {
                                        piecetomove = tmp; // restore piecetomove
                                        return false;
                                    }
                                    break;
                                case -5:
                                case 5:
                                    if (moveQueen(threatArray[i].row, threatArray[i].col))
                                    {
                                        piecetomove = tmp; // restore piecetomove
                                        return false;
                                    }
                                    break;
                            }
                        }
                    }
                }
            piecetomove = tmp; // restore piecetomove
            return true;
        }
        private void providePromotionOptions()
        {
            f.Invoke(f.promote); // add buttons
        } 
        public void promotePawn()
        {
            player--;
            int row = moverow;
            int col = movecol;
            switch (promotionChoice)
            {
                case 'Q':
                    if (player % 2 == 0) board[row, col] = -5;
                    else board[row, col] = 5;
                    break;
                case 'B':
                    if (player % 2 == 0) board[row, col] = -4;
                    else board[row, col] = 4;
                    break;
                case 'K':
                    if (player % 2 == 0) board[row, col] = -3;
                    else board[row, col] = 3;
                    break;
                case 'R':
                    if (player % 2 == 0) board[row, col] = -2;
                    else board[row, col] = 2;
                    break;
            }
            f.Invoke(f.promoteDelete); // remove buttons

            // take piece out of captured array if in it and replace with pawn
            if (player % 2 == 0)
            {
                if (capturedBlackPieces.Contains(board[row, col]))
                    capturedBlackPieces.Remove(board[row, col]);
                capturedBlackPieces.Add(-1);
            }
            else
            {
                if (capturedWhitePieces.Contains(board[row, col]))
                    capturedWhitePieces.Remove(board[row, col]);
                capturedWhitePieces.Add(1);
            }
            player++;
            calcMaterials();
            if (isCheckmate())
                f.Invoke(f.updatestat, "Checkmate", whiteTotalMaterials, blackTotalMaterials);
            else if (isStalemate())
                f.Invoke(f.updatestat, "Stalemate", whiteTotalMaterials, blackTotalMaterials);
            else {
                if (kingCheck(false))
                    f.Invoke(f.updatestat, "Check", whiteTotalMaterials, blackTotalMaterials);
                else
                    f.Invoke(f.updatestat, "", whiteTotalMaterials, blackTotalMaterials);
                if (mode == 0)
                {
                    displayBoard();
                    f.Invoke(f.delCaptured, capturedWhitePieces, capturedBlackPieces);
                    aiMove();
                }
            }
            return;
        }
        public void loadGame()
        {
            capturedWhitePieces.Clear();
            capturedBlackPieces.Clear();
            string fileContent = File.ReadAllText(saveFile);
            string[] integerStrings = fileContent.Split(new char[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
            int[] integers = new int[integerStrings.Length];
            for (int n = 0; n < integerStrings.Length; n++)
                integers[n] = int.Parse(integerStrings[n]);
            player = integers[0];
            int i = 1;
            for (int row = 0; row < 8; row++)
                for (int col = 0; col < 8; col++, i++)
                    board[row, col] = integers[i];
            // read in captured pieces
            for (; integers[i] != -9; i++)
                capturedWhitePieces.Add(integers[i]);
            for (i++; integers[i] != -9; i++)
                capturedBlackPieces.Add(integers[i]);
            // read in flags for castling
            i++;
            wKingMoved = integers[i++];
            bKingMoved = integers[i++];
            wKingRookMoved = integers[i++];
            wQueenRookMoved = integers[i++];
            bKingRookMoved = integers[i++];
            bQueenRookMoved = integers[i++];
            calcMaterials();
            piecetomove = new piece(-1, -1);
            if (isCheckmate())
                f.Invoke(f.updatestat, "Checkmate", whiteTotalMaterials, blackTotalMaterials);
            else if (isStalemate())
                f.Invoke(f.updatestat, "Stalemate", whiteTotalMaterials, blackTotalMaterials);
            else if (kingCheck(false))
                f.Invoke(f.updatestat, "Check", whiteTotalMaterials, blackTotalMaterials);
            else
                f.Invoke(f.updatestat, "", whiteTotalMaterials, blackTotalMaterials);
        }
        public string saveFile;
        public void saveGame()
        {
            string lines = "";
            lines += (player % 2).ToString() + "\r\n";
            // Compose a string that consists of eight lines.
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                    lines += board[row, col].ToString() + "\t";
                lines += "\r\n";
            }
            for (int i = 0; i < capturedWhitePieces.Count; i++)
                lines += capturedWhitePieces[i] + "\t";
            lines += "-9\r\n";
            for (int i = 0; i < capturedBlackPieces.Count; i++)
                lines += capturedBlackPieces[i] + "\t";
            lines += "-9\r\n";
            lines += wKingMoved + "\t" + bKingMoved + "\r\n";
            lines += wKingRookMoved + "\t" + wQueenRookMoved + "\r\n";
            lines += bKingRookMoved + "\t" + bQueenRookMoved + "\r\n";
            // Write the string to a file.
            System.IO.StreamWriter file = new System.IO.StreamWriter(saveFile);
            file.WriteLine(lines);

            file.Close();
        }
    }
}
