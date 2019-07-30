// Gabrielle Li, CS106B, Due 08/01/2019
//Patient Queue (.h)
//In this assignment, I implement patient queue using min/max heap. In this .h file,
//the user will be able to know what variables and methods this class has and their
//individual usage.

#ifndef _patientqueue_h
#define _patientqueue_h

#include <iostream>
#include <string>
#include "patient.h"
using namespace std;

class PatientQueue {
public:
    //This function initialize our patient queue to be stored in an arraylist of size 20.
    PatientQueue(bool isMinHeap);

    //This function runs when the user chooses to exit to free memory that is no longer used.
    ~PatientQueue();

    //This function takes in a string (the name of the element of which the user wants to change
    //the priority) and a new priority) and it updates the priority of that element.
    void changePriority(string value, int newPriority);

    //This function clears all elements in the patient queue. (The array still exists and is ready
    //to be reused)
    void clear();

    //empty
    void debug();

    //This function dequeues the element with the lowest/highest priority and restore the remaining queue
    //according to the rule (depending on min/max heap using) and return the element name.
    string dequeue();

    //This function enqueues an element (intakes name and priority) and put it in the right place
    //(depending on min/max heap using).
    void enqueue(string value, int priority);

    //This function returns a boolean (true: empty queue; false: non-empty).
    bool isEmpty() const;

    //This function returns the name the element with the lowest/highest priority
    //(depending on min/max heap using)
    string peek() const;

    //This function returns the priority the element with the lowest/highest priority
    //(depending on min/max heap using)
    int peekPriority() const;

    //This function returns the size of the queue.
    int size() const;

    //This function overloads operator "<<": add the whole queue to the output file and return it.
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

private:
    Patient *_patientQueue;
    int _size;
    int _capacity;
    bool _min;

    //Description of private methods see .cpp file.
    int getIndex(string name) const;
    void swap(int index1, int index2);
    Patient getMaxPriority(int index1, int index2);
    Patient getMinPriority(int index1, int index2);
    void bubbleDown(int index);
    void bubbleUp(int index);
    void resize();
};

#endif
