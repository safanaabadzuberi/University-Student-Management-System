#ifndef REPORTS_H
#define REPORTS_H

#include "../file_handler/file_handler.h"

// Aggregates data from various modules to show comprehensive reports
void showStudentSummaryReport(const char* roll);
void showCoursePerformanceReport(const char* courseCode);
void showSemesterAcademicSummary(const char* roll, const char* semester);

void studentSummaryMenu();
void coursePerformanceMenu();
void semesterSummaryMenu();

#endif