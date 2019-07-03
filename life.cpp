// Gabrielle Li, CS 106B
// For this assignment I implement a simplified simulation of "Game of Life". In this game, the program
//takes in a file that the user put in, read the file and use it as the "world" for the game. Then the
//user is able to give a series of command such as see next generation of world, skip through multiple
//generations, or quit. All of the methods I used in this game come from either class material or stanford
//library online. I got help from LaIR Tuesday night for some debugging. Other than that, all of the codes
//are produced solely by me. Note: my debugging file is under "Life" << "Other files" << "res".

#include <iostream>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "grid.h"
#include "string.h"
#include "lifegui.h"

using namespace std;

Grid<char> getMyGrid();
void printMyGrid(Grid<char>& grid);
string playGame(Grid<char>& currentGrid);
void nextGeneration(Grid<char>& currentGrid);
void animationResult(Grid<char>& currentGrid, int& userNum);

int main() {
    cout << "Welcome to the CS 106B/X Game of Life!" << endl;
    cout << "This program simulates the lifecycle of a bacterial colony." << endl;
    cout << "Cells (X) live and die by the following rules: " << endl;
    cout << "* A cell with 1 or fewer neighbors dies." << endl;
    cout << "* Locations with 2 neighbors remain stable." << endl;
    cout << "* Locations with 3 neighbors will create life." << endl;
    cout << "* A cell with 4 or more neighbors dies." << endl;
    cout << endl;

    LifeGUI::initialize();

    Grid<char> currentGrid = getMyGrid();

    while (playGame(currentGrid) == "quit") {
       string userQuit = getLine("Load another file? (y/n)");

       while (userQuit[0] != 'Y' && userQuit[0] != 'N' && userQuit[0] != 'y' && userQuit[0] != 'n') {
           cout << "Please type a word that starts with 'Y' or 'N'." << endl;
           userQuit = getLine("Load Another File (y/n)?");
       }
       if (userQuit[0] == 'y' || userQuit[0] == 'y') {
           currentGrid = getMyGrid();
       }
       else if (userQuit[0] == 'n' || userQuit[0] == 'N') {
           break;
       }
    }
    LifeGUI::shutdown();
    cout << "Have a nice Life!" << endl;
    return 0;
}

//The function getMyGrid print out rules of this game
//and repeatedly prompt the player to enter a grid input
//file name until one that exists is entered. Then it
//records the number of rows and columns and create a
//grid named "myWorld" like the grid in the input file.
Grid<char> getMyGrid() {
    ifstream infile;
    promptUserForFile(infile, "Grid input file name?");    //do we really need to check since promptUserForFile already did the job???
    string line;

    getline(infile, line);
    int numOfRow = stringToInteger(line);
    getline(infile, line);
    int numOfColumn = stringToInteger(line);
    LifeGUI::resize(numOfRow, numOfColumn);

    Grid<char> myWorld(numOfRow, numOfColumn);
    for (int r = 0; r < numOfRow; r++) {
        getline(infile, line);
        cout << line << endl;

        for (int c = 0; c < numOfColumn; c++) {
                myWorld[r][c] = line[c];
                if (myWorld[r][c] == 'X') {
                    LifeGUI::fillCell(r, c);
                }
            }
        }
    LifeGUI::repaint();
    return myWorld;
}

//Function printMyGrid is called to print out a grid to the console.
//This function takes in a grid and will reiterate through each grid inside to print them.
//When a row is finished, another row will be printed after a line break.
void printMyGrid (Grid<char>& grid) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            cout << grid[r][c];
        }
        cout << endl;
    }
}

//Function nextGeneration is called to construct and print out next generation.
//It takes in our current grid.
//It first makes a copy a our current Grid and create a new, temporary grid.
//Then it reiterate through every cell to check how many neighbors it has.
//After getting a value of number of neighbors for each grid,
//it will make changes to the grid in the new grid we created,
//according to the number of neighbors each cell has.
//To finish up, it will update our current cell by passing each grid of the new grid
//back to the original current grid.
void nextGeneration(Grid<char>& currentGrid) {
    LifeGUI::clear();
    Grid<char> newGrid(currentGrid.numRows(),currentGrid.numCols());

//The reason why I created a new copied grid instead of making edits on the old grid:
//once I go through every cell on the old grid to make changes, the cell coming after that
//will be impacted because the change I made might change the number of neighbors of cells
//coming after. Therefore, refering neighbor on the old grid and making changes to a new grid
//ensures cell changes together at a tick.
    for (int r = 0; r < currentGrid.numRows(); r++) {
        for (int c = 0; c < currentGrid.numCols(); c++) {
            newGrid[r][c] = currentGrid[r][c];
            int countCell = 0;
            //The following double for loop go through all cells in the grid and count the number
            //of neighbors it has. The count is stored in a variable called countCell.
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    //We want to check the adjacent eight cells while skipping the cell itself.
                    if (i == 0 && j == 0) {
                        continue;
                    }
                    //Variable dr and dc take care of the "wrapping around" feature.
                    //If some neighboring cell's index is out of bound, dr and dc will
                    //be correspondingly modified (+/- #columns or #rows) to reach the other
                    //end of the grid.
                    int dr = r + i;
                    if (dr < 0) {
                        dr += currentGrid.numRows();
                    }
                    else if (dr > currentGrid.numRows() - 1) {
                        dr -= currentGrid.numRows();
                    }
                    int dc = c + j;
                    if (dc < 0) {
                        dc += currentGrid.numCols();
                    }
                    else if (dc > currentGrid.numCols() - 1) {
                        dc -= currentGrid.numCols();
                    }

                    if (currentGrid[dr][dc] == 'X') {
                        countCell ++;
                    }
                }
            }
            if (countCell < 2 || countCell >= 4) {
                newGrid[r][c] = '-';
            }
            else if (countCell == 2 && currentGrid[r][c] == 'X') {
                LifeGUI::fillCell(r, c);
            }
            else if (countCell == 3) {

                    newGrid[r][c] = 'X';
                    LifeGUI::fillCell(r, c);
            }
        }
    }
    for (int r = 0; r < currentGrid.numRows(); r++) {
        for (int c = 0; c < currentGrid.numCols(); c++) {
            currentGrid[r][c] = newGrid[r][c];
        }
    }
    printMyGrid(currentGrid);
    LifeGUI::repaint();
}

//Function animationResult is called when the player choose "animation".
//It takes in the current grid and an number (how many generations of change the user wants to see)
//and it will make use of the function "nextGeneration" to go through however many generation the
//user types in.
void animationResult(Grid<char>& currentGrid, int& userNum) {
    for (int i = 0; i < userNum; i++)
    {
        clearConsole();
        nextGeneration(currentGrid);
        pause(100);
    }
}

//This is the main function that guide the user to play the game.
//It takes in a grid and the grid will be the grid which the user play with.
//When user's choice is not to quit, the game keeps going.
//When user's choice is to quit, we exit this function and go back to main
//to determine whether to play again.
string playGame(Grid<char>& currentGrid){
    string userMove = " ";
    while (userMove != "q") {
        userMove = getLine("a)nimate, t)ick, q)uit?");
        while (userMove != "a" && userMove != "t"  && userMove != "q" ) {
            cout << "Invalid choice; please try again." << endl;
            userMove = getLine("a)nimate, t)ick, q)uit?");
        }

        if (userMove == "t") {
            nextGeneration(currentGrid);
        }

        else if (userMove == "a") {
            int userFrames = getInteger("How many frames?");
            animationResult(currentGrid, userFrames);
        }
    } while (userMove != "q");
    return "quit";
}
