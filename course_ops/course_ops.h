#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include "filehandler.h"

// Struct data
struct Course {
    string code;
    string name;
    int creditHours;
    string instructor;
    int capacity;
    int enrolled;
    string prerequisite;
};

// Enroll student
void enrollStudent(string rollNo, string courseCode);

// Drop course
void dropCourse(string rollNo, string courseCode);

// Get load
int getCreditLoad(string rollNo, int semester);

// Check prereq
bool checkPrerequisite(string rollNo, string courseCode);

// List enrolled
void listEnrolledStudents(string courseCode);

#endif