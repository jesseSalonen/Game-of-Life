#include "Board.h"
#include <iostream>

using std::cout;
using std::endl;

Board::Board() : mBoardSize{ 9 } {
    for (int i = 0; i < mBoardSize; i++) {
        mBoard.push_back(std::make_shared<std::vector<bool>>());
        for (int j = 0; j < mBoardSize; j++) {
            mBoard.back()->push_back(false);
        }
    }
}

Board::Board(const int aBoardSize) : mBoardSize{ aBoardSize } {
    for (int i = 0; i < mBoardSize; i++) {
        mBoard.push_back(std::make_shared<std::vector<bool>>());
        for (int j = 0; j < mBoardSize; j++) {
            mBoard.back()->push_back(false);
        }
    }
}

Board::~Board() {
}

int Board::getBoardSize() const {
    return mBoardSize;
}

void Board::reviveCell(const int x, const int y) {
    if (mBoard[y]->at(x) == true) cout << "Cell (" << x << "," << y << ") already alive, no changes made to the board!" << endl;
    else {
        mBoard[y]->at(x) = true;
        cout << "Cell (" << x << "," << y << ") revived!" << endl;
    }
}

int Board::processNextState() {
    std::vector<std::shared_ptr<std::vector<bool>>> newBoard;
    int neighborCount = 0;
    int changeCount = 0;

    for (int i = 0; i < mBoardSize; i++) {
        newBoard.push_back(std::make_shared<std::vector<bool>>());
        for (int j = 0; j < mBoardSize; j++) {
            for (int y = i - 1; y <= i + 1; y++) {
                for (int x = j - 1; x <= j + 1; x++) {
                    if (y >= 0 && x >= 0 && y < mBoardSize && x < mBoardSize && (y != i || x != j)) {
                        if (mBoard[y]->at(x) == true) neighborCount++;
                    }
                }
            }

            if (mBoard[i]->at(j) == true) {
                if (neighborCount != 2 && neighborCount != 3) {
                    newBoard.back()->push_back(false);
                    changeCount++;
                }
                else newBoard.back()->push_back(true);
            }
            else {
                if (neighborCount == 3) {
                    newBoard.back()->push_back(true);
                    changeCount++;
                }
                else newBoard.back()->push_back(false);
            }
            neighborCount = 0;
        }
    }
    mBoard = newBoard;

    return changeCount;
}

void Board::printBoard() const {
    for (int i = 0; i < mBoardSize * 2 + 2; i++) {
        for (int j = 0; j < mBoardSize * 2 + 3; j++) {
            // If printing the first row, print column numbers
            if (i == 0) {
                if ((j % 2 != 0 && j >= 3) || int((j-2) / 2) > 10) {
                    cout << int((j-2) / 2);
                    if (int((j-2) / 2) < 10) cout << " ";
                    j++;
                }
                else cout << " ";
            }
            // If not the first row and printing the first two symbols, print row numbers
            else if (j <= 1) {
                if (i % 2 == 0) {
                    cout << int((i-2) / 2);
                    if (int((i-2) / 2) <= 9) cout << " ";
                    j++;
                }
                else cout << " ";
            }
            // If not the first row and is a cell row, print either a cell wall or a cell value
            else if (i % 2 == 0) {
                if (j % 2 == 0) cout << "|";
                else (mBoard[(i-2) / 2]->at((j-2) / 2)) ? cout << "#" : cout << " ";
            }
            // If not the first row and not in a cell row, print a horizontal line
            else cout << "-";
        }
        // Start a new line 
        cout << endl;
    }
}