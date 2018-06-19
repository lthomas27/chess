import javax.imageio.*;
import javax.swing.*;
import java.awt.*;

/**
 * Created by Luke on 4/6/2017.
 */
public class chessView extends JFrame {
    protected JButton buttons[][] = new JButton[8][8];
    JPanel menuArea = new JPanel(new GridLayout(5, 1));
    protected JButton Save;
    protected JButton Load;
    protected JButton Restart;
    protected JButton Undo;
    protected JButton Resign;

    protected JRadioButton PlayAI;
    protected JRadioButton PlayHuman;

    protected JLabel lscore;
    protected JLabel dscore;

    JPanel chessBoardArea = new JPanel();
    String title="Chess";
    protected int width, height;

    protected chessGame newGame = new chessGame();

    protected chessView() {
        width=750;
        height=750;
        setMinimumSize(new Dimension(width, height));
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setTitle(title);
        setBackground(Color.BLACK);
        setLayout(new BorderLayout());

        Save = new JButton("Save");
        Load = new JButton("Load");
        Restart = new JButton("Restart");
        Undo = new JButton("Undo");
        Resign = new JButton("Resign");

        ButtonGroup modes=new ButtonGroup();
        PlayAI = new JRadioButton("AI");
        newGame.playAI=true;
        PlayAI.setSelected(true);
        PlayHuman = new JRadioButton("Human");
        modes.add(PlayAI);
        modes.add(PlayHuman);

        JPanel buttonContainer=new JPanel(new GridLayout(7, 1));
        buttonContainer.add(Save);
        buttonContainer.add(Load);
        buttonContainer.add(Restart);
        buttonContainer.add(Undo);
        buttonContainer.add(Resign);
        buttonContainer.add(PlayAI);
        buttonContainer.add(PlayHuman);
        menuArea.add(buttonContainer);

        lscore=new JLabel("White: -");
        dscore=new JLabel("Black: -");
        menuArea.add(dscore);
        menuArea.add(lscore);

        add(menuArea, BorderLayout.WEST);

        GridLayout board = new GridLayout(8,8);
        chessBoardArea.setLayout(board);

        int cnt=0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                cnt++;
                JButton btn = new JButton();
                buttons[row][col] = btn;
                btn.setPreferredSize(new Dimension(80, 80));
                if (cnt % 2 != 0)
                    btn.setBackground(Color.white);
                else
                    btn.setBackground(Color.gray);
                chessBoardArea.add(btn);
            }
            cnt++;
        }
        add(chessBoardArea);
        setResizable(false);
        setVisible(true);
    }
    void drawChessBoard(Graphics g) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (newGame.board[row][col] == 0) {
                    buttons[row][col].setIcon(null);
                }
                else {
                    try {
                        Image img=ImageIO.read(getClass().getResource("images/"+newGame.board[row][col]+".png"));
                        buttons[row][col].setIcon(new ImageIcon(img));
                    } catch (Exception ex) {
                        System.out.println(ex);
                    }
                }
            }
        }
    }
    public void paint(Graphics g) {
        super.paint(g);
        Graphics bg = chessBoardArea.getGraphics();
        chessBoardArea.paint(bg);
        drawChessBoard(bg);
    }
}
