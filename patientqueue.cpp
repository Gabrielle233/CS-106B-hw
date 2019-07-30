// Gabrielle Li, CS106B, Due 08/01/2019
//Patient Queue (.cpp)
//In this assignment, I implement patient queue using min/max heap. In this .cpp file,
//I implemented all the methods in the patient queue class and commented each one of them.
//resources: spec, class slides, LaIR, Tyler's office hour

#include "patientqueue.h"
#include <algorithm>

//This method create an arraylist on the heap which is used to store the patient queue.
//The initial size of the patient queue is 0. The capacity of the arraylist on the heap is 30.
PatientQueue::PatientQueue(bool isMinHeap) {
    _capacity = 20;
    _patientQueue = new Patient[_capacity];
    _size = 0;
    _min = isMinHeap;
}

//This method free the memory of the patient queue from the heap when the user finishes using it.
PatientQueue::~PatientQueue() {
    delete[] _patientQueue;
}

//This private helper function search for index of a given name. It takes in a string name, then it
//iterates over the array to find the index of the corresponding name and returns it. If there are
//multiple element, this function returns the first one with the name. If the name isn't find, this
//function gives an error message.
int PatientQueue::getIndex(string name) const {
    for (int i = 1; i < _size + 1; i++) {
        if (_patientQueue[i].name == name) {
        return i;
        }
    }
    throw "invalid name.";
}

//This private helper function modifies the patient queue by swapping two element of given index.
//It takes in two integer representing the two indecies and swap the elements stored.
void PatientQueue::swap(int index1, int index2) {
    Patient store = _patientQueue[index1];
    _patientQueue[index1] = _patientQueue[index2];
    _patientQueue[index2] = store;
}

//This private helper function takes in two indecies and return the patient with the index that has a
//higher number for priority. If they are equal, return the frontmost one.
Patient PatientQueue::getMaxPriority(int index1, int index2) {
    if(_patientQueue[index1] > _patientQueue[index2]) {
        return _patientQueue[index1];
    } else if (_patientQueue[index1] < _patientQueue[index2]){
        return _patientQueue[index2];
    } else {
        return _patientQueue[min(index1, index2)];
    }
}

//This private helper function takes in two integers to represent indecies and return the patient with
//the index that has a lower number for priority. If they are equal, return the frontmost one.
Patient PatientQueue::getMinPriority(int index1, int index2) {
    if(_patientQueue[index1] < _patientQueue[index2]) {
        return _patientQueue[index1];
    } else if (_patientQueue[index1] > _patientQueue[index2]){
        return _patientQueue[index2];
    } else {
        return _patientQueue[min(index1, index2)];
    }
}

//This private helper function takes in an integer to represent an index and bubble down the patient at that
//index to restore the order.
void PatientQueue::bubbleDown(int index) {
    if (_min) {
        while(true) {
            //several different possibilities:
            //if it has no children, or if it has two children but it's priority number is smaller than any of the children
            //break.
            if (index*2 > _size || (index*2 + 1 < _size && _patientQueue[index] < getMinPriority(index*2, index*2 + 1))) {
                break;
            }
            //else, if it has one children and the children has a higher priority number, break
            else if (index*2 + 1 > _size && _patientQueue[index] < _patientQueue[index*2]) {
                break;
            }
            //else, if it has one children and the children has a lower priority number, swap them, update index
            else if (index*2 + 1 > _size && _patientQueue[index] > _patientQueue[index*2]) {
                swap(index, index*2);
                index *= 2;
            }
            //else, if it's priority number is larger than both of the children, swap with the one with min number priority
            //then update index
            else if (_patientQueue[index] >= getMaxPriority(index*2, index*2 + 1)) {
                if (_patientQueue[index*2] < _patientQueue[index*2 + 1]) {
                    swap(index, index*2);
                    index *= 2;
                } else {
                    swap(index, index*2 + 1);
                    index = index*2 + 1;
                }
            }
            //if its priority in between two children, swap with the one with lower number of priority, update index
            else if (_patientQueue[index*2] < _patientQueue[index] && _patientQueue[index] < _patientQueue[index*2 + 1]) {
                swap(index, index*2);
                index *= 2;
            }
            else if (_patientQueue[index*2 + 1] < _patientQueue[index] && _patientQueue[index] < _patientQueue[index*2]) {
                swap(index, index*2 + 1);
                index = index*2 + 1;
            }
            else {
                return;
            }
        }
    } else {
        while(true) {
            //several different possibilities:
            //if it has no children, or if it has two children but it's priority number is higher than any of the children
            //break.
            if (index*2 > _size || (index*2 + 1 < _size && _patientQueue[index] > getMaxPriority(index*2, index*2 + 1))) {
                break;
            }
            //else, if it has one children and the children has a lower priority number, break
            else if (index*2 + 1 > _size && _patientQueue[index] > _patientQueue[index*2]) {
                break;
            }
            //else, if it has one children and the children has a higher priority number, swap them, update index
            else if (index*2 + 1 > _size && _patientQueue[index] < _patientQueue[index*2]) {
                swap(index, index*2);
                index *= 2;
            }
            //else, if it's priority number is smaller than both of the children, swap with the one with max number priority
            //then update index
            else if (_patientQueue[index] <= getMinPriority(index*2, index*2 + 1)) {
                if (_patientQueue[index*2] > _patientQueue[index*2 + 1]) {
                    swap(index, index*2);
                    index *= 2;
                } else {
                    swap(index, index*2 + 1);
                    index = index*2 + 1;
                }
            }
            //if its priority in between two children, swap with the one with bigger number of priority, update index
            else if (_patientQueue[index*2] < _patientQueue[index] && _patientQueue[index] < _patientQueue[index*2 + 1]) {
                swap(index, index*2 + 1);
                index = index*2 + 1;
            }
            else if (_patientQueue[index*2 + 1] < _patientQueue[index] && _patientQueue[index] < _patientQueue[index*2]) {
                swap(index, index*2);
                index *= 2;
            }
            else {
                return;
            }
        }
    }
}

//This private helper function takes in an integer to represent an index and bubble up the patient at that,
//index to restore the order.
void PatientQueue::bubbleUp(int index) {
    //while the patient at given index has lower number of priority than its parent, swap them and update the index.
    if (_min == true) {
        while(_patientQueue[index/2] > _patientQueue[index]) {
            swap(index, index/2);
            index /= 2;
        }
    }
    //while the patient at given index has higher number of priority than its parent, swap them and update the index.
    else {
        while(_patientQueue[index/2] < _patientQueue[index] && index/2 > 0) {
            swap(index, index/2);
            index /= 2;
        }
    }
}

//This private helper function double the capacity of an arraylist on the heap. It first initiates an array with double
//the capacity, and then loop around the queue to copy it to the new array. Then it free the old array and update the name.
void PatientQueue::resize() {
    _capacity *= 2;
    Patient *_newPatientQueue = new Patient[_capacity];
    for (int i = 0; i <= _size; i++) {
        _newPatientQueue[i] = _patientQueue[i];
    }
    delete[] _patientQueue;
    _patientQueue = _newPatientQueue;
}

//This function changes the priority of a given element. It takes in a string
//to represent the name of the element and an integer to represent the new priority.
//It first loops over the array to find the index of the element and store the index and
//current priority. It then changes its priority to the new one.
void PatientQueue::changePriority(string value, int newPriority) {
    int index = 0;
    int currP = 0;
    for (int i = 1; i < _size + 1; i++) {
        if (_patientQueue[i].name == value) {
            index = i;
            currP = _patientQueue[i].priority;
            _patientQueue[i].priority = newPriority;
            break;
        }
        if (i == _size) {
            throw "This name cannout be found!";
        }
    }
    //For min heap, if the new priority is smaller
    //than the old one, we bubble up the element (and if the new priority is bigger than the old one
    //we bubble down) until it's in order.
    if (_min == true) {
        if (currP > newPriority) {
            bubbleUp(index);
        } else {
            bubbleDown(index);
        }
    //for max heap we do the opposite.
    } else {
        if (currP < newPriority) {
            bubbleUp(index);
        } else {
            bubbleDown(index);
        }
    }
}

//This function clears all elements in the patient queue by reducing the size
//of the array to 0. The array still exists and is ready to be reused.
void PatientQueue::clear() {
    _size = 0;
}

void PatientQueue::debug() {
    // empty
}

//This function dequeues frontmost element in the queue and retuns its name.
//Then it pulls the last element to the front, and makes use of bubble down
//to restore the order of the queue.
string PatientQueue::dequeue() {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    Patient toReturn = _patientQueue[1];
    _patientQueue[1] = _patientQueue[_size];
    _size--;
    int index = 1;
    bubbleDown(index);
    return toReturn.name;
}

//This function enqueues an element to the queue. It takes in a value to
//represent the name of the element and an integer to represent the priority.
//The element is initially added to the end and it will make use of bubble up
//until it's in order.
void PatientQueue::enqueue(string value, int priority) {
    _size ++;
    if (_size > _capacity) {
        resize();
    }
    Patient pat = Patient(value, priority);
    _patientQueue[_size] = pat;
    int index = _size;
    bubbleUp(index);
}

//This function returns true if our queue is empty (size = 0);
bool PatientQueue::isEmpty() const {
    return _size == 0;
}

//This function returns the name of the frontmost element
//(definition of frontmost depends on max/min heap).
//If the queue is empty an error message will be thrown.
string PatientQueue::peek() const {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    return _patientQueue[1].name;
}

//This function returns the priority of the frontmost element
//(definition of frontmost depends on max/min heap).
//If the queue is empty an error message will be thrown.
int PatientQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    return _patientQueue[1].priority;
}

//This function returns the size of our queue.
int PatientQueue::size() const {
    return _size;
}

//This function overloads the operator "<<" and returns an output file.
//Each element in the array will be added to the output file for printing.
ostream& operator <<(ostream& out, const PatientQueue& queue) {
        out << "{";
        //loop over the array and add elements to output file.
        for (int i = 1; i < queue.size(); i++) {
            out << queue._patientQueue[i] << ", ";
        }
        //for the last element we do not want to print the ", ". We do it out of the loop.
        if (!queue.isEmpty()) {
            out << queue._patientQueue[queue.size()];
        }
        out << "}";
    return out;
}
