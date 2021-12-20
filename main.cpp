#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "Board.h"

using std::cin;
using std::cout;
using std::endl;
// Function for checking the user input
int checkInput(const std::string &number, const int min, const int max, bool &exitApp);
// main handles the user interaction and passes operations to the Board-class which does the game calculations
/*
string boardSize: size of the play area, (5-100)
string xAxis, yAxis: variables for user input on coordinates
string nextStep: variable stating the type of advancement in the game, either automatic or user-controlled
string continueGame: variable for user's input on whether the game should continue or end
bool exit: bool for exit checking, if true, stop executing the code and exit the app
bool initializingDone: bool for checking if the user is happy with the starting state of the game
int initialCellCount: amount of cells the user has inserted in to the starting state, min = 1
int changedCellsAmount: counter for changed cells in a single state change, if 0, game is completed
int stepCount: counter for steps in a single game
int userInputPlaceHolder: placeholder for checkInput's return value.
    Used when same input would otherwise be checked multiple times, possibly resulting in multiple same error messages
*/
int main() {
    std::string boardSize, xAxis, yAxis, nextStep, continueGame;
    bool exit = false;
    bool initializingDone;
    int initialCellCount, changedCellsAmount, stepCount, userInputPlaceholder;
    // continue looping the game until user wants to exit
    do {
        cout << "Welcome to the Game of Life!" << endl;
        cout << "Please follow the instructions and the game will simulate a single Game of Life" << endl;
        cout << "To exit the application, simply type 'q' in an input field\n" << endl;
        // Validate the input
        do {
            cout << "Please choose a size for the board (5-100):";
            getline(cin, boardSize);
        } while (checkInput(boardSize, 5, 100, exit) < 0);

        // Create a game if input ok/user wants to continue
        if (!exit) {
            // Initialize the variables for a new game
            Board board{stoi(boardSize)};
            initialCellCount = 0;
            initializingDone = false;
            stepCount = 0;
            nextStep = "";
            // Keep asking for new cells as a starting state. Stop when user is done, or wants to exit the game
            while (!exit && !initializingDone) {
                cout << "This is the game board" << endl;
                board.printBoard();
                cout << "All the cells are 'dead' by default" << endl;
                cout << "Please insert 'alive' cells to initialize a starting state" << endl;
                cout << "When done with inputting alive cells, type 'done'\n" << endl;
                // Ask for x-axis number, also check for 'done' meaning user is ready
                do {
                    cout << "Give cell's x-axis number (horizontal, range 0-" << board.getBoardSize() - 1 << "):";
                    getline(cin, xAxis);
                    if (xAxis == "done") {
                        if (initialCellCount >= 1) initializingDone = true;
                        else cout << "Add at least one cell before continuing!" << endl;
                    }
                    // If initializing isn't done and user don't want to exit and input is not a number in range = ask input again
                } while (!initializingDone && xAxis != "q" && checkInput(xAxis, 0, board.getBoardSize() - 1, exit) < 0);

                // If initializing not done and user wants to continue, ask for y-axis number
                if (!initializingDone && !exit) {
                    do {
                        cout << "Then give the y-axis number (vertical, range 0-" << board.getBoardSize() - 1 << "):";
                        getline(cin, yAxis);
                        // Save the input to a placeholder, otherwise multiple function calls = multiple error messages on same input
                        userInputPlaceholder = checkInput(yAxis, 0, board.getBoardSize() - 1, exit);
                        // If input ok, 'revive' cell with given coordinates
                        if (userInputPlaceholder > 0) {
                            // Clear the console by moving the text out of sight, and then re-positioning the cursor to the top-left corner
                            cout << "\033[2J\033[1;1H";
                            board.reviveCell(stoi(xAxis), stoi(yAxis));
                            initialCellCount++;
                        }
                        // If input not ok and user wants to continue, keep asking for valid input
                    } while (userInputPlaceholder < 0);
                }
            }
            // Clear the console
            cout << "\033[2J\033[1;1H";
            if (!exit) {
                // Start playing the game. End game loop when no more cells change state
                do {
                    cout << "Step count: " << stepCount << "\n" << endl;

                    board.printBoard();
                    if (nextStep == "") {
                        cout << "This is the board after " << stepCount << " steps!" << endl;
                        cout << "To continue to the next state, press enter" << endl;
                        cout << "To automatically progress through the game, type 'auto'" << endl;
                        // Ask for a command to step to next state
                        do {
                            cout << "Command:";
                            getline(cin, nextStep);
                            if (nextStep != "" && nextStep != "auto") cout << "Invalid input! Press either only enter, or type 'auto' and press enter" << endl;
                        } while (nextStep != "" && nextStep != "auto");
                    }
                    // Do calculations, and save the amount of changes made
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
                cout << "Steps: " << stepCount << endl;
                board.printBoard();
                cout << endl;
                // Ask if the user wants to play another game
                do {
                    cout << "To play again, press 'a', otherwise exit with 'q'";
                    getline(cin, continueGame);
                    if (continueGame != "q" && continueGame != "a") cout << "Invalid input!" << endl;
                } while (continueGame != "q" && continueGame != "a");
                if (continueGame == "q") exit = true;
            }
        }

    } while (!exit);

    return 0;
}

// function for validating the user input
int checkInput(const std::string &number, const int min, const int max, bool &exitApp)
{
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