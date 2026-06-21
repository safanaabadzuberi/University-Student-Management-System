#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include "../file_handler/file_handler.h"

#define STUDENTS_FILE "students.txt"

bool validateRollFormat(const char* roll);
bool validateName(const char* name);
bool validateCGPA(const char* cgpa);

void addStudent();
void searchStudentByRoll();
void searchStudentByName();
void updateStudent();
void softDeleteStudent();
void listActiveStudents();

#endif
