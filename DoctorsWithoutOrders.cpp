/*
 * Doctors Without Orders - By Gabrielle Li and Noopur Bhatt
 * Partner SUNets: gabbyli1; nbhatt
 * CS 106B - Summer 2019
 * Due July 25, 2019
 *
 * The main objective of this program is to take in a list of patients and a list of
 * doctors to generate a schedule that fits the needs of the doctor and the patient.
 * This program emphasizes the use of recursive backtracking.
 *
 * Sources Used: Spec; Lecture Slides / Code Day #1 - #15; LAiR
 */


#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "vector.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
//prototype of helper function
bool helper(Vector<Doctor> &doctors,
            Vector<Patient> &patients,
            Map<string, Set<string>>& schedule);

bool canAllPatientsBeSeen(Vector<Doctor> &doctors,
                          Vector<Patient> &patients,
                          Map<string, Set<string>>& schedule) {
    //Make a copy of patient list and doctor list
    Vector<Patient> copyPat = patients;
    Vector<Doctor> copyDoc = doctors;
    //Pass in the copy instead of the list so that the list will not be changed for the purpose of current print-out result
    return helper(copyDoc, copyPat, schedule);
}

bool helper(Vector<Doctor> &doctors,
            Vector<Patient> &patients,
            Map<string, Set<string>>& schedule) {

    //base case: there is no patients. Immediately return true.
    if (patients.size() == 0) {
        return true;
    }
    //base case: there are patients but no doctors. Immediately return false.
    else if (doctors.size() == 0) {
        return false;
    }
    //simple case: if total hours of patients exceed total hours of doctors, return false.
    int totalDocHours = 0;
    for (Doctor currentDoc: doctors) {
        int hours = currentDoc.hoursFree;
        totalDocHours += hours;
    }
    int totalPatHours = 0;
    for (Patient currentPat: patients) {
        int hours = currentPat.hoursNeeded;
        totalPatHours += hours;
    }
    if (totalPatHours > totalDocHours) {
        return false;
    }

    //recursive case:
    for (int i = 0; i < doctors.size(); i++) {
        Patient thisPat = patients[0];
        //choose: we loop through all the doctors and assign patients to them from the list of patients.
        //If the doctor's available hours is greater than the patient's needed hours we make the decision.
        if (doctors[i].hoursFree >= thisPat.hoursNeeded) {
            //If this doctor is not yet on the schedule, put it on the schedule as a key.
            if (!schedule.containsKey(doctors[i].name)) {
                Set<string> values;
                schedule.put(doctors[i].name, values);
            }
            //Get the set of patients associated with this doctor, add current patient to the list,
            //and linked the updated value with the key
            Set<string> thisValue = schedule[doctors[i].name];
            thisValue.add(thisPat.name);
            schedule.put(doctors[i].name, thisValue);
        //explore: update the hoursFree of the doctor and remove the patient from the list of patients
            doctors[i].hoursFree -= thisPat.hoursNeeded;
            patients.remove(0);

            if (canAllPatientsBeSeen(doctors, patients, schedule)) {
                return true;
            }
            //unchoose: backtracking: remove the patient from the list associated with this doctor, restore
            //the doctors available hours, put the patient back to the list.
            schedule[doctors[i].name].remove(thisPat.name);
            doctors[i].hoursFree += thisPat.hoursNeeded;
            patients.insert(0, thisPat);
        }
    }
    //if no solution, return false
    return false;
}
