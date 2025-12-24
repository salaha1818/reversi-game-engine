#include "reversi.h"
#include "helper.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declare function prototypes
void printBoard(char board[][26], int n);
void setBoard(char board[][26], int size);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char color, int deltaRow, int deltaCol);
void checkDirections(char board[][26], int n, char color,
                     char validMoves[100][3], int *counter);
bool isValidMove(char c2, char c3, char validMoves[100][3], int counter);
void switchColors(char board[][26], int n, int row, int col, char color);
int countFlips(char board[][26], int n, int row, int col, char color,
               int deltaRow, int deltaCol);
int countTotalFlips(char board[][26], int n, int row, int col, char color);
bool checkWin(char board[][26], int n);
int makeMove(const char board[][26], int n, char turn, int *row, int *col);

char computer;
char ai;

Main function commented out for submission.
int main(void) {
  // declare variables
  int n;
  char board[26][26];
  char validMovesH[100][3];
  char validMovesC[100][3];
  int counterai = 0, counterComputer = 0;

  // ask user to enter an even dimension
  do {
    printf("Enter the board dimension: ");
    scanf("%d", &n);
  } while (n % 2 != 0);

  printf("Computer plays (B/W) : ");
  scanf(" %c", &computer);

  // set and print initial board
  setBoard(board, n);
  printBoard(board, n);

  do {
    bool moveMade = false;

    if (computer == 'B') {
      ai = 'W';

      // computer turn
      counterComputer = 0;
      checkDirections(board, n, computer, validMovesC, &counterComputer);

      if (counterComputer > 0) {
        int row1, col1;
        // find and play best computer move
        makeMove(board, n, computer, &row1, &col1);
        printf("Computer places W at %c%c.\n", row1 + 'a', col1 + 'a');
        switchColors(board, n, row1, col1, computer);
        printBoard(board, n);
        moveMade = true;
        if (checkWin(board, n))
          return 0; // check for winner
      } else {
        printf("%c player has no valid move.\n",
               computer); // indicate if computer has no moves
      }

      // ai turn
      counterai = 0;
      checkDirections(board, n, ai, validMovesH, &counterai);

      // if there is atleast one possible move, let ai make move
      if (counterai > 0) {
        int row2, col2;
        // find and play best ai move
        findSmartestMove(board, n, ai, &row2, &col2);
        printf("Testing AI move (row, col): %c%c\n", row2 + 'a', col2 + 'a');
        switchColors(board, n, row2, col2, ai);
        printBoard(board, n);
        moveMade = true;
        if (checkWin(board, n))
          return 0; // check for winner
      } else {
        printf("%c player has no valid move.\n",
               ai); // indicate if ai has no moves
      }
    } else { // computer is 'W'
      ai = 'B';

      // ai turn
      counterai = 0;
      checkDirections(board, n, ai, validMovesH, &counterai);

      // if there is atleast one possible move, let ai make move
      if (counterai > 0) {
        int row3, col3;
        // find and play best ai move
        findSmartestMove(board, n, ai, &row3, &col3);
        printf("Testing AI move (row, col): %c%c\n", row3 + 'a', col3 + 'a');
        switchColors(board, n, row3, col3, ai);
        printBoard(board, n);
        moveMade = true;
        if (checkWin(board, n))
          return 0; // check for winner.
      } else {
        printf("%c player has no valid move.\n",
               ai); // indicate if ai has no moves
      }

      // computer turn
      counterComputer = 0;
      checkDirections(board, n, computer, validMovesC, &counterComputer);

      // find and play best computer move
      if (counterComputer > 0) {
        int row4, col4;
        makeMove(board, n, computer, &row4, &col4);
        // Add validation
        if (row4 >= 0 && col4 >= 0 && row4 < n && col4 < n) {
          printf("Computer places W at %c%c.\n", row4 + 'a', col4 + 'a');
          switchColors(board, n, row4, col4, computer);
          printBoard(board, n);
          moveMade = true;
          if (checkWin(board, n))
            return 0; // check for winner
        } else {
          printf("Invalid computer move detected!\n"); // indicate if computer
                                                       // has no moves
        }
      }
    }

    // check if game should end
    if (!moveMade) {
      printf("No more valid moves for either player.\n");
      checkWin(board, n);
      break;
    }
  } while (true);
  return 0;
}

// create board and display starting positions
void setBoard(char board[][26], int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      // set top left and bottom right of the four middle pieces to W
      if (i == j && (i == size / 2 || i == (size / 2) - 1)) {
        board[i][j] = 'W';
      }
      // set bottom left and top right of the four middle pieces to B
      else if ((i + j == size - 1) && (i == size / 2 || i == (size / 2) - 1)) {
        board[i][j] = 'B';
      }
      // set all other pieces to U
      else {
        board[i][j] = 'U';
      }
    }
  }
}

// print current board
void printBoard(char board[][26], int n) {
  char abc[26] = "abcdefghijklmnopqrstuvwxyz";
  // print column indicators
  printf("  ");
  for (int i = 0; i < n; i++) {
    printf("%c", abc[i]);
  }
  printf("\n");

  for (int i = 0; i < n; i++) {
    printf("%c ", abc[i]); // print row indicators
    for (int j = 0; j < n; j++) {
      printf("%c", board[i][j]); // print booard
    }
    printf("\n");
  }
}

// check if position is is bounds
bool positionInBounds(int n, int row, int col) {
  if (row >= 0 && col >= 0 && row < n && col < n) {
    return true;
  } else {
    return false;
  }
}

// check for legal moves in given direction for a given color
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char color, int deltaRow, int deltaCol) {

  bool oppPeice = false;

  // check if position is W or B (given colour), and set opponent as the other
  int opponent;
  if (color == 'B') {
    opponent = 'W';
  } else {
    opponent = 'B';
  }

  // set row and col to next place in given direction
  row += deltaRow;
  col += deltaCol;

  // check if the next place is in bounds and determine if it's valid
  while (positionInBounds(n, row, col)) {

    // if opponent is in next place, indicate that there is an opponent piece
    if (board[row][col] == opponent) {
      oppPeice = true;
    }

    // if next place is empty, move is not valid
    if (board[row][col] == 'U') {
      return false;
    }

    // if next peice is the players color, return true or false based on whether
    // an opponent peice was is the path
    if (board[row][col] == color) {
      return oppPeice;
    }

    // increment rows and columns to check all places in given direction
    row += deltaRow;
    col += deltaCol;
  }
  return false;
}

// find and keep track of legal moves for given color
void checkDirections(char board[][26], int n, char color,
                     char validMoves[100][3], int *counter) {
  char abc[26] = "abcdefghijklmnopqrstuvwxyz";

  // interates through the entire board and checks all places to identify any
  // valid moves
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // checks for valid moves in eight directions and ensures the place is
      // empty
      if (board[i][j] == 'U' &&
          (checkLegalInDirection(board, n, i, j, color, -1, 0) ||  // up
           checkLegalInDirection(board, n, i, j, color, 1, 0) ||   // down
           checkLegalInDirection(board, n, i, j, color, 0, -1) ||  // left
           checkLegalInDirection(board, n, i, j, color, 0, 1) ||   // right
           checkLegalInDirection(board, n, i, j, color, -1, -1) || // up-left
           checkLegalInDirection(board, n, i, j, color, -1, 1) ||  // up-right
           checkLegalInDirection(board, n, i, j, color, 1, -1) ||  // down-left
           checkLegalInDirection(board, n, i, j, color, 1, 1))) {  // down-right

        // stores the row and column of all valid moves in a string array
        validMoves[*counter][0] = abc[i];
        validMoves[*counter][1] = abc[j];
        validMoves[*counter][2] = '\0';
        (*counter)++; // keeps track of the size of the array (number of valid
                      // moves)
      }
    }
  }
}

bool isValidMove(char c2, char c3, char validMoves[100][3], int counter) {
  for (int i = 0; i < counter; i++) {
    if (validMoves[i][0] == c2 && validMoves[i][1] == c3) {
      return true;
    }
  }
  return false;
}

// finds the peices being sandwiched and flips them
void switchColors(char board[][26], int n, int row, int col, char color) {
  int directions[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                          {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  char opponent;
  if (color == 'B') {
    opponent = 'W';
  } else {
    opponent = 'B';
  }

  // place peice on board
  board[row][col] = color;

  // interate through the 8 different directions to find all the pieces that
  // need to be swapped.
  for (int i = 0; i < 8; i++) {
    int deltaRow = directions[i][0];
    int deltaCol = directions[i][1];
    int checkRow = row + deltaRow;
    int checkCol = col + deltaCol;
    bool foundOpponent = false;

    // move in the given direction and ensure there is an opponent piece
    while (positionInBounds(n, checkRow, checkCol) &&
           board[checkRow][checkCol] == opponent) {
      checkRow += deltaRow;
      checkCol += deltaCol;
      foundOpponent = true;
    }

    // if there are pieces being sandwiched between the peice places and another
    // peice start from the first position after the peice and change all the
    // opponent pieces to the players piece.
    if (foundOpponent && positionInBounds(n, checkRow, checkCol) &&
        board[checkRow][checkCol] == color) {
      // reset back to the start of the sandwich path
      checkRow = row + deltaRow;
      checkCol = col + deltaCol;

      // flip all opponent pieces in this direction
      while (board[checkRow][checkCol] == opponent) {
        board[checkRow][checkCol] = color;
        checkRow += deltaRow;
        checkCol += deltaCol;
      }
    }
  }
}

// counts flips for a given direction
int countFlips(char board[][26], int n, int row, int col, char color,
               int deltaRow, int deltaCol) {
  char opponent;
  int flips = 0;
  int checkRow = row + deltaRow;
  int checkCol = col + deltaCol;

  // sets opponent
  if (color == 'B') {
    opponent = 'W';
  } else {
    opponent = 'B';
  }

  // counts the nummber of consecutive opponent pieces in a given direction,
  // that will be flipped.
  while (positionInBounds(n, checkRow, checkCol) &&
         board[checkRow][checkCol] == opponent) {
    flips++;
    checkRow += deltaRow;
    checkCol += deltaCol;
  }

  // once the players peice is reached, stops counting.
  if (positionInBounds(n, checkRow, checkCol) &&
      board[checkRow][checkCol] == color) {
    return flips;
  }
  return 0;
}

// counts the total number of flips for a given move
int countTotalFlips(char board[][26], int n, int row, int col, char color) {
  int directions[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                          {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  int totalFlips = 0;

  // iterates through the different directions, and counts the number of peices
  // to be flipped.
  for (int i = 0; i < 8; i++) {
    int incFlips = countFlips(board, n, row, col, color, directions[i][0],
                              directions[i][1]);
    totalFlips += incFlips;
  }
  // returns the total number of flips for a move.
  return totalFlips;
}

// check if any player won
bool checkWin(char board[][26], int n) {
  int countFull = 0, countW = 0, countB = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // count the number of full spaces.
      if (board[i][j] != 'U') {
        countFull++;
      }
      // count the number of W pieces.
      if (board[i][j] == 'W') {
        countW++;
      }
      // count the number of B pieces.
      if (board[i][j] == 'B') {
        countB++;
      }
    }
  }

  // if board is full and there are more W pieces, W wins.
  if (countFull == n * n && countW > countB) {
    printf("W player wins.\n");
    return true;
  }

  // if board is full and there are more B pieces, B wins.
  else if (countFull == n * n && countW < countB) {
    printf("B player wins.\n");
    return true;
  }
  return false;
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  char tempBoard[26][26];
  char validMoves[100][3];
  int counter = 0;
  int bestScore = -1;
  bool firstMove = true; // bool to track if move has been selected yet

  // copy board to tempBoard since board given is a const
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      tempBoard[i][j] = board[i][j];

  // Get all valid moves
  checkDirections(tempBoard, n, turn, validMoves, &counter);
  if (counter == 0)
    return 0; // return if no moves available

  // set reversi position value grid
  // https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.researchgate.net%2Ffigure%2FPosition-value-of-each-cell-in-the-Othello-board_fig2_328458543&psig=AOvVaw2uVcFd7EYP7zY0-9kXqAgc&ust=1743365106346000&source=images&cd=vfe&opi=89978449&ved=0CBcQjhxqFwoTCIjsk8-LsIwDFQAAAAAdAAAAABAE
  int moveValue[26][26];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if ((i == 0 || i == n - 1) && (j == 0 || j == n - 1))
        moveValue[i][j] = 100; // set corners
      else if ((i == 1 || i == n - 2) && (j == 1 || j == n - 2))
        moveValue[i][j] = -25; // set diagonal to corners
      else if ((i == 0 || i == n - 1) && (j == 1 || j == n - 2))
        moveValue[i][j] = -20; // set edge horizontal
      else if ((j == 0 || j == n - 1) && (i == 1 || i == n - 2))
        moveValue[i][j] = -20; // set edge vertical
      else if (i == 0 || i == n - 1 || j == 0 || j == n - 1)
        moveValue[i][j] = 10; // set safe edges
      else
        moveValue[i][j] = 1; // set remaining places
    }
  }

  // pick best move
  for (int i = 0; i < counter; i++) {
    int r = validMoves[i][0] - 'a';
    int c = validMoves[i][1] - 'a';
    int flips = countTotalFlips(tempBoard, n, r, c,
                                turn); // count total number of flips
    int score = moveValue[r][c] +
                (flips * 10); // calculate score by adding the position value
                              // and the number of flips

    // if the first move being considered is valid or its score is higher than
    // current best or scores are equal but the move is in a lower index then
    // the move should be considered the new best
    if (firstMove || score > bestScore ||
        (score == bestScore && (r < *row || (r == *row && c < *col)))) {
      bestScore = score;
      *row = r;
      *col = c;
      firstMove = false; // set back to false before re-evaluating next move
    }
  }
  return 0;
}
