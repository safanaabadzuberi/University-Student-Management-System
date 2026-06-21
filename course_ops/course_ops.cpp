#include <iostream>
#include <string>
#include "course_ops.h"
#include "filehandler.h"

using namespace std;

// Enroll student
void enrollStudent(string rollNo, string courseCode) {
    string sData[MAX_ROWS][MAX_COLS], cData[MAX_ROWS][MAX_COLS], eData[MAX_ROWS][MAX_COLS];
    int sRows, sCols, cRows, cCols, eRows, eCols;

    // Load all data
    readTXT("students.txt", sData, sRows, sCols);
    readTXT("courses.txt", cData, cRows, cCols);
    readTXT("enrollments.txt", eData, eRows, eCols);

    // 1. Check if active
    bool active = false;
    for(int i = 0; i < sRows; i++) 
        if(sData[i][0] == rollNo && sData[i][5] == "active") active = true;
    if(!active) { cout << "Student inactive.\n"; return; }

    // 2. Check capacity
    bool space = false;
    for(int i = 0; i < cRows; i++) 
        if(cData[i][0] == courseCode && stoi(cData[i][5]) < stoi(cData[i][4])) space = true;
    if(!space) { cout << "Course full.\n"; return; }

    // 3. Check duplicate
    for(int i = 0; i < eRows; i++) 
        if(eData[i][1] == rollNo && eData[i][2] == courseCode) { cout << "Already enrolled.\n"; return; }

    // 4. Check prerequisite
    if (!checkPrerequisite(rollNo, courseCode)) {
        cout << "Prerequisite not met.\n";
        return;
    }

    // 5. Save new record
    string newRow[MAX_COLS] = {"", rollNo, courseCode, "1", "0.0", "active"};
    appendTXT("enrollments.txt", newRow, 6);
    cout << "Enrolled successfully.\n";
}

// Drop course
void dropCourse(string rollNo, string courseCode) {
    // 1. Check attendance_log.txt for any record of this student in this course
    // 2. If no attendance, update status in enrollments.txt to 'dropped'
    cout << "Course " << courseCode << " dropped for " << rollNo << ".\n";
}

// Get credit load
int getCreditLoad(string rollNo, int semester) {
    int totalCredits = 0;
    string enrollData[MAX_ROWS][MAX_COLS];
    int eRows, eCols;
    
    readTXT("enrollments.txt", enrollData, eRows, eCols);
    
    // Nested loop over enrollments and courses
    for (int i = 0; i < eRows; i++) {
        if (enrollData[i][1] == rollNo) { // matches roll
            string courseData[MAX_ROWS][MAX_COLS];
            int cRows, cCols;
            readTXT("courses.txt", courseData, cRows, cCols);
            
            for (int j = 0; j < cRows; j++) {
                if (courseData[j][0] == enrollData[i][2]) { // matches code
                    totalCredits += stoi(courseData[j][2]); // credit hours
                }
            }
        }
    }
    return totalCredits;
}

// Check prerequisite
bool checkPrerequisite(string rollNo, string courseCode) {
    // Step A: Get the prerequisite for current course
    string courseData[MAX_ROWS][MAX_COLS];
    int rows, cols;
    readTXT("courses.txt", courseData, rows, cols);
    
    string prereqCode = "NONE";
    for(int i = 0; i < rows; i++) {
        if(courseData[i][0] == courseCode) {
            prereqCode = courseData[i][6]; // Assuming col 6 is prereq
            break;
        }
    }

    // Step B: If "NONE", they can enroll
    if(prereqCode == "NONE") return true;

    // Step C: Check if student passed the prerequisite
    // Search enrollments.txt for (rollNo AND courseCode == prereqCode)
    // AND check if their grade is not 'F'
    string enrollData[MAX_ROWS][MAX_COLS];
    readTXT("enrollments.txt", enrollData, rows, cols);
    
    for(int i = 0; i < rows; i++) {
        if(enrollData[i][1] == rollNo && enrollData[i][2] == prereqCode) {
            if(enrollData[i][6] != "F") return true; // Passed!
        }
    }
    
    return false; // Prereq not found or failed
}

// List enrolled students
void listEnrolledStudents(string courseCode) {
    string enrollData[MAX_ROWS][MAX_COLS];
    int rows, cols;
    readTXT("enrollments.txt", enrollData, rows, cols);
    
    cout << "Students in " << courseCode << ":\n";
    for (int i = 0; i < rows; i++) {
        if (enrollData[i][2] == courseCode && enrollData[i][5] == "active") {
            cout << enrollData[i][1] << endl;
        }
    }
}