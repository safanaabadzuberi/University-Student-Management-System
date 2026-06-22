#ifndef GRADES_H
#define GRADES_H

#include "../file_handler/file_handler.h"

#define GRADES_FILE "grades.txt"

struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};

double bestThreeOfFive(double marks[], int n);
double computeWeightedTotal(double quiz, double asgn, double mid, double final_);
const char* getLetterGrade(double total);
double computeGPA(const char* roll, const char* semester);
Stats  computeClassStats(const char* courseCode);
void   applyAttendancePenalty(const char* roll, const char* courseCode);

void enterMarksMenu();
void showGradeReportMenu();
void classStatsMenu();
void semesterGPAMenu();

#endif
