import sun.awt.image.ImageWatched;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Random;
import java.util.Scanner;
import java.util.Stack;

/**
 * Created by Luke on 4/6/2017.
 */
public class chessGame {
    public int board[][];
    public int turn;
    public boolean promotion;
    private boolean pawn_double_skip;
    private boolean enPassant;
    private piece king;
    private piece checkSrc;
    public int light_material;
    public int dark_material;
    private boolean lkrmoved, lqrmoved, dkrmoved, dqrmoved;
    private boolean lkmoved, dkmoved;
    public boolean playAI;
    public Stack<historyMove> history;

    // game initialization-----------------------------------------------
    public chessGame() {
        board=new int[8][8];
        moves=new LinkedList<>();
        history=new Stack<>();
        load("src/gamefiles/gamefile1.txt");
        reset();
    }
    public void load(String gamefile) {
        try {
            Scanner scanner = new Scanner(new File(gamefile));
            int i;
            for (int r=0; r<8; r++)
                for (int c=0; c<8; c++) {
                    i=scanner.nextInt();
                    board[r][c]=i;
                }
            while(scanner.hasNextInt() && scanner.hasNext())
                history.add(new historyMove(scanner.nextInt(), scanner.nextInt(), scanner.nextInt(),
                        scanner.nextInt(), scanner.nextInt()));
        } catch(IOException e) {
        }
    }
    public void reset() {
        turn=0;
        light_material=39;
        dark_material=39;
        pawn_double_skip=false;
        enPassant=false;
        promotion=false;
        lkrmoved=false;
        lqrmoved=false;
        dkrmoved=false;
        dqrmoved=false;
        lkmoved=false;
        dkmoved=false;
        history.clear();
    }
    public void save(String gamefile) {
        BufferedWriter out = null;
        try {
            FileWriter fstream = new FileWriter(gamefile, false); //true tells to append data.
            out = new BufferedWriter(fstream);
            for(int r=0; r<8; r++)
                for(int c=0; c<8; c++) {
                    out.write(board[r][c]+"\n");
                }
            for(int i=0; i<history.size(); i++) {
                out.write(history.get(i).sr + " " + history.get(i).sc + " " +
                        history.get(i).dr + " " + history.get(i).dc + " " + history.get(i).target + " ");
            }
        } catch (IOException e) {
        }
        finally {
            try {
                if(out != null) {
                    out.close();
                }
            } catch (IOException e) {
            }
        }
    }
    // game initialization-----------------------------------------------

    // movement/movement-testing-----------------------------------------
    public boolean test_move(int srow, int scol, int drow, int dcol) {
        promotion=false;
        if (legal(srow, scol, drow, dcol, false)) {
            history.add(new historyMove(srow, scol, drow, dcol, board[drow][dcol]));
            make_move(srow, scol, drow, dcol);
            turn++;
            if(enPassant) board[srow][dcol]=0;
            if((drow==0 && turn%2==0) || (drow==7 && turn%2==1))
                if(Math.abs(board[drow][dcol]) == 1) promotion=true;
            return true;
        }
        return false;
    }
    private void make_move(int srow, int scol, int drow, int dcol) {
        board[drow][dcol]=board[srow][scol];
        board[srow][scol]=0;
        calc_material();
    }
    public void undo_move(int srow, int scol, int drow, int dcol, int target) {
        board[srow][scol]=board[drow][dcol];
        board[drow][dcol]=target;
    }
    private boolean legal(int srow, int scol, int drow, int dcol, boolean testing) {
        boolean valid=true;

        if (board[srow][scol] * board[drow][dcol] > 0) // attacking own piece
            valid=false;
        else {
            switch (Math.abs(board[srow][scol])) {
                case 1:
                    valid=pawn_move(srow, scol, drow, dcol, testing);
                    break;
                case 2:
                    valid=rook_move(srow, scol, drow, dcol);
                    if(valid) {
                        if (turn % 2 == 0) {
                            if (scol == 0) lqrmoved = true;
                            else lkrmoved = true;
                        } else {
                            if (scol == 0) dqrmoved = true;
                            else dkrmoved = true;
                        }
                    }
                    break;
                case 3:
                    valid=knight_move(srow, scol, drow, dcol);
                    break;
                case 4:
                    valid=bishop_move(srow, scol, drow, dcol);
                    break;
                case 5:
                    valid=queen_move(srow, scol, drow, dcol);
                    break;
                case 6:
                    valid=king_move(srow, scol, drow, dcol);
                    if(!valid && ((drow==0 && turn%2==1) || (drow==7 && turn%2==0)) && Math.abs(scol-dcol)==2)
                        valid=test_castle(srow, scol, drow, dcol);
                    if(valid) {
                        if (turn % 2 == 0) lkmoved=true;
                        else dkmoved=true;
                    }
                    break;
            }
            // pretend move
            int target=board[drow][dcol];
            make_move(srow, scol, drow, dcol);
            if (enPassant) board[srow][dcol]=0;
            if (test_check()) valid=false; // if move places own king in check or leaves king in check
            //undo move
            undo_move(srow, scol, drow, dcol, target);
            if (valid && enPassant) {
                if (turn % 2 == 0)
                    board[srow][dcol]=-1;
                else
                    board[srow][dcol]=1;
            }
        }
        return valid;
    }
    // movement/movement-testing-----------------------------------------

    // piece movements---------------------------------------------------
    private boolean pawn_move(int srow, int scol, int drow, int dcol, boolean testing) {
        boolean valid=false;
        boolean movingtwo=false;
        enPassant=false;

        if ((srow - drow) * board[srow][scol] > 0) { // moving right direction
            if (Math.abs(srow - drow) == 1 && scol == dcol && board[drow][dcol] == 0) // moving forward one
                valid = true;
            else if (Math.abs(srow - drow) == 2 && // moving forward two
                board[(srow + drow) / 2][dcol] == 0 && board[drow][dcol] == 0 && scol == dcol &&
                ((srow == 6 && turn % 2 == 0) || (srow == 1 && turn % 2 == 1))) {
                valid = true;
                if (!testing) {
                    movingtwo = true;
                    pawn_double_skip = true;
                }
            }
            else if (Math.abs(srow-drow) == 1 && Math.abs(scol-dcol) == 1) { // attacking
                if ((board[drow][dcol] * board[srow][scol] < 0))
                    valid = true;
                else if(board[srow][dcol] * board[srow][scol] < 0 && pawn_double_skip  && !testing) { // en Passant
                    valid = true;
                    enPassant=true;
                }
            }
            if (!testing && !movingtwo) {
                pawn_double_skip=false;
            }
        }
        return valid;
    }
    private boolean rook_move(int srow, int scol, int drow, int dcol) {
        boolean valid = true;

        if (srow == drow) { // moving horizontally
            int offset = dcol - scol;
            int range = Math.abs(offset);

            for (int i = 1; i < range; i++)
                if (board[srow][scol + i*offset/range] != 0) {
                    valid = false;
                    break;
                }
        }
        else if (scol == dcol) { // moving vertically
            int offset = drow - srow;
            int range = Math.abs(offset);

            for (int i = 1; i < range; i++)
                if (board[srow + i*offset/range][scol] != 0) {
                    valid = false;
                    break;
                }
        }
        else valid = false;

        return valid;
    }
    private boolean knight_move(int srow, int scol, int drow, int dcol) {
        boolean valid = false;
        if (srow != drow && scol != dcol && (Math.abs(drow - srow) + Math.abs(dcol - scol) == 3))
            valid = true;
        return valid;
    }
    private boolean bishop_move(int srow, int scol, int drow, int dcol) {
        boolean valid = true;

        if (Math.abs(srow - drow) == Math.abs(scol - dcol)) {
            int offsetcol = dcol - scol;
            int offsetrow = drow - srow;
            int range = Math.abs(offsetcol);

            for (int i=1; i<range; i++)
                if (board[srow + i*offsetrow / range][scol + i*offsetcol/range] != 0) {
                    valid = false;
                    break;
                }
        }
        else valid = false;

        return valid;
    }
    private boolean queen_move(int srow, int scol, int drow, int dcol) {
        boolean valid = false;

        if (srow == drow || scol == dcol)
            valid = rook_move(srow, scol, drow, dcol);
        else if (Math.abs(srow - drow) == Math.abs(scol - dcol))
            valid = bishop_move(srow, scol, drow, dcol);

        return valid;
    }
    private boolean king_move(int srow, int scol, int drow, int dcol) {
        boolean valid = false;

        if ((Math.abs(srow - drow) == 1 && scol == dcol) || (Math.abs(scol - dcol) == 1 && srow == drow)
                || (srow != drow && scol != dcol && Math.abs(srow - drow) + Math.abs(scol - dcol) == 2))
            valid = true;

        return valid;
    }
    // piece movements---------------------------------------------------

    // king functions----------------------------------------------------
    private boolean test_check() {
        boolean in_check=false;
        piece king = new piece(-1, -1);

        for (int row=0; row<8; row++) { // find king
            for (int col=0; col<8; col++) {
                if (((board[row][col] == 6) && turn % 2 == 0) ||
                ((board[row][col] == -6) && turn % 2 == 1)) {
                    king.r = row;
                    king.c = col;
                    break;
                }
            }
            if (king.r != -1) break;
        }
        // display board and turn in case of not finding king-error
        if(king.r==-1) {
            System.out.print("turn:" + turn%2 + "\n");
            for(int r=0; r<8; r++) {
                for (int c = 0; c < 8; c++)
                    System.out.print(board[r][c]+"\t");
                System.out.print("\n");
            }
        }
        for (int row=0; row<8; row++) {
            for (int col=0; col<8; col++) {
                if (board[king.r][king.c] * board[row][col] < 0) {
                    switch (board[row][col]) {
                        case -1:
                        case 1:
                            if (pawn_move(row, col, king.r, king.c, true))
                                in_check=true;
                            break;
                        case -2:
                        case 2:
                            if (rook_move(row, col, king.r, king.c))
                                in_check=true;
                            break;
                        case -3:
                        case 3:
                            if (knight_move(row, col, king.r, king.c))
                                in_check=true;
                            break;
                        case -4:
                        case 4:
                            if (bishop_move(row, col, king.r, king.c))
                                in_check=true;
                            break;
                        case -5:
                        case 5:
                            if (queen_move(row, col, king.r, king.c))
                                in_check=true;
                            break;
                    }
                    if (in_check) {
                        checkSrc = new piece(row, col);
                        return in_check;
                    }
                }
            }
        }
        return in_check;
    }
    public boolean test_checkmate() {
        int srow=-1;
        int scol=-1;
        int drow=-1;
        int dcol=-1;

        if (!test_check() || king_can_move()) return false;

        // get coordinates of squares in between king and attacker as well as of attacker
        int offsetInt, range;
        int offsetcol, offsetrow;
        piece[] threatArray;

        if (king.r == checkSrc.r) {
            offsetInt = checkSrc.c - king.c;
            range = Math.abs(offsetInt);

            threatArray = new piece[range];

            for (int i = 0; i < range; i++)
                threatArray[i] = new piece(king.r, king.c + (i+1)*offsetInt/range);
        }
        else if (king.c == checkSrc.c) {
            offsetInt = checkSrc.r-king.r;
            range = Math.abs(offsetInt);

            threatArray = new piece[range];

            for (int i=0; i<range; i++)
                threatArray[i]=new piece(king.r + (i+1)*offsetInt/range, king.c);
        }
        else {
            offsetcol=checkSrc.c-king.c;
            offsetrow=checkSrc.r-king.r;
            range=Math.abs(offsetcol);

            threatArray = new piece[range];

            for (int i = 0; i < range; i++)
                threatArray[i] = new piece(king.r + (i+1)*offsetrow/range, king.c + (i+1)*offsetcol/range);
        }
        for (int r = 0; r < 8; r++) // see if check can be blocked by another piece
            for (int c = 0; c < 8; c++) {
                if (board[r][c] * board[king.r][king.c] > 0) {
                    srow=r;
                    scol=c;
                    for (int i = 0; i < range; i++) {
                        drow=threatArray[i].r;
                        dcol=threatArray[i].c;
                        if (legal(srow, scol, drow, dcol, true))
                            return false;
                    }
                }
            }
        return true;
    }
    private boolean king_can_move() {
        piece dest;
        boolean kingFound = false;
        int srow=-1;
        int scol=-1;
        int[][] offset=new int[][] {
            {1,0},
            {0,1},
            {-1,0},
            {0,-1},
            {1,1},
            {1,-1},
            {-1,1},
            {-1,-1}
        };

        for (int row=0; row<8; row++) {
            for (int col=0; col<8; col++) {
                if ((board[row][col] == 6 && turn % 2 == 0) ||
                (board[row][col] == -6 && turn % 2 == 1)) {
                    srow = row;
                    scol = col;
                    king = new piece(row, col);
                    kingFound=true;
                    break;
                }
            }
            if (kingFound) break;
        }
        for (int i=0; i<8; i++) {
            dest = new piece(srow + offset[i][0], scol + offset[i][1]);
            if (dest.r > -1 && dest.r < 8 && dest.c > -1 && dest.c < 8) {
                if ((board[srow][scol] * board[dest.r][dest.c] <= 0)
                        && king_move(srow, scol, dest.r, dest.c)) { // if king has way out
                    int target = board[dest.r][dest.c];
                    make_move(srow, scol, dest.r, dest.c);
                    if (!test_check()) {
                        undo_move(srow, scol, dest.r, dest.c, target);
                        return true;
                    }
                    undo_move(srow, scol, dest.r, dest.c, target);
                }
            }
        }
        return false;
    }
    public boolean test_stalemate() {
        boolean stalemate=false;
        if (light_material==0 && dark_material==0) stalemate=true; // only kings left on board
        else if (light_material==0 || dark_material==0)
            if (insufficientMaterial()) stalemate=true; // only kings and one minor piece left on board
            else if (!test_check() && ((light_material==0 && turn%2==0) || (dark_material==0 && turn%2==1)))
                if (!king_can_move()) stalemate=true; // lone king has no move and it's his turn
        return stalemate;
    }
    private boolean insufficientMaterial() {
        boolean insufficient=false;
        if (light_material==3 || dark_material==3) {
            for (int row = 0; row < 8; row++)
                for (int col = 0; col < 8; col++) {
                    if ((board[row][col]==1 && light_material==3) || (board[row][col]==-1 && dark_material==3))
                        return false;
                }
            insufficient=true;
        }
        return insufficient;
    }
    private boolean test_castle(int srow, int scol, int drow, int dcol) {
        boolean valid=true;
        if(turn%2==0) { // light turn
            if(lkmoved) valid=false;
            else if(dcol==2 && lqrmoved)
                valid=false;
            else if(dcol==6 && lkrmoved)
                valid=false;
        }
        else { // dark turn
            if(dkmoved) valid=false;
            else if(dcol==2 && dqrmoved)
                valid=false;
            else if(dcol==6 && dkrmoved)
                valid=false;
        }
        if (valid) { // test squares between them and threats
            int offset;
            int range;
            if (dcol == 2) {
                offset=-4;
                range=4;
            }
            else {
                offset=3;
                range=3;
            }
            for (int i=1; i<range; i++) // see if path is clear
                if (board[srow][scol + i*offset/range] != 0)
            valid=false;
            if (valid) // if path is clear
                for (int i=1; i<3; i++) { // see if path is threatened
                    piece trialDest=new piece(srow, scol + i*offset/range);
                    make_move(srow, scol, drow, trialDest.c);
                    if (test_check()) valid=false;
                    undo_move(srow, scol, drow, trialDest.c, 0);
                    if (!valid) break;
                }
            if (valid) {
                if(srow==0 && dcol==2) {
                    board[0][0]=0;
                    board[0][3]=-2;
                }
                else if(srow==0 && dcol==6) {
                    board[0][7]=0;
                    board[0][5]=-2;
                }
                else if(srow==7 && dcol==2) {
                    board[7][0]=0;
                    board[7][3]=2;
                }
                else if(srow==7 && dcol==6) {
                    board[7][7]=0;
                    board[7][5]=2;
                }
            }
        }
        return valid;
    }
    // king functions----------------------------------------------------

    // AI functions------------------------------------------------------
    public void AImove() {
        int sr=7-history.peek().sr;
        int sc=7-history.peek().sc;
        int dr=7-history.peek().dr;
        int dc=7-history.peek().dc;
        make_move(sr,sc,dr,dc);
        turn++;
        /*findAllMoves();

        int num_wins;
        int most_wins=-1;
        move bestMove=new move(-1, -1, -1, -1, -1);
        for(int i=0; i<1; i++) {
            num_wins=0;

            Random r=new Random();
            int n=r.nextInt(moves.size());
            int target=board[moves.get(n).dr][moves.get(n).dc];

            num_wins++;

            /*make_move(moves.get(n).sr, moves.get(n).sc, moves.get(n).dr, moves.get(n).dc);

            turn++;
            for(int j=0; j<1; j++)
                num_wins += monteCarlo();

            undo_move(moves.get(n).sr, moves.get(n).sc, moves.get(n).dr, moves.get(n).dc, target);
            turn--;*/

            /*if(num_wins > most_wins) {
                most_wins=num_wins;
                bestMove=new move(moves.get(n).sr, moves.get(n).sc, moves.get(n).dr, moves.get(n).dc, num_wins);
            }
        }
        make_move(bestMove.sr, bestMove.sc, bestMove.dr, bestMove.dc);
        turn++;*/
    }
    private LinkedList<move> moves;
    private void findAllMoves() {
        moves.clear();
        for(int row=0; row<8; row++)
            for(int col=0; col<8; col++) {
                if(turn % 2 == 0) {
                    if(board[row][col] > 0)
                        populateMoveList(row, col);
                }
                else if (board[row][col] < 0)
                    populateMoveList(row, col);
            }
    }
    private void populateMoveList(int row, int col) {
        for(int r=0; r<8; r++)
            for(int c=0; c<8; c++) {
                if (legal(row, col, r, c, true)) {
                    moves.add(new move(row, col, r, c, 0));
                    if((r==0 && turn%2==0) || (r==7 && turn%2==1)) {
                        if (Math.abs(board[row][col]) == 1) {
                            // test promotion options
                            board[row][col]=0;
                            int target=board[r][c];
                            if(board[row][col] > 0) {
                                // TODO white promotion
                                board[r][c]=5;
                                // evaluate board
                                board[r][c]=4;
                                // evaluate board
                                board[r][c]=3;
                                // evaluate board
                                board[r][c]=2;
                                // evaluate board

                                // reset pawn
                                board[row][col]=1;
                            }
                            else {
                                // TODO black promotion
                                board[r][c]=-5;
                                // evaluate board
                                board[r][c]=-4;
                                // evaluate board
                                board[r][c]=-3;
                                // evaluate board
                                board[r][c]=-2;
                                // evaluate board

                                // reset pawn
                                board[row][col]=-1;
                            }
                            // undo move
                            board[r][c]=target;
                        }
                    }
                }
            }
    }
    private int monteCarlo() {
        int win;

        if(test_checkmate() && turn%2==1) // if AI wins
            return 1;
        findAllMoves(); // find moves available
        if(moves.size()==0) // stalemate or AI loses
            return 0;

        Random r=new Random();
        int n=r.nextInt(moves.size());
        int sr=moves.get(n).sr;
        int sc=moves.get(n).sc;
        int dr=moves.get(n).dr;
        int dc=moves.get(n).dc;
        int target=board[moves.get(n).dr][moves.get(n).dc];

        make_move(sr, sc, dr, dc);
        turn++;
        win=monteCarlo();
        undo_move(sr, sc, dr, dc, target);
        turn--;

        return win;
    }
    // AI functions------------------------------------------------------

    // scoring functions-------------------------------------------------
    public void calc_material() {
        light_material=0;
        dark_material=0;
        int value=0;

        for(int r=0; r<8; r++)
            for(int c=0; c<8; c++) {
                switch(board[r][c]) {
                    case -1:
                    case 1:
                        value=1;
                        break;
                    case -2:
                    case 2:
                        value=5;
                        break;
                    case -3:
                    case 3:
                        value=3;
                        break;
                    case -4:
                    case 4:
                        value=3;
                        break;
                    case -5:
                    case 5:
                        value=9;
                        break;
                    default:
                        value=0;
                        break;
                }
                if(board[r][c] > 0)
                    light_material+=value;
                else
                    dark_material+=value;
            }
    }
    // scoring functions-------------------------------------------------
}
