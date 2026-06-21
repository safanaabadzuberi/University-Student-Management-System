#include "course_ops.h"
#include "../student_ops/student_ops.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

void enrollResultMessage(EnrollResult r) {
    switch (r) {
        case ENROLL_SUCCESS:           cout << "Enrolled successfully!\n"; break;
        case ENROLL_STUDENT_INACTIVE:  cout << "Error: Student is inactive!\n"; break;
        case ENROLL_COURSE_NOT_FOUND:  cout << "Error: Course not found!\n"; break;
        case ENROLL_NO_SEATS:          cout << "Error: No seats available!\n"; break;
        case ENROLL_ALREADY_ENROLLED:  cout << "Error: Already enrolled!\n"; break;
        case ENROLL_CREDIT_OVERLOAD:   cout << "Error: Credit load would exceed 21!\n"; break;
        case ENROLL_PREREQ_NOT_MET:    cout << "Error: Prerequisite not met!\n"; break;
    }
}

bool checkPrerequisite(const char* roll, const char* courseCode) {
    char courseRow[MAX_COLS][MAX_LEN];
    int found = findRow(COURSES_FILE, 0, courseCode, courseRow, MAX_COLS);
    if (found == 0) return false;
    if (strcmp(courseRow[6], "NONE") == 0) return true;
    // prereq is courseRow[6]; check grades file
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(GRADES_FILE, rows, MAX_ROWS);
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][0], roll) == 0 &&
            strcmp(rows[i][1], courseRow[6]) == 0 &&
            strcmp(rows[i][7], "F") != 0) {
            return true;
        }
    }
    return false;
}

int getCreditLoad(const char* roll, const char* semester) {
    char enRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int enTotal = readFile(ENROLLMENTS_FILE, enRows, MAX_ROWS);
    int totalCredits = 0;
    for (int i = 0; i < enTotal; i++) {
        if (strcmp(enRows[i][1], roll) == 0 &&
            strcmp(enRows[i][3], semester) == 0 &&
            strcmp(enRows[i][5], "active") == 0) {
            char courseRow[MAX_COLS][MAX_LEN];
            int cf = findRow(COURSES_FILE, 0, enRows[i][2], courseRow, MAX_COLS);
            if (cf > 0) {
                int credits = 0;
                for (int k = 0; courseRow[2][k] >= '0' && courseRow[2][k] <= '9'; k++)
                    credits = credits * 10 + (courseRow[2][k] - '0');
                totalCredits += credits;
            }
        }
    }
    return totalCredits;
}

EnrollResult enrollStudent(const char* roll, const char* courseCode, const char* semester) {
    // check student active
    char sRow[MAX_COLS][MAX_LEN];
    int sf = findRow(STUDENTS_FILE, 0, roll, sRow, MAX_COLS);
    if (sf == 0 || strcmp(sRow[5], "active") != 0) return ENROLL_STUDENT_INACTIVE;
    // check course exists
    char cRow[MAX_COLS][MAX_LEN];
    int cf = findRow(COURSES_FILE, 0, courseCode, cRow, MAX_COLS);
    if (cf == 0) return ENROLL_COURSE_NOT_FOUND;
    // check seats
    int capacity = 0, enrolled = 0;
    for (int k = 0; cRow[4][k] >= '0' && cRow[4][k] <= '9'; k++)
        capacity = capacity * 10 + (cRow[4][k] - '0');
    for (int k = 0; cRow[5][k] >= '0' && cRow[5][k] <= '9'; k++)
        enrolled = enrolled * 10 + (cRow[5][k] - '0');
    if (enrolled >= capacity) return ENROLL_NO_SEATS;
    // check not already enrolled
    char enRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int enTotal = readFile(ENROLLMENTS_FILE, enRows, MAX_ROWS);
    for (int i = 0; i < enTotal; i++) {
        if (strcmp(enRows[i][1], roll) == 0 &&
            strcmp(enRows[i][2], courseCode) == 0 &&
            strcmp(enRows[i][3], semester) == 0 &&
            strcmp(enRows[i][5], "active") == 0)
            return ENROLL_ALREADY_ENROLLED;
    }
    // check credit load
    char courseRow[MAX_COLS][MAX_LEN];
    findRow(COURSES_FILE, 0, courseCode, courseRow, MAX_COLS);
    int newCredits = 0;
    for (int k = 0; courseRow[2][k] >= '0' && courseRow[2][k] <= '9'; k++)
        newCredits = newCredits * 10 + (courseRow[2][k] - '0');
    if (getCreditLoad(roll, semester) + newCredits > 21) return ENROLL_CREDIT_OVERLOAD;
    // check prerequisite
    if (!checkPrerequisite(roll, courseCode)) return ENROLL_PREREQ_NOT_MET;
    // generate enrollment id
    char eid[MAX_LEN];
    sprintf(eid, "E%04d", enTotal + 1);
    // append enrollment
    char fields[MAX_COLS][MAX_LEN];
    strncpy(fields[0], eid, MAX_LEN-1);
    strncpy(fields[1], roll, MAX_LEN-1);
    strncpy(fields[2], courseCode, MAX_LEN-1);
    strncpy(fields[3], semester, MAX_LEN-1);
    strncpy(fields[4], "01-01-2025", MAX_LEN-1);
    strncpy(fields[5], "active", MAX_LEN-1);
    appendFile(ENROLLMENTS_FILE, fields, 6);
    // update enrolled count
    char newEnrolled[MAX_LEN];
    sprintf(newEnrolled, "%d", enrolled + 1);
    updateFile(COURSES_FILE, 0, courseCode, 5, newEnrolled);
    return ENROLL_SUCCESS;
}

void dropCourse(const char* roll, const char* courseCode, const char* semester) {
    // check attendance rows
    char attRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int attTotal = readFile("attendance_log.txt", attRows, MAX_ROWS);
    for (int i = 0; i < attTotal; i++) {
        if (strcmp(attRows[i][1], roll) == 0 &&
            strcmp(attRows[i][2], courseCode) == 0) {
            cout << "Cannot drop: attendance already recorded!\n";
            return;
        }
    }
    // find enrollment and mark dropped
    char enRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int enTotal = readFile(ENROLLMENTS_FILE, enRows, MAX_ROWS);
    bool found = false;
    for (int i = 0; i < enTotal; i++) {
        if (strcmp(enRows[i][1], roll) == 0 &&
            strcmp(enRows[i][2], courseCode) == 0 &&
            strcmp(enRows[i][3], semester) == 0 &&
            strcmp(enRows[i][5], "active") == 0) {
            updateFile(ENROLLMENTS_FILE, 0, enRows[i][0], 5, "dropped");
            // decrement enrolled count
            char cRow[MAX_COLS][MAX_LEN];
            int cf = findRow(COURSES_FILE, 0, courseCode, cRow, MAX_COLS);
            if (cf > 0) {
                int enrolled = 0;
                for (int k = 0; cRow[5][k] >= '0' && cRow[5][k] <= '9'; k++)
                    enrolled = enrolled * 10 + (cRow[5][k] - '0');
                if (enrolled > 0) enrolled--;
                char nb[MAX_LEN];
                sprintf(nb, "%d", enrolled);
                updateFile(COURSES_FILE, 0, courseCode, 5, nb);
            }
            found = true;
            break;
        }
    }
    if (found) cout << "Course dropped successfully!\n";
    else       cout << "Enrollment not found!\n";
}

void listEnrolledStudents(const char* courseCode) {
    char enRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int enTotal = readFile(ENROLLMENTS_FILE, enRows, MAX_ROWS);
    cout << "\n--- Students enrolled in " << courseCode << " ---\n";
    printf("%-14s %-20s %-10s\n", "Roll No", "Semester", "Status");
    cout << string(50, '-') << "\n";
    int count = 0;
    for (int i = 0; i < enTotal; i++) {
        if (strcmp(enRows[i][2], courseCode) == 0 &&
            strcmp(enRows[i][5], "active") == 0) {
            char sRow[MAX_COLS][MAX_LEN];
            findRow(STUDENTS_FILE, 0, enRows[i][1], sRow, MAX_COLS);
            printf("%-14s %-20s %-10s\n", enRows[i][1], sRow[1], enRows[i][5]);
            count++;
        }
    }
    cout << "Total enrolled: " << count << "\n";
}

void addCourse() {
    char code[MAX_LEN], name[MAX_LEN], credits[MAX_LEN],
         instr[MAX_LEN], cap[MAX_LEN], prereq[MAX_LEN];
    cout << "\n--- Add Course ---\n";
    cout << "Course Code: ";    cin >> code;
    if (checkRecordExists(COURSES_FILE, 0, code)) {
        cout << "Course already exists!\n";
        return;
    }
    cin.ignore();
    cout << "Course Name: ";    cin.getline(name, MAX_LEN);
    cout << "Credit Hours: ";   cin >> credits;
    cin.ignore();
    cout << "Instructor: ";     cin.getline(instr, MAX_LEN);
    cout << "Capacity: ";       cin >> cap;
    cout << "Prerequisite (course code or NONE): "; cin >> prereq;
    char fields[MAX_COLS][MAX_LEN];
    strncpy(fields[0], code,    MAX_LEN-1);
    strncpy(fields[1], name,    MAX_LEN-1);
    strncpy(fields[2], credits, MAX_LEN-1);
    strncpy(fields[3], instr,   MAX_LEN-1);
    strncpy(fields[4], cap,     MAX_LEN-1);
    strncpy(fields[5], "0",     MAX_LEN-1);
    strncpy(fields[6], prereq,  MAX_LEN-1);
    appendFile(COURSES_FILE, fields, 7);
    cout << "Course added!\n";
}

void enrollStudentMenu() {
    char roll[MAX_LEN], code[MAX_LEN], sem[MAX_LEN];
    cout << "\n--- Enroll Student ---\n";
    cout << "Roll No: ";      cin >> roll;
    cout << "Course Code: "; cin >> code;
    cout << "Semester: ";    cin >> sem;
    EnrollResult r = enrollStudent(roll, code, sem);
    enrollResultMessage(r);
}

void dropCourseMenu() {
    char roll[MAX_LEN], code[MAX_LEN], sem[MAX_LEN];
    cout << "\n--- Drop Course ---\n";
    cout << "Roll No: ";      cin >> roll;
    cout << "Course Code: "; cin >> code;
    cout << "Semester: ";    cin >> sem;
    dropCourse(roll, code, sem);
}

void listCoursesMenu() {
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(COURSES_FILE, rows, MAX_ROWS);
    cout << "\n--- All Courses ---\n";
    printf("%-10s %-35s %-4s %-8s %-8s %-12s\n",
            "Code","Title","Cr","Cap","Enrolled","Prereq");
    cout << string(80, '-') << "\n";
    for (int i = 0; i < total; i++) {
        printf("%-10s %-35s %-4s %-8s %-8s %-12s\n",
                rows[i][0], rows[i][1], rows[i][2],
                rows[i][4], rows[i][5], rows[i][6]);
    }
}

void listEnrolledMenu() {
    char code[MAX_LEN];
    cout << "Enter Course Code: ";
    cin >> code;
    listEnrolledStudents(code);
}