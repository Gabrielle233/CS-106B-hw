//Gabrielle Li, CS 106B, summer 2019
//Add des

#include <iostream>
#include <fstream>
#include <string>
#include "filelib.h"
#include "simpio.h"
#include "console.h"
#include "queue.h"
#include "set.h"
#include "stl.h"
#include "strlib.h"

using namespace std;

Set<string> buildDictionary ();
bool findNeighbor(Queue <Stack<string> >& queue, const Set<string>& dictionary, const string& start, const string& goal);
void printStack(Stack<string>& stack);


int main() {
    cout << "Welcome to CS 106B/X Word Ladder!" << endl;
    cout << "Please give me two English words, and I will convert the" << endl;
    cout << "first into the second by modifying one letter at a time." << endl;
    cout << endl;
    Set<string> dictionary = buildDictionary();
    string word1;
    string word2;
    do {
        word1 = getLine("Word 1 (or Enter to quit): ");
        word2 = getLine("Word 2 (or Enter to quit): ");
        while (word1.size() != word2.size()) {
            cout << "The two words must be the same length." << endl;
            cout << endl;
            word1 = getLine("Word 1 (or Enter to quit): ");
            word2 = getLine("Word 2 (or Enter to quit): ");
    }
        while (word1 == word2) {
            cout << "The two words must be different." << endl;
            cout << endl;
            word1 = getLine("Word 1 (or Enter to quit): ");
            word2 = getLine("Word 2 (or Enter to quit): ");
    }
        while (!(dictionary.contains(word1) && dictionary.contains(word2))) {
            cout << "The two words must be found in the dictionary." << endl;
            cout << endl;
            word1 = getLine("Word 1 (or Enter to quit): ");
            word2 = getLine("Word 2 (or Enter to quit): ");
    }
    Stack<string> stack;
    Queue<Stack<string> > queue;
    stack.push(word2);
    queue.enqueue(stack);
    findNeighbor(queue, dictionary, word2, word1);
    word1 = getLine("Word 1 (or Enter to quit): ");
    if (word1 == "") {
        break;
    }
    word2 = getLine("Word 2 (or Enter to quit): ");
    } while (word1 != "" || word2 != "");

    cout << "Have a nice day." << endl;

    return 0;
}

Set<string> buildDictionary() {
    ifstream infile;
    Set<string> dictionary;
    promptUserForFile(infile, "Dictionary file name:");
    cout << endl;
    string word;
    while (infile >> word) {
        dictionary.add(word);
    }
    return dictionary;
}

bool findNeighbor(Queue <Stack<string> >& queue, const Set<string>& dictionary, const string& start, const string& goal) {
    while (!queue.isEmpty()) {
        Stack<string> stack = queue.dequeue();
        string top = stack.peek();
        for (int i = 0; i < top.size(); i++) {
            for (char ch = 'a'; ch <= 'z'; ch++) {
                Stack<string> copy = stack;
                string word = top;
                word[i] = ch;

                if (word.compare(top) == 0) {
                    continue;
                }
                else if (dictionary.contains(word)) {
                    copy.add(word);
                    queue.enqueue(copy);
                    if (word.compare(goal)==0) {
                        cout << "A ladder from " << start << " back to " << goal << ": " << endl;
                        printStack(copy);

                        return true;
                }
                }
            }
        }

    }
    cout << "No word ladder found from " << start << " back to " << goal << "." << endl;
    cout << endl;
    return false;
}

void printStack(Stack<string>& stack) {
    Stack<string> newStack;
    while (!stack.isEmpty()) {
    newStack.push(stack.pop());
    }
    while (!newStack.isEmpty()) {
    cout << newStack.pop() << " ";
    }
    cout << endl;
    cout << endl;
}
