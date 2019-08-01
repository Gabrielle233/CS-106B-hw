//n-gram - By Gabrielle Li

//The ngram program takes in a text file, and a number n indicating the size of a frame of words
//and the number of words the user wants to generate and it will generate random words from the text.

#include <iostream>
#include <fstream>
#include <string>
#include "filelib.h"
#include "simpio.h"
#include "console.h"
#include "map.h"
#include "queue.h"
#include "set.h"
#include "stl.h"
#include "random.h"

using namespace std;
Map<Queue<string>, Vector<string> > createDictionary(ifstream& infile);
string pickString(Vector<string>& words);
Queue<string> ramdomPickQueueOfString(Map<Queue<string>, Vector<string> >& map);

int main() {
    //printing the welcoming message.
    cout << "Welcome to CS 106B/X Random Writer ('N-Grams')!" << endl;
    cout << "This program generates random text based on a document."<< endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you."<< endl;
    cout << endl;

    ifstream infile;
    promptUserForFile(infile, "Input file name?");
    Map<Queue<string>, Vector<string> > map = createDictionary(infile);
    cout << endl;


    int numOfWords = getInteger("# of random words to generate (0 to quit)?");
    if (numOfWords == 0) {
        cout << "Exiting." << endl;
        return 0;
    }

    while (numOfWords < 4) {
        cout << "Must be at least 4 words." << endl;
        numOfWords = getInteger("# of random words to generate (0 to quit)?");
        if (numOfWords == 0) {
            break;
        }
    }
    do {
        //Pick a random window to start
        Queue<string> starter = ramdomPickQueueOfString(map);
        int lengthOfWindow = starter.size();
        //Then the initial window will point to a vector a string. We will call
        //pickString to randomly pick a string called choice from the vector.
        //We then dequeue the window and enqueue choice into window, which keeps
        //the window moving. We repeat this process until the desired number of
        //words have been generated.
        for (int i = 0; i < numOfWords; i++) {
                Vector<string> pointedValue = map.get(starter);
                string choice = pickString(pointedValue);
                starter.dequeue();
                starter.enqueue(choice);
                cout << choice << " ";
        }
        cout << "..." << endl;
        cout << endl;
        numOfWords = getInteger("# of random words to generate (0 to quit)?");
        if (numOfWords == 0) {
            break;
        }
        while (numOfWords < 4) {
            cout << "Must be at least 4 words." << endl;
            numOfWords = getInteger("# of random words to generate (0 to quit)?");
            if (numOfWords == 0) {
                break;
            }
        }
    } while (numOfWords != 0);

    cout << "Exiting." << endl;
    return 0;
}

//The purpose of this method is to create and return a dictionary based on the file taking in
//and the input n by the user. It takes in a file. It will first read the file word by word and
//store all the words in a vector. Then the user will be asked to input their
//desired value of n. And then a dictionary will be created. I will provide detailed comments
//step by step which could be found below.
Map<Queue<string>, Vector<string> > createDictionary(ifstream& infile) {
    Queue<string> window;
    Map<Queue<string>, Vector<string> > map;
    Vector<string> fileContent;
    string word;
    //The following steps read the file from word to word and store all words
    //into a vector called fileContent.
    while (infile >> word) {
        fileContent.add(word);
    }
    infile.close();
    //This following steps prompt the user for an N value.
    //If the N is bigger than the number of words in the file
    //or smaller than 1, it will keep reprompting until a valid N is put in.
    int n = getInteger("Value of N?");
    while (!((1 < n) && (n <= fileContent.size() ))) {
        cout << "N cannot be greater than the number of words in the file." << endl;
        n = getInteger("Value of N?");
    }
    //A window is a queue of (n-1) strings that serve as the key. The following steps take care
    //of the wrapping around feature, which means when we are approaching the end of the file, we
    //make sure that it wraps back to the beginning of the file. This is achieved by apending the first
    //(n-1) strings to the back of the vector.
    int lengthOfWindow = n - 1;
    //wrapping around
    for (int i = 0; i < lengthOfWindow; i++) {
        fileContent.add(fileContent[i]);
    }
    //This following steps add value to a key by keep the window moving.
    //The if statement below makes sure that the key only starts to take in values
    //when it reaches the length of a window. When the window is long enough, it will
    //take in the next string as its value. Then we dequeue the first string of the window,
    //enqueue the string after the window, and repeat, which keeps the window moving.
    for (int i = 0; i < fileContent.size(); i++) {
        if (window.size() == lengthOfWindow) {
            map[window].add(fileContent[i]);
            window.dequeue();
        }
        window.enqueue(fileContent[i]);
    }
    return map;
}

//The purpose of this method is to generate a starter window and return it.
//This method will randomly pick a queue of string from a map of queue of string to vector of string (from the key).
//First the method map.keys() stores all the keys in a vector. Then a random number will be generated.
//The random number generated will be in the range of valid.
//Then we return the queue of string whose index is the generated number inside the vector, which ensures that the
//queue generated is random.
Queue<string> ramdomPickQueueOfString(Map<Queue<string>, Vector<string> >& map) {
    cout << "... ";
    Vector<Queue<string> > storeKey;
    storeKey = map.keys();
    int size = storeKey.size();
    Queue<string> starter = storeKey[randomInteger(0, size - 1)];
    return starter;
}

//This method will randomly return a string from a vector of strings. It first takes in a vector of string,
//and then a random number will be generated. The random number generated will be in the range of valid.
//Then we return the string whose index is the generated number inside the vector, which ensures that the
//string generated is random.
string pickString(Vector<string>& words) {
    string chosen;
    int size = words.size();
    chosen = words[randomInteger(0, size - 1)];
    return chosen;
}
