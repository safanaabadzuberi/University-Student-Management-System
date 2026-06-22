#include "grades.h"
#include "../student_ops/student_ops.h"
#include "../course_ops/course_ops.h"
#include "../attendance/attendance.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

double bestThreeOfFive(double marks[], int n) {
    if (n <= 0) return 0.0;
    if (n < 3) {
        double sum = 0;
        for (int i = 0; i < n; i++) sum += marks[i];
        return sum / n;
    }
    // find and exclude two lowest
    double tmp[5];
    for (int i = 0; i < n; i++) tmp[i] = marks[i];
    for (int i = 0; i < 2; i++) {
        int minIdx = 0;
        for (int j = 1; j < n; j++)
            if (tmp[j] < tmp[minIdx]) minIdx = j;
        tmp[minIdx] = 1e9;
    }
    double sum = 0; int count = 0;
    for (int i = 0; i < n; i++)
        if (tmp[i] < 1e8) { sum += tmp[i]; count++; }
    return count > 0 ? sum / count : 0.0;
}

double computeWeightedTotal(double quiz, double asgn, double mid, double final_) {
    return quiz * 0.10 + asgn * 0.10 + mid * 0.30 + final_ * 0.50;
}

const char* getLetterGrade(double total) {
    if (total >= 85) return "A";
    if (total >= 80) return "B+";
    if (total >= 70) return "B";
    if (total >= 65) return "C+";
    if (total >= 60) return "C";
    if (total >= 50) return "D";
    return "F";
}

double computeGPA(const char* roll, const char* semester) {
    char gradeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int grTotal = readFile(GRADES_FILE, gradeRows, MAX_ROWS);
    double totalPoints = 0.0;
    int    totalCredits = 0;
    for (int i = 0; i < grTotal; i++) {
        if (strcmp(gradeRows[i][0], roll) == 0 &&
            strcmp(gradeRows[i][2], semester) == 0) {
            int credits = 0;
            for (int k = 0; gradeRows[i][8][k] >= '0' && gradeRows[i][8][k] <= '9'; k++)
                credits = credits * 10 + (gradeRows[i][8][k] - '0');
            double gpa = 0.0;
            char* lg = gradeRows[i][7];
            if (strcmp(lg,"A")==0)       gpa = 4.0;
            else if (strcmp(lg,"B+")==0) gpa = 3.5;
            else if (strcmp(lg,"B")==0)  gpa = 3.0;
            else if (strcmp(lg,"C+")==0) gpa = 2.5;
            else if (strcmp(lg,"C")==0)  gpa = 2.0;
            else if (strcmp(lg,"D")==0)  gpa = 1.0;
            else                         gpa = 0.0;
            totalPoints  += gpa * credits;
            totalCredits += credits;
        }
    }
    return totalCredits > 0 ? totalPoints / totalCredits : 0.0;
}

Stats computeClassStats(const char* courseCode) {
    char gradeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int grTotal = readFile(GRADES_FILE, gradeRows, MAX_ROWS);
    double totals[MAX_ROWS];
    int count = 0;
    for (int i = 0; i < grTotal; i++) {
        if (strcmp(gradeRows[i][1], courseCode) == 0) {
            double val = 0.0;
            int j = 0; bool dec = false; double d = 0.1;
            for (; gradeRows[i][6][j] != '\0'; j++) {
                char c = gradeRows[i][6][j];
                if (c == '.') { dec = true; continue; }
                if (c >= '0' && c <= '9') {
                    if (!dec) val = val*10+(c-'0');
                    else { val += (c-'0')*d; d*=0.1; }
                }
            }
            totals[count++] = val;
        }
    }
    Stats s = {0,0,0,0};
    if (count == 0) return s;
    s.highest = totals[0]; s.lowest = totals[0]; s.mean = 0.0;
    for (int i = 0; i < count; i++) {
        if (totals[i] > s.highest) s.highest = totals[i];
        if (totals[i] < s.lowest)  s.lowest  = totals[i];
        s.mean += totals[i];
    }
    s.mean /= count;
    for (int i = 0; i < count-1; i++)
        for (int j = 0; j < count-i-1; j++)
            if (totals[j] > totals[j+1]) {
                double tmp = totals[j]; totals[j]=totals[j+1]; totals[j+1]=tmp;
            }
    s.median = (count%2==0) ? (totals[count/2-1]+totals[count/2])/2.0
                            : totals[count/2];
    return s;
}

void applyAttendancePenalty(const char* roll, const char* courseCode) {
    double pct = getAttendancePct(roll, courseCode);
    if (pct < 75.0) {
        updateFile(GRADES_FILE, 0, roll, 7, "F");
        cout << "Attendance penalty applied: grade set to F for " << roll << "\n";
    }
}

void enterMarksMenu() {
    char roll[MAX_LEN], code[MAX_LEN], sem[MAX_LEN];
    cout << "\n--- Enter Marks ---\n";
    cout << "Roll No: ";      cin >> roll;
    cout << "Course Code: "; cin >> code;
    cout << "Semester: ";    cin >> sem;
    char grRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int grTotal = readFile(GRADES_FILE, grRows, MAX_ROWS);
    for (int i = 0; i < grTotal; i++) {
        if (strcmp(grRows[i][0], roll)==0 && strcmp(grRows[i][1], code)==0) {
            cout << "Marks already entered!\n";
            return;
        }
    }
    double quizMarks[5]; int qn = 0;
    cout << "How many quizzes (1-5)? "; cin >> qn;
    if (qn<1) qn=1; if (qn>5) qn=5;
    for (int i = 0; i < qn; i++) {
        cout << "Quiz " << i+1 << " (out of 10): ";
        cin >> quizMarks[i];
    }
    double quizAvg = bestThreeOfFive(quizMarks, qn);
    double asgn[3]; double asgnAvg = 0;
    cout << "Assignments (3, out of 10 each):\n";
    for (int i = 0; i < 3; i++) {
        cout << "Assignment " << i+1 << ": ";
        cin >> asgn[i];
        asgnAvg += asgn[i];
    }
    asgnAvg /= 3.0;
    double mid=0, final_=0;
    cout << "Mid exam (out of 40): ";   cin >> mid;
    cout << "Final exam (out of 60): "; cin >> final_;
    double midN   = (mid   / 40.0) * 100.0;
    double finalN = (final_/ 60.0) * 100.0;
    double total  = computeWeightedTotal(quizAvg, asgnAvg, midN, finalN);
    const char* grade = getLetterGrade(total);
    char cRow[MAX_COLS][MAX_LEN];
    findRow(COURSES_FILE, 0, code, cRow, MAX_COLS);
    char totalStr[MAX_LEN], quizStr[MAX_LEN], midStr[MAX_LEN], finStr[MAX_LEN];
    sprintf(totalStr, "%.2f", total);
    sprintf(quizStr,  "%.2f", quizAvg);
    sprintf(midStr,   "%.2f", midN);
    sprintf(finStr,   "%.2f", finalN);
    char fields[MAX_COLS][MAX_LEN];
    strncpy(fields[0], roll,      MAX_LEN-1);
    strncpy(fields[1], code,      MAX_LEN-1);
    strncpy(fields[2], sem,       MAX_LEN-1);
    strncpy(fields[3], quizStr,   MAX_LEN-1);
    sprintf(fields[4], "%.2f", asgnAvg);
    strncpy(fields[5], midStr,    MAX_LEN-1);
    strncpy(fields[6], totalStr,  MAX_LEN-1);
    strncpy(fields[7], grade,     MAX_LEN-1);
    strncpy(fields[8], cRow[2][0]?cRow[2]:"3", MAX_LEN-1);
    appendFile(GRADES_FILE, fields, 9);
    printf("\nTotal: %.2f | Grade: %s\n", total, grade);
    applyAttendancePenalty(roll, code);
}

void showGradeReportMenu() {
    char roll[MAX_LEN], sem[MAX_LEN];
    cout << "\nRoll No: ";   cin >> roll;
    cout << "Semester: ";   cin >> sem;
    char grRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int grTotal = readFile(GRADES_FILE, grRows, MAX_ROWS);
    cout << "\n--- Grade Report: " << roll << " Sem " << sem << " ---\n";
    printf("%-10s %-8s %-6s %-6s %-6s %-6s %-7s\n",
            "Course","Sem","Quiz","Asgn","Mid","Total","Grade");
    cout << string(55, '-') << "\n";
    for (int i = 0; i < grTotal; i++) {
        if (strcmp(grRows[i][0], roll)==0 && strcmp(grRows[i][2], sem)==0) {
            printf("%-10s %-8s %-6s %-6s %-6s %-6s %-7s\n",
                    grRows[i][1], grRows[i][2], grRows[i][3],
                    grRows[i][4], grRows[i][5], grRows[i][6], grRows[i][7]);
        }
    }
    printf("\nSemester GPA: %.2f\n", computeGPA(roll, sem));
}

void classStatsMenu() {
    char code[MAX_LEN];
    cout << "\nCourse Code: "; cin >> code;
    Stats s = computeClassStats(code);
    printf("\n--- Class Stats: %s ---\n", code);
    printf("Highest : %.2f\n", s.highest);
    printf("Lowest  : %.2f\n", s.lowest);
    printf("Mean    : %.2f\n", s.mean);
    printf("Median  : %.2f\n", s.median);
}

void semesterGPAMenu() {
    char roll[MAX_LEN], sem[MAX_LEN];
    cout << "\nRoll No: ";  cin >> roll;
    cout << "Semester: ";  cin >> sem;
    printf("Semester GPA for %s (Sem %s): %.2f\n", roll, sem, computeGPA(roll, sem));
}