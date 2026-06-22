#include <fstream>
#include "attendance.h"
#include "../course_ops/course_ops.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

// backup for undo
static char backupRows[MAX_ROWS][MAX_COLS][MAX_LEN];
static int  backupCount = 0;
static bool hasBackup   = false;

void markAttendance(const char* courseCode, const char* date, const char* semester) {
    // save backup
    backupCount = readFile(ATTENDANCE_FILE, backupRows, MAX_ROWS);
    hasBackup   = true;
    // get enrolled students
    char enRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int enTotal = readFile(ENROLLMENTS_FILE, enRows, MAX_ROWS);
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(ATTENDANCE_FILE, rows, MAX_ROWS);
    int logId = total + 1;
    cout << "\n--- Mark Attendance: " << courseCode
         << " | Date: " << date << " ---\n";
    for (int i = 0; i < enTotal; i++) {
        if (strcmp(enRows[i][2], courseCode) == 0 &&
            strcmp(enRows[i][3], semester)    == 0 &&
            strcmp(enRows[i][5], "active")    == 0) {
            char status[MAX_LEN];
            cout << "Roll: " << enRows[i][1] << " -> P/A/L: ";
            cin >> status;
            // validate
            if (strcmp(status,"P")!=0 && strcmp(status,"A")!=0 && strcmp(status,"L")!=0) {
                cout << "Invalid! Defaulting to A\n";
                strncpy(status, "A", MAX_LEN-1);
            }
            char lid[MAX_LEN];
            sprintf(lid, "L%05d", logId++);
            char fields[MAX_COLS][MAX_LEN];
            strncpy(fields[0], lid,        MAX_LEN-1);
            strncpy(fields[1], enRows[i][1],MAX_LEN-1);
            strncpy(fields[2], courseCode,  MAX_LEN-1);
            strncpy(fields[3], date,        MAX_LEN-1);
            strncpy(fields[4], status,      MAX_LEN-1);
            appendFile(ATTENDANCE_FILE, fields, 5);
        }
    }
    cout << "Attendance marked!\n";
}

double getAttendancePct(const char* roll, const char* courseCode) {
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(ATTENDANCE_FILE, rows, MAX_ROWS);
    int sessions = 0, present = 0, late = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][1], roll) == 0 &&
            strcmp(rows[i][2], courseCode) == 0) {
            sessions++;
            if (strcmp(rows[i][4], "P") == 0) present++;
            else if (strcmp(rows[i][4], "L") == 0) late++;
        }
    }
    if (sessions == 0) return 0.0;
    return (present + 0.5 * late) / sessions * 100.0;
}

void getShortageList(const char* courseCode) {
    char enRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int enTotal = readFile(ENROLLMENTS_FILE, enRows, MAX_ROWS);
    cout << "\n--- Attendance Shortage (<75%) in " << courseCode << " ---\n";
    printf("%-14s %-6s\n", "Roll No", "Att%");
    cout << string(25, '-') << "\n";
    for (int i = 0; i < enTotal; i++) {
        if (strcmp(enRows[i][2], courseCode) == 0 &&
            strcmp(enRows[i][5], "active") == 0) {
            double pct = getAttendancePct(enRows[i][1], courseCode);
            if (pct < 75.0) {
                printf("%-14s %.2f%%\n", enRows[i][1], pct);
            }
        }
    }
}

bool undoLastSession(const char* courseCode, const char* date) {
    if (!hasBackup) return false;
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(ATTENDANCE_FILE, rows, MAX_ROWS);
    ifstream fin2(ATTENDANCE_FILE);
    char header[MAX_COLS*MAX_LEN]="";
    if (fin2.is_open()) { fin2.getline(header, sizeof(header)); fin2.close(); }
    ofstream fout(ATTENDANCE_FILE);
    fout << header << "\n";
    char buf[MAX_COLS*MAX_LEN];
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][2], courseCode)==0 && strcmp(rows[i][3], date)==0)
            continue;
        char fields[MAX_COLS][MAX_LEN];
        for (int k=0;k<5;k++) strncpy(fields[k],rows[i][k],MAX_LEN-1);
        joinFields(fields, 5, buf, ',');
        fout << buf;
    }
    fout.close();
    cout << "Last session undone!\n";
    return true;
}

void printDailySheet(const char* courseCode, const char* date) {
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(ATTENDANCE_FILE, rows, MAX_ROWS);
    cout << "\n--- Daily Sheet: " << courseCode
              << " | " << date << " ---\n";
    printf("%-14s %-8s\n", "Roll No", "Status");
    cout << string(25, '-') << "\n";
    int count = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][2], courseCode)==0 && strcmp(rows[i][3], date)==0) {
            printf("%-14s %-8s\n", rows[i][1], rows[i][4]);
            count++;
        }
    }
    if (count == 0) cout << "No records for this date!\n";
}

void markAttendanceMenu() {
    char code[MAX_LEN], date[MAX_LEN], sem[MAX_LEN];
    cout << "\nCourse Code: ";   cin >> code;
    cout << "Date (DD-MM-YYYY): "; cin >> date;
    cout << "Semester: ";        cin >> sem;
    markAttendance(code, date, sem);
}

void attendancePercentMenu() {
    char roll[MAX_LEN], code[MAX_LEN];
    cout << "\nRoll No: ";      cin >> roll;
    cout << "Course Code: ";   cin >> code;
    double pct = getAttendancePct(roll, code);
    printf("Attendance: %.2f%%\n", pct);
    if (pct < 75.0) cout << "WARNING: Below 75%!\n";
}

void shortageListMenu() {
    char code[MAX_LEN];
    cout << "\nCourse Code: "; cin >> code;
    getShortageList(code);
}

void undoSessionMenu() {
    char code[MAX_LEN], date[MAX_LEN];
    cout << "\nCourse Code: ";      cin >> code;
    cout << "Date to undo (DD-MM-YYYY): "; cin >> date;
    if (!undoLastSession(code, date))
        cout << "No backup to undo!\n";
}

void dailySheetMenu() {
    char code[MAX_LEN], date[MAX_LEN];
    cout << "\nCourse Code: ";   cin >> code;
    cout << "Date (DD-MM-YYYY): "; cin >> date;
    printDailySheet(code, date);
}