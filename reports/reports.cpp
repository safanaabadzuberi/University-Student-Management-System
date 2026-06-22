#include "reports.h"
#include "../student_ops/student_ops.h"
#include "../attendance/attendance.h"
#include "../grades/grades.h"
#include "../fee_tracker/fee_tracker.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

void showStudentSummaryReport(const char* roll) {
    char sRow[MAX_COLS][MAX_LEN];
    
    if (findRow(STUDENTS_FILE, 0, roll, sRow, MAX_COLS)) {
        cout << "\n" << string(50, '=') << "\n";
        cout << "STUDENT SUMMARY REPORT\n";
        cout << string(50, '-') << "\n";
        cout << left << setw(12) << "Name:" << sRow[1] << "\n";
        cout << left << setw(12) << "Roll No:" << roll << "\n";
        cout << left << setw(12) << "Dept:" << sRow[2] << "\n";
        
        // Dynamic GPA (Optional: pass semester as argument)
        cout << left << setw(12) << "Latest GPA:" << fixed << setprecision(2) 
             << computeGPA(roll, "FALL-2025") << "\n";
        
        // Fee Detail
        double fine = computeLateFine(roll, "FALL-2025");
        cout << left << setw(12) << "Fee Status:" << (fine > 0 ? "Pending Fine" : "Cleared") 
             << (fine > 0 ? " (PKR " + to_string((int)fine) + ")" : "") << "\n";
        
        cout << string(50, '=') << "\n";
    } else {
        cout << "Error : Student " << roll << " not found!\n";
    }
}

void showCoursePerformanceReport(const char* courseCode) {
    Stats s = computeClassStats(courseCode);
    cout << "\n--- Course Analysis: " << courseCode << " ---\n";
    cout << "Class Mean:   " << fixed << setprecision(2) << s.mean << "\n";
    cout << "Class Median: " << s.median << "\n";
    cout << "Performance Spread: " << (s.highest - s.lowest) << "\n";
}

void showSemesterAcademicSummary(const char* roll, const char* semester) {
    cout << "\n--- Academic Summary: " << roll << " | " << semester << " ---\n";
    double gpa = computeGPA(roll, semester);
    cout << "Semester GPA: " << fixed << setprecision(2) << gpa << "\n";
    
    // Check Fee Status
    cout << "Fee Status: ";
    double fine = computeLateFine(roll, semester);
    cout << (fine > 0 ? "Outstanding Fine / Delinquent" : "Clear") << "\n";
}

void studentSummaryMenu() {
    char roll[MAX_LEN];
    cout << "Enter Roll No: "; cin >> roll;
    showStudentSummaryReport(roll);
}

void coursePerformanceMenu() {
    char code[MAX_LEN];
    cout << "Enter Course Code: "; cin >> code;
    showCoursePerformanceReport(code);
}

void semesterSummaryMenu() {
    char roll[MAX_LEN], sem[MAX_LEN];
    cout << "Enter Roll No: "; cin >> roll;
    cout << "Enter Semester: "; cin >> sem;
    showSemesterAcademicSummary(roll, sem);
}