#include <iostream>
#include <string>
#include "student_ops.h"
#include "filehandler.h"

using namespace std;

// Loads student data
void loadStudents(Student students[], int &count) {
    string data[MAX_ROWS][MAX_COLS];
    int rows = 0, cols = 0;
    if (readTXT("students.txt", data, rows, cols)) {
        for (int i = 0; i < rows; i++) {
            students[i].rollNo = data[i][0];
            students[i].name = data[i][1];
            students[i].dept = data[i][2];
            students[i].semester = stoi(data[i][3]);
            students[i].cgpa = stof(data[i][4]);
            students[i].status = data[i][5];
        }
        count = rows;
    }
}

void addStudent(Student students[], int &count) {
    if (count < MAX_ROWS) {
        cout << "Enter Roll No: "; cin >> students[count].rollNo;
        cout << "Enter Name: "; cin.ignore(); getline(cin, students[count].name);
        cout << "Enter Dept: "; cin >> students[count].dept;
        students[count].semester = 1; // Default
        students[count].cgpa = 0.0;   // Default
        students[count].status = "active";
        count++;
        cout << "Student added successfully.\n";
    } else {
        cout << "Database full!\n";
    }
}

// Finds index
int searchByRoll(Student students[], int count, string rollNo) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == rollNo) return i;
    }
    return -1;
}

// Search by name
void searchByName(Student students[], int count, string nameQuery) {
    cout << "Search Results:\n";
    for (int i = 0; i < count; i++) {
        // Simple substring check
        if (students[i].name.find(nameQuery) != string::npos) {
            cout << students[i].rollNo << " - " << students[i].name << endl;
        }
    }
}

// Update status/data
void updateStudent(Student students[], int count, string rollNo) {
    int idx = searchByRoll(students, count, rollNo);
    if (idx != -1) {
        cout << "Enter new Name: "; cin >> students[idx].name;
        cout << "Enter new CGPA: "; cin >> students[idx].cgpa;
        cout << "Student updated.\n";
    } else {
        cout << "Student not found.\n";
    }
}

void saveStudents(Student students[], int count) {
    string data[MAX_ROWS][MAX_COLS];
    string header[MAX_COLS] = {"roll_no", "name", "department", "semester", "cgpa", "status"};
    
    for(int i = 0; i < count; i++) {
        data[i][0] = students[i].rollNo;
        data[i][1] = students[i].name;
        data[i][2] = students[i].dept;
        data[i][3] = to_string(students[i].semester);
        data[i][4] = to_string(students[i].cgpa);
        data[i][5] = students[i].status;
    }
    
    writeTXT("students.txt", header, 6, data, count, 6);
}

// Soft delete
void softDelete(Student students[], int count, string rollNo) {
    int idx = searchByRoll(students, count, rollNo);
    if (idx != -1) {
        students[idx].status = "inactive";
        cout << "Student soft-deleted.\n";
    }
}

// List active
void listActiveStudents(Student students[], int count) {
    for (int i = 0; i < count; i++) {
        if (students[i].status == "active") {
            cout << students[i].rollNo << " | " << students[i].name << endl;
        }
    }
}