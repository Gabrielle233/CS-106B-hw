//Gabrielle Li, CS 106B, Summer 2019
//Assignment 3: Recursion (B, Grammar Solver)
//The grammarsolver program generates a number of grammar structures according to the rules
//in the input file. It takes in an input file, a string symbol that is to be generated, and
//an int representing how many times the symbol is going to be generated. This program has four
//helper function: readInput reads, processes, and stores the input file; generateElem generates
//element according to the rules, isTerminal determines if a symbol is a terminal or not according
//to the rules, and randomPick randomly picks a string from a vector of strings.
//Resources: assignment spec, Stanford C++ library, LaIR debugging help
//This program is created by myself.

#include "grammarsolver.h"
#include <fstream>
//#include "console.h"
//#include "filelib.h"
//#include "simpio.h"
//#include "string.h"
#include "strlib.h"
//#include "random.h"
//#include "filelib.h"
#include "map.h"
using namespace std;

//helper function prototype
Map<string, Vector <string> > readInput(istream& input);
string generateElem(Map<string, Vector<string> >& store, string& symbol);
bool isTerminal(Map<string, Vector <string> >& store, string& symbol);
string randomPick(Vector<string>& v);

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector <string> > store = readInput(input);
    Vector<string> result;
    for (int i = 0; i < times; i++) {
    result.add(generateElem(store, symbol));
    }
    return result;
}

//The purpose of this function is to read an input file and to store grammar rules in a map.
//This function takes in a file and returns a map (key: string; value: vector of string).
//The file passed in will be read line by line. Each line will be splitted into two strings
//(one part before the "::=", another part after) and they will bestored in a vector called split.
//Then the second part of split (the actual rule part) will be splitted again by "|" to store individual rules
//in a vector called splitAgain. A map called store will then be generated to store the rules.
//The first string in the vector will be stored as key,
//and the vector of strings created after the second split will be corresponding value. Store will be returned.
Map<string, Vector <string> > readInput(istream& input) {
    string line;
    Map<string, Vector <string> > store;
    while (getline(input, line)) {
    Vector<string> split = stringSplit(line, "::=");
    Vector<string> splitAgain = stringSplit(split[1], "|");
    store.put(split[0], splitAgain);
    }
    return store;
}

//This fuction generates elements using recursion. It takes in the map that stores tha rules, a symbol, and an integer
//representing how many times the symbol is going to be generated.
//Detailed description on how recursion is implemented is written above each case.
string generateElem(Map<string, Vector<string> >& store, string& symbol) {
    //Base case: if the symbol is a terminal, do nothing but adding the symbol to the result string.
        string result;
        if (symbol == "") {
            throw "This symbol is not valid.";
        }
        else if (isTerminal(store, symbol)) {
            result = result + " " + symbol;
        }
        //Recursive case: if the symbol is a terminal, we first access its rules from the map. As each vector between the pipes
        //(we already splitted them) represent an expansion rule and all of them are interchangable, this function will randomly
        //pick a rule to implement. Then it will check if the rule has multiple parts separated by " ". If it does, for each part,
        //the function calls itself recursively to generate that part.
        else {
            Vector<string> rules = store[symbol];
            string choice = randomPick(rules);
            choice = trim(choice);
            if (stringContains(choice, " ")) {
                Vector<string> finalSplit = stringSplit(choice, " ");
                for (string element: finalSplit) {
                result += generateElem(store, element);
                }
            } else {
                result += generateElem(store, choice);
            }
        }
    return result;
}

//This function is used to determine if a symbol put in is terminal or not based on the map
//of grammar rules we created. It takes in a map that stores the rules and a symbol, and it
//returns true if the symbol is a terminal, false if it is not.
//First, call keys() on the map and all the keys will be stored in vector storeKey. For each
//element in storeKey, we check if they equal to the symbol (as stated on the spec, terminals
//only appear on the right side of ":==", which means they do not appear in the keys)
bool isTerminal(Map<string, Vector <string>  >& store, string& symbol) {
    Vector<string> storeKey = store.keys();
    for (string  element:storeKey) {
        if (equalsIgnoreCase(element, symbol)) {
            //Check for exception: whether more line one line has the same grammar
            for (int i = 0; i < storeKey.size(); i ++) {
                for (int j = storeKey.size() - 1; j > i; j--) {
                    if (storeKey[i] == storeKey[j]) {
                        throw "More than one line of the same non-terminal grammar.";
                    }
                }
            }
            return false;
        }
    }
    return true;
}

//This function randomly picks an element (string) from a vector of string.
//It takes in a vector of string and returns the selected string from the vector.
//A random number will first be generated, and then the element whose index is the
//randomly generated number will be returned. This ensured that the string is
//randomly chosen from the vector.
string randomPick(Vector<string>& v) {
    int size = v.size();
    return v[randomInteger(0, size - 1)];
}
