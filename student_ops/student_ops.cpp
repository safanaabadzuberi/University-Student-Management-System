#include "student_ops.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

bool validateRollFormat(const char* roll) {
    // Must match BSAI-YY-XXX (len 11)
    if (strlen(roll) != 11) return false;
    if (roll[0] != 'B' || roll[1] != 'S' || roll[2] != 'A' || roll[3] != 'I') return false;
    if (roll[4] != '-') return false;
    if (roll[5] < '0' || roll[5] > '9') return false;
    if (roll[6] < '0' || roll[6] > '9') return false;
    if (roll[7] != '-') return false;
    if (roll[8] < '0' || roll[8] > '9') return false;
    if (roll[9] < '0' || roll[9] > '9') return false;
    if (roll[10] < '0' || roll[10] > '9') return false;
    return true;
}

bool validateName(const char* name) {
    // Check digits
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] >= '0' && name[i] <= '9') return false;
    }
    return name[0] != '\0';
}

bool validateCGPA(const char* cgpa) {
    double val = 0.0;
    int i = 0;
    // Integer part
    while (cgpa[i] >= '0' && cgpa[i] <= '9') {
        val = val * 10 + (cgpa[i] - '0');
        i++;
    }
    // Fractional part
    if (cgpa[i] == '.') {
        i++;
        double dec = 0.1;
        while (cgpa[i] >= '0' && cgpa[i] <= '9') {
            val += (cgpa[i] - '0') * dec;
            dec *= 0.1;
            i++;
        }
    }
    return (cgpa[0] != '\0' && cgpa[i] == '\0' && val >= 0.0 && val <= 4.0);
}

void addStudent() {
    char roll[MAX_LEN], name[MAX_LEN], dept[MAX_LEN], sem[MAX_LEN], cgpa[MAX_LEN];
    cout << "\n--- Add Student ---\n";
    cout << "Enter Roll No (BSAI-YY-XXX): ";
    cin >> roll;
    if (!validateRollFormat(roll)) {
        cout << "Invalid roll format!\n";
        return;
    }
    if (checkRecordExists(STUDENTS_FILE, 0, roll)) {
        cout << "Student already exists!\n";
        return;
    }
    cin.ignore();
    cout << "Enter Name: ";
    cin.getline(name, MAX_LEN);
    if (!validateName(name)) {
        cout << "Name cannot contain digits!\n";
        return;
    }
    cout << "Enter Department: ";
    cin.getline(dept, MAX_LEN);
    cout << "Enter Semester: ";
    cin.getline(sem, MAX_LEN);
    cout << "Enter CGPA (0.0-4.0): ";
    cin >> cgpa;
    if (!validateCGPA(cgpa)) {
        cout << "Invalid CGPA!\n";
        return;
    }
    char fields[MAX_COLS][MAX_LEN];
    strncpy(fields[0], roll, MAX_LEN - 1);
    strncpy(fields[1], name, MAX_LEN - 1);
    strncpy(fields[2], dept, MAX_LEN - 1);
    strncpy(fields[3], sem, MAX_LEN - 1);
    strncpy(fields[4], cgpa, MAX_LEN - 1);
    strncpy(fields[5], "active", MAX_LEN - 1);
    appendFile(STUDENTS_FILE, fields, 6);
    cout << "Student added successfully!\n";
}

void searchStudentByRoll() {
    char roll[MAX_LEN];
    cout << "\nEnter Roll No: ";
    cin >> roll;
    char result[MAX_COLS][MAX_LEN];
    int found = findRow(STUDENTS_FILE, 0, roll, result, MAX_COLS);
    if (found == 0) {
        cout << "Student not found!\n";
        return;
    }
    cout << "\n--- Student Record ---\n";
    cout << "Roll No    : " << result[0] << "\n";
    cout << "Name       : " << result[1] << "\n";
    cout << "Department : " << result[2] << "\n";
    cout << "Semester   : " << result[3] << "\n";
    cout << "CGPA       : " << result[4] << "\n";
    cout << "Status     : " << result[5] << "\n";
}

void searchStudentByName() {
    char keyword[MAX_LEN];
    cin.ignore();
    cout << "\nEnter Name (or part of name): ";
    cin.getline(keyword, MAX_LEN);
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(STUDENTS_FILE, rows, MAX_ROWS);
    bool found = false;
    for (int i = 0; i < total; i++) {
        char* haystack = rows[i][1];
        char* needle = keyword;
        int hl = strlen(haystack), nl = strlen(needle);
        bool matched = false;
        for (int j = 0; j <= hl - nl && !matched; j++) {
            bool ok = true;
            for (int k = 0; k < nl && ok; k++) {
                char h = haystack[j + k], n = needle[k];
                if (h >= 'A' && h <= 'Z') h += 32;
                if (n >= 'A' && n <= 'Z') n += 32;
                if (h != n) ok = false;
            }
            if (ok) matched = true;
        }
        if (matched) {
            if (!found) cout << "\n--- Search Results ---\n";
            cout << rows[i][0] << " | " << rows[i][1] << " | " << rows[i][2] 
                 << " | CGPA: " << rows[i][4] << " | " << rows[i][5] << "\n";
            found = true;
        }
    }
    if (!found) cout << "No students found!\n";
}

void updateStudent() {
    char roll[MAX_LEN];
    cout << "\nEnter Roll No to update: ";
    cin >> roll;
    if (!checkRecordExists(STUDENTS_FILE, 0, roll)) {
        cout << "Student not found!\n";
        return;
    }
    cout << "What to update?\n1. Name\n2. Department\n3. Semester\n4. CGPA\nChoice: ";
    int choice;
    cin >> choice;
    cin.ignore();
    char newVal[MAX_LEN];
    int col = 0;
    switch (choice) {
        case 1: cout << "New Name: "; cin.getline(newVal, MAX_LEN); col = 1; break;
        case 2: cout << "New Department: "; cin.getline(newVal, MAX_LEN); col = 2; break;
        case 3: cout << "New Semester: "; cin.getline(newVal, MAX_LEN); col = 3; break;
        case 4: cout << "New CGPA: "; cin >> newVal; col = 4; break;
        default: cout << "Invalid choice!\n"; return;
    }
    updateFile(STUDENTS_FILE, 0, roll, col, newVal);
    cout << "Student updated successfully!\n";
}

void softDeleteStudent() {
    char roll[MAX_LEN];
    cout << "\nEnter Roll No to deactivate: ";
    cin >> roll;
    if (!checkRecordExists(STUDENTS_FILE, 0, roll)) {
        cout << "Student not found!\n";
        return;
    }
    updateFile(STUDENTS_FILE, 0, roll, 5, "inactive");
    cout << "Student marked as inactive!\n";
}

void listActiveStudents() {
    char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(STUDENTS_FILE, rows, MAX_ROWS);
    char active[MAX_ROWS][MAX_COLS][MAX_LEN];
    int ac = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][5], "active") == 0) {
            for (int j = 0; j < MAX_COLS; j++)
                strncpy(active[ac][j], rows[i][j], MAX_LEN - 1);
            ac++;
        }
    }
    // Selection sort
    for (int i = 0; i < ac - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < ac; j++) {
            if (strcmp(active[j][0], active[minIdx][0]) < 0) minIdx = j;
        }
        if (minIdx != i) {
            char tmp[MAX_COLS][MAX_LEN];
            for (int k = 0; k < MAX_COLS; k++) {
                strncpy(tmp[k], active[i][k], MAX_LEN - 1);
                strncpy(active[i][k], active[minIdx][k], MAX_LEN - 1);
                strncpy(active[minIdx][k], tmp[k], MAX_LEN - 1);
            }
        }
    }
    cout << "\n--- Active Students ---\n";
    printf("%-14s %-20s %-25s %-6s %-6s\n", "Roll No", "Name", "Department", "Sem", "CGPA");
    cout << string(78, '-') << "\n";
    for (int i = 0; i < ac; i++) {
        printf("%-14s %-20s %-25s %-6s %-6s\n", active[i][0], active[i][1], active[i][2], active[i][3], active[i][4]);
    }
    cout << "Total active students: " << ac << "\n";
}