#include "Board.h"

Board::Board() : mBoardSize{ 9 } {
    for (int i = 0; i < mBoardSize; i++) {
        mBoard.push_back({});
        for (int j = 0; j < mBoardSize; j++) {
            mBoard.back().push_back(std::make_shared<bool>(false));
            
        }
    }
}