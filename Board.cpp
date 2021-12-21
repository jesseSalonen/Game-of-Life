#include "Board.h"
#include <iostream>

using std::cout;
using std::endl;
// Default constructor with board size of 10
Board::Board() : mBoardSize{ 10 } {
    for (int i = 0; i < mBoardSize; i++) {
        mBoard.push_back(std::make_shared<std::vector<bool>>());
        for (int j = 0; j < mBoardSize; j++) {
            mBoard.back()->push_back(false);
        }
    }
}
// Parameter constructor with the size of the board coming from user input
Board::Board(const int aBoardSize) : mBoardSize{ aBoardSize } {
    for (int i = 0; i < mBoardSize; i++) {
        mBoard.push_back(std::make_shared<std::vector<bool>>());
        for (int j = 0; j < mBoardSize; j++) {
            mBoard.back()->push_back(false);
        }
    }
}
// Destructor
Board::~Board() {
}
// Getter for board's size
int Board::getBoardSize() const {
    return mBoardSize;
}
// turn a dead cell into an alive cell at the given coordinates
void Board::reviveCell(const int x, const int y) {
    if (mBoard[y]->at(x) == true) cout << "Cell (" << x << "," << y << ") already alive, no changes made to the board!" << endl;
    else {
        mBoard[y]->at(x) = true;
        cout << "Cell (" << x << "," << y << ") revived!" << endl;
    }
}
// Processes the calculations between each state
int Board::processNextState() {
    // Temporary board-vector for saving the new state's cells
    std::vector<std::shared_ptr<std::vector<bool>>> newBoard;
    int neighborCount = 0;
    int changeCount = 0;

    for (int i = 0; i < mBoardSize; i++) {
        newBoard.push_back(std::make_shared<std::vector<bool>>());
        for (int j = 0; j < mBoardSize; j++) {
            // Calcutate the neighbor count for all cells in the board
            for (int y = i - 1; y <= i + 1; y++) {
                for (int x = j - 1; x <= j + 1; x++) {
                    if (y >= 0 && x >= 0 && y < mBoardSize && x < mBoardSize && (y != i || x != j)) {
                        if (mBoard[y]->at(x) == true) neighborCount++;
                    }
                }
            }
            // after calculating the neighbor count, process the cell using the game rules

            // If the cell is alive
            if (mBoard[i]->at(j) == true) {
                // If the cell's neighbor count is other than 2 or 3, change it to dead
                if (neighborCount != 2 && neighborCount != 3) {
                    newBoard.back()->push_back(false);
                    changeCount++;
                }
                // Otherwise, keep it as alive
                else newBoard.back()->push_back(true);
            }
            // If the cell is dead
            else {
                // Only if the cell has exactly 3 neighbors, change it to alive
                if (neighborCount == 3) {
                    newBoard.back()->push_back(true);
                    changeCount++;
                }
                // Otherwise don't do anything
                else newBoard.back()->push_back(false);
            }
            neighborCount = 0;
        }
    }
    // Replace the old board with a new one
    mBoard = newBoard;

    return changeCount;
}
// method for printing the game board
void Board::printBoard() const {
    for (int y = 0; y < mBoardSize * 2 + 3; y++) {
        for (int x = 0; x < mBoardSize * 2 + 3; x++) {
            // If printing the first two rows, print column numbers
            if (y <= 1) {
                if (x % 2 != 0 && x >= 3) {
                    if (int((x-2) / 2) >= 10 && y == 0) cout << std::to_string(int((x-2) / 2)).front();
                    else if (int((x-2) / 2) < 10 && y == 1) cout << int((x-2) / 2);
                    else if (int((x-2) / 2) >= 10 && y == 1) cout << std::to_string(int((x-2) / 2)).back();
                    else cout << " ";
                }
                else cout << " ";
            }
            // If not the first row and printing the first two symbols, print row numbers
            else if (x <= 1) {
                if (y % 2 != 0) {
                    cout << int((y-3) / 2);
                    if (int((y-3) / 2) <= 9) cout << " ";
                    x++;
                }
                else cout << " ";
            }
            // If not the first row and is a cell row, print either a cell wall or a cell value
            else if (y % 2 != 0) {
                if (x % 2 == 0) cout << "|";
                else (mBoard[(y-3) / 2]->at((x-2) / 2)) ? cout << "#" : cout << " ";
            }
            // If not the first row and not in a cell row, print a horizontal line
            else cout << "-";
        }
        // Start a new line 
        cout << endl;
    }
}