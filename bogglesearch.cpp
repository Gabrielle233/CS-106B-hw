/*
 * Doctors Without Orders - By Gabrielle Li and Noopur Bhatt
 * Partner SUNets: gabbyli1; nbhatt
 * CS 106B - Summer 2019
 * Due July 25, 2019
 *
 * The main objective of this program is to take 1) takes in an input word and to
 * see if the word exists validly on board. 2) use recursive backtracking to find
 * all words on a board
 *
 * Sources Used: Spec; Lecture Slides / Code Day #1 - #15; LAiR
 */


#include "bogglegui.h"
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
using namespace std;

bool isAdjacent(Grid<char>& board, int& row, int& col, string& toGo, Grid<bool> mark);
bool humanHelper(Grid<char>& board, Lexicon& dictionary, string& word, string& toGo, int& row, int& col);
void computerHelper (Grid<char>& board, string soFar, Lexicon& dictionary,
                     Grid<bool>& mark, Set<string>& allWords, int& r, int& c, Set<string>& humanWords);
int getPoints(string& word);

//HumanWordSearch took in an input word, a dictionary, and a board and return
//a bool (true if the word could be found on the boggle board)
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    string toGo = word;
    //simple case: check the size of the word
    if (word.size() < BoggleGUI::MIN_WORD_LENGTH) {
        return false;
    }
    //simple case: check if the word is in the dictionary
    else if (!dictionary.contains(word)) {
        return false;
    }
    int row;
    int col;
    return humanHelper(board, dictionary, word, toGo, row, col);
}

//humanhelper is a helper function of humanWordSearch. It takes in every parameter taked in by humanWordSearch,
//as well as a string toGo and two integer row and col to represent the location of the beginning letter of toGo.
//It returns a boolean if the word can be found on the board.
bool humanHelper(Grid<char>& board, Lexicon& dictionary, string& word, string& toGo, int& row, int& col) {
    Grid<bool> mark(board.numRows(), board.numCols());

    char start = toGo[0];
    toGo = toUpperCase(toGo);
    Vector<Vector<int> > storeStart;
//store all letters on the board that could possibly be the beginning letter of the input word
    for (int c = 0; c < board.numCols(); c++) {
        for (int r = 0; r < board.numRows(); r++) {
            Vector<int> toStart;
            if (toGo == word && toGo[0] == board.get(r, c)) {
                toStart.add(r);
                toStart.add(c);
                storeStart.add(toStart);
            }
        }
    //everytime as we recursive, we erase the first letter of toGo
    toGo = toGo.substr(1);
    //base case: if toGo is empty, return true
    if (toGo == "") {
        return true;
        }
    //base case: if the first letter of word is not on board, return false immediately
    if (storeStart.isEmpty()) {
        return false;
    }
    //recursive case: we check if the first letter of toGo is on board and is adjacent to
    //the location passed in. If so, we will call humanHelper on the updated toGo and updated
    //row and col.
    for (int i = 0; i < storeStart.size(); i++) {

    int row = storeStart[i] [0];
    int col = storeStart[i] [1];
    BoggleGUI::setHighlighted(row, col, false);
    //choose
    if (isAdjacent(board, row, col, toGo, mark)) {
        mark[row][col] = true;
    }
    //explore
    if (humanHelper(board, dictionary, word, toGo, row, col) == true) {
            BoggleGUI::scorePointsHuman(getPoints(word));
            return true;
            }
    //unchoose
    toGo = start + toGo;
    BoggleGUI::setHighlighted(row, col, false);
    mark[row][col] = false;
        }
    }
    BoggleGUI::clearHighlighting();
    return false;
}

//This function takes in a board, two integers representing row and col of a grid on the board.
//a string that is the (part of) the word we are looking for, and a grid of bool mark. It will return a boolean.
//If the beginning of string toGo is in the eight neighbor grids of the location (row, col) given, and the location is
//not already explored (marked true),
//we modify the location to be the location to be the location of the beginning of toGo and return true.
//Otherwise, return false.
bool isAdjacent(Grid<char>& board, int& row, int& col, string& toGo, Grid<bool> mark) {
    for (int r = -1; r < 2; r++) {
        for (int c = -1; c < 2; c++) {
            if (r == 0 && c == 0) {
                continue;
            }
            if (board.inBounds(row + r, col + c) && toGo[0] == board[row + r][col + c] && mark[row + r][col + c] == false) {
                BoggleGUI::setHighlighted(row + r, col + c, true);
                row += r;
                col += c;
                return true;
            }
        }
    }
    return false;
}

//computerWordSearch performs exhaustive search on all words that could be formed on the board and
//will add the ones human didn't find to a set. The set will be returned.
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> allWords;
    string soFar = "";
    Grid<bool> mark(board.numRows(), board.numCols());
    //mark is a board of the same size of board but filled with boolean to represent if one grid is
    //explored or not.
    mark.fill(false);
    for (int r = 0; r < board.numRows(); r++) {
        for (int c = 0; c < board.numCols(); c++) {
            computerHelper(board, soFar, dictionary, mark, allWords, r, c, humanWords);
            //after explore every starting grid, reset the state board to false and soFar to empty string.
            mark.fill(false);
            soFar = "";
        }
    }
    return allWords;
}

//computerHelper is a helper function that takes in everything computerWordSearch takes in put two integers
//to represent a grid on board, the mark grid to represent the state of each grid (explored or not) in one
//search, and a set of string to be added with words found by the computer.
void computerHelper (Grid<char>& board, string soFar, Lexicon& dictionary,
                     Grid<bool>& mark, Set<string>& allWords, int& r, int& c, Set<string>& humanWords) {
    if(mark.inBounds(r, c) && mark[r][c] == false) {
        //if soFar isn't a prefix in dictionary, we skip and perform other search.
        if (!dictionary.containsPrefix(soFar)) {
            return;
        } else {
            //explore
            soFar += board[r][c];
            //add possibly valid word to list
            if (soFar.size() >= BoggleGUI::MIN_WORD_LENGTH && dictionary.contains(soFar)
                    && (!humanWords.contains(soFar)) &&(!allWords.contains(soFar))) {
                allWords.add(soFar);
                BoggleGUI::scorePointsComputer(getPoints(soFar));
            }
            //explore the eight neighbors of a grid. Do recursive backtraking on those grid.
            mark[r][c] = true;
            for (int i = r - 1; i < r + 2; i++) {
                for (int j = c - 1; j < c + 2; j++) {
                    if (i == r && j == c) {
                        continue;
                    }
                    computerHelper(board, soFar, dictionary, mark, allWords, i, j, humanWords);
                }
            }
        }
    }
}

//This function takes in a string word and returns the corresponding points that it earns according to the rule.
int getPoints(string& word) {
    int point = 0;
    int length = word.size();
    if (length <= 4) {
        point = 1;
    }
    else if (length == 5) {
        point = 2;
    }
    else if (length == 6) {
        point = 3;
    }
    else if (length == 7) {
        point = 5;
    }
    else if (length >= 8) {
        point = 11;
    }
    return point;
}
