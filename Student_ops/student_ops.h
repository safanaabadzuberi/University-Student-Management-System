#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include "filehandler.h"

// Struct storage
struct Student {
    string rollNo;
    string name;
    string dept;
    int semester;
    float cgpa;
    string status;
};

// Load data
void loadStudents(Student students[], int &count);

// Add student
void addStudent(Student students[], int &count);

// Search roll
int searchByRoll(Student students[], int count, string rollNo);

// Search name
void searchByName(Student students[], int count, string nameQuery);

// Update student
void updateStudent(Student students[], int count, string rollNo);

//Save student
void saveStudents(Student students[], int count)

// Soft delete
void softDelete(Student students[], int count, string rollNo);

// List active
void listActiveStudents(Student students[], int count);

#endif