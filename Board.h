#pragma once
#include <vector>
#include <memory>

class Board {
public:
    Board();
    Board(const int aBoardSize);
    ~Board();

private:
    std::vector<std::vector<std::shared_ptr<bool>>> mBoard;
    int mBoardSize;
};