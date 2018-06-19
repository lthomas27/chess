import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by Luke on 4/6/2017.
 */
public class chessController extends chessView implements ActionListener {
    boolean endofgame;
    public chessController() {
        Save.addActionListener(this);
        Load.addActionListener(this);
        Restart.addActionListener(this);
        Undo.addActionListener(this);
        Resign.addActionListener(this);
        PlayAI.addActionListener(this);
        PlayHuman.addActionListener(this);
        for (int row=0; row<8; row++) {
            for (int col=0; col<8; col++) {
                buttons[row][col].putClientProperty("row", row);
                buttons[row][col].putClientProperty("col", col);
                buttons[row][col].addActionListener(this);
            }
        }
        endofgame=false;
    }
    boolean piece_selected=false;
    boolean moved=false;
    int srow, scol, drow, dcol;
    public void actionPerformed (ActionEvent e) {
        Object buttonClicked=e.getSource();

        if (buttonClicked==super.Save) {
            saveGame();
        }
        else if (buttonClicked==super.Load) {
            loadGame();
        }
        else if(buttonClicked==super.Restart) {
            newGame.load("src/gamefiles/gamefile1.txt");
            newGame.reset();
        }
        else if(buttonClicked==super.Undo) {
            if (!newGame.history.isEmpty()) {
                historyMove hm = newGame.history.pop();
                newGame.undo_move(hm.sr, hm.sc, hm.dr, hm.dc, hm.target);
                newGame.turn--;
                if(newGame.playAI) {
                    hm = newGame.history.pop();
                    newGame.undo_move(hm.sr, hm.sc, hm.dr, hm.dc, hm.target);
                    newGame.turn--;
                }
            }
        }
        else if(buttonClicked==super.Resign) {
            endofgame=true;
        }
        else if(buttonClicked==super.PlayAI) {
            newGame.playAI=true;
        }
        else if(buttonClicked==super.PlayHuman) {
            newGame.playAI=false;
        }
        else {
            JButton btn = (JButton) e.getSource();
            if (piece_selected) {
                drow = (Integer) btn.getClientProperty("row");
                dcol = (Integer) btn.getClientProperty("col");

                if(newGame.turn%2==0 && newGame.board[drow][dcol] > 0) { // choosing another piece
                    resetColors();
                    srow = (Integer) btn.getClientProperty("row");
                    scol = (Integer) btn.getClientProperty("col");
                    if ((newGame.board[srow][scol] > 0 && newGame.turn % 2 == 0) ||
                            (newGame.board[srow][scol] < 0 && newGame.turn % 2 == 1)) {
                        piece_selected = true;
                        buttons[srow][scol].setBackground(Color.CYAN);
                    }
                }
                else {
                    moved = newGame.test_move(srow, scol, drow, dcol);
                    piece_selected = false;
                    if (newGame.promotion) {
                        final ImageIcon icon;
                        if (newGame.turn % 2 == 1)
                            icon = new ImageIcon("src/images/lpromote.png");
                        else
                            icon = new ImageIcon("src/images/dpromote.png");
                        String[] options = {"Queen", "Bishop", "Knight", "Rook"};
                        int choice = -1;
                        while (choice == -1) {
                            choice = JOptionPane.showOptionDialog(null, "Promotion Choice",
                                    "Pawn Promotion",
                                    JOptionPane.NO_OPTION, JOptionPane.QUESTION_MESSAGE, icon, options, options[0]);
                        }
                        switch (choice) {
                            case 0:
                                if (newGame.turn % 2 == 1)
                                    newGame.board[drow][dcol] = 5;
                                else
                                    newGame.board[drow][dcol] = -5;
                                break;
                            case 1:
                                if (newGame.turn % 2 == 1)
                                    newGame.board[drow][dcol] = 4;
                                else
                                    newGame.board[drow][dcol] = -4;
                                break;
                            case 2:
                                if (newGame.turn % 2 == 1)
                                    newGame.board[drow][dcol] = 3;
                                else
                                    newGame.board[drow][dcol] = -3;
                                break;
                            case 3:
                                if (newGame.turn % 2 == 1)
                                    newGame.board[drow][dcol] = 2;
                                else
                                    newGame.board[drow][dcol] = -2;
                                break;
                        }
                        newGame.calc_material();
                    }
                    if (newGame.test_checkmate()) {
                        JOptionPane.showMessageDialog(super.chessBoardArea, "Checkmate.");
                        endofgame = true;
                    }
                    if (newGame.test_stalemate()) {
                        JOptionPane.showMessageDialog(super.chessBoardArea, "Stalemate.");
                        endofgame = true;
                    }
                    // reset colors
                    resetColors();
                }
            }
            else {
                srow = (Integer) btn.getClientProperty("row");
                scol = (Integer) btn.getClientProperty("col");
                if ((newGame.board[srow][scol] > 0 && newGame.turn % 2 == 0) ||
                        (newGame.board[srow][scol] < 0 && newGame.turn % 2 == 1)) {
                    piece_selected = true;
                    buttons[srow][scol].setBackground(Color.CYAN);
                }
            }
        }
        if (endofgame) {
            repaint();
            this.setEnabled(false);
            final ImageIcon icon;
            if (newGame.turn % 2 == 0)
                icon = new ImageIcon("src/images/dwin.png");
            else
                icon = new ImageIcon("src/images/lwin.png");
            String[] options = {"New Game", "Load Game", "Quit"};
            int choice = -1;
            while (choice == -1) {
                choice = JOptionPane.showOptionDialog(null, "Options", "Game Over",
                        JOptionPane.NO_OPTION, JOptionPane.QUESTION_MESSAGE, icon, options, options[0]);
            }
            this.setEnabled(true);
            if (choice == 0) {
                endofgame = false;
                newGame.reset();
                newGame.load("src/gamefiles/gamefile1.txt");
            } else if (choice == 1) loadGame();
            else System.exit(0);
        }
        else if(moved && !piece_selected && newGame.playAI)
            newGame.AImove();
        lscore.setText("White: " + Integer.toString(newGame.light_material-newGame.dark_material));
        dscore.setText("Black: " + Integer.toString(newGame.dark_material-newGame.light_material));
        moved=false;
        repaint();
    }
    private void resetColors() {
        int cnt = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                cnt++;
                if (cnt % 2 == 1)
                    buttons[row][col].setBackground(Color.white);
                else
                    buttons[row][col].setBackground(Color.gray);
            }
            cnt++;
        }
    }
    private void saveGame() {
        String[] loadoptions = {"Slot 1", "Slot 2", "Slot 3"};
        int choice = -1;
        while (choice == -1) {
            choice = JOptionPane.showOptionDialog(null, "Game Slots", "Save Game",
                    JOptionPane.NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, loadoptions, loadoptions[0]);
        }
        switch (choice) {
            case 0:
                newGame.save("src/gamefiles/saved1.txt");
                break;
            case 1:
                newGame.save("src/gamefiles/saved2.txt");
                break;
            case 2:
                newGame.save("src/gamefiles/saved3.txt");
                break;
            default:
                break;
        }
    }
    private void loadGame() {
        endofgame = false;
        newGame.reset();
        String[] loadoptions = {"Game 1", "Game 2", "Game 3"};
        int choice = -1;
        while (choice == -1) {
            choice = JOptionPane.showOptionDialog(null, "Saved Games", "Load Game",
                    JOptionPane.NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, loadoptions, loadoptions[0]);
        }
        switch (choice) {
            case 0:
                newGame.load("src/gamefiles/saved1.txt");
                break;
            case 1:
                newGame.load("src/gamefiles/saved2.txt");
                break;
            case 2:
                newGame.load("src/gamefiles/saved3.txt");
                break;
            default:
                break;
        }
    }
}
