#pragma once
#include <vector>
#include <memory>

class Board {
public:
    Board();
    Board(const int aBoardSize);
    ~Board();

    int getBoardSize() const;
    void reviveCell(const int x, const int y);
    int processNextState();

    void printBoard() const;

private:
    std::vector<std::shared_ptr<std::vector<bool>>> mBoard;
    int mBoardSize;
};