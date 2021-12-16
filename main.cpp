#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "Board.h"

using std::cin;
using std::cout;
using std::endl;

int checkInput(const std::string& number, const int min, const int max, bool& exitApp);

int main() {
    std::string boardSize, xAxis, yAxis, nextStep, continueGame;
    bool exit = false;
    bool initializingDone;
    int initialCellCount, changedCellsAmount, stepCount;

    do {
        cout << "Welcome to the Game of Life!" << endl;
        cout << "Please follow the instructions and the game will simulate a single Game of Life" << endl;
        cout << "To exit the application, simply type 'q' in an input field\n" << endl;
        // Validate the input
        do {
            cout << "Please choose a size for the board (5-100):";
            getline(cin, boardSize);
        } while (checkInput(boardSize, 5, 100, exit) < 0);

        if (!exit) {
            Board board{ stoi(boardSize) };
            initialCellCount = 0;
            initializingDone = false;
            stepCount = 0;
            nextStep = "";

            while (!exit || initializingDone ) {
                cout << "This is the game board" << endl;
                board.printBoard();
                cout << "All the cells are 'dead' by default" << endl;
                cout << "Please insert 'alive' cells to initialize a starting state" << endl;
                cout << "When done with inputting alive cells, type 'done'\n" << endl;

                do {
                    cout << "Give cell's x-axis number (horizontal, range 0-" << board.getBoardSize() - 1 << "):";
                    getline(cin, xAxis);
                    if (xAxis == "done") {
                        if (initialCellCount >= 1) initializingDone = true;
                        else cout << "Add at least one cell before continuing!" << endl;
                    }
                // If initializing done or input is a number in range or user wants to exit = go to next step
                } while ((!initializingDone || xAxis != "q") && checkInput(xAxis, 0, board.getBoardSize() - 1, exit) < 0 );
                
                // If 
                if (!initializingDone && !exit) {
                    do {
                        cout << "Then give the y-axis number (vertical, range 0-" << board.getBoardSize() - 1 << "):";
                        getline(cin, yAxis);
                        if (checkInput(yAxis, 0, board.getBoardSize() - 1, exit) > 0) {
                            board.reviveCell(stoi(xAxis), stoi(yAxis));
                            initialCellCount++;
                        }
                    } while (checkInput(yAxis, 0, board.getBoardSize() - 1, exit) < 0 || !exit);
                }
            }
            // Clear the console
            cout << "\033[2J\033[1;1H";

            do {
                cout << "Step count: " << stepCount << "\n" << endl;

                board.printBoard();
                if (nextStep == "") {
                    cout << "This is the board after " << stepCount << " steps!" << endl;
                    cout << "To continue to the next state, press enter" << endl;
                    cout << "To automatically progress through the game, type 'auto'" << endl;

                    do {
                        cout << "Command:";
                        getline(cin, nextStep);
                        if (nextStep != "" && nextStep != "auto") cout << "Invalid input! Press either only enter, or type 'auto' and press enter" << endl;
                    } while (nextStep != "" && nextStep != "auto");
                }
                changedCellsAmount = board.processNextState();
                stepCount++;
                if (nextStep == "auto") {
                    // Wait one second before printing next state
                    std::chrono::seconds duration(1);
                    std::this_thread::sleep_for(duration);
                }
                // Clear the console
                cout << "\033[2J\033[1;1H";
            } while (changedCellsAmount != 0);
            cout << "Game completed!" << endl;
            cout << "Steps: " << stepCount  << endl;
            board.printBoard();
            cout << endl;
            do {
                cout << "To play again, press 'a', otherwise exit with 'q'";
                getline(cin, continueGame);
                if (continueGame != "q" && continueGame != "a") cout << "Invalid input!" << endl;
            } while (continueGame != "q" && continueGame != "a");
            if (continueGame == "q") exit = true;
        }
        
    } while (!exit);

    return 0;
}

// function for validating the user input
int checkInput(const std::string& number, const int min, const int max, bool& exitApp) {
    // Check if number == "q", user wants to exit the application
    if (number == "q") {
        exitApp = true;
        return 0;
    }
    // Check if empty or not a number, return -1
    else if (number.empty() || number.find_first_not_of("0123456789") != std::string::npos) {
        cout << "Invalid input! Make sure to type only numbers!" << endl;
        return -1;
    }
    // Check if not in given range, return 0
    else if (stoi(number) < min || stoi(number) > max) {
        cout << "Invalid input! Make sure the number is in range!" << endl;
        return -2;
    }
    // If ok, return 1
    else return 1;
}