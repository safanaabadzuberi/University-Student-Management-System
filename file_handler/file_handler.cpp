#include "file_handler.h"
#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;

void splitLine(const char* line, char fields[][MAX_LEN], int& count) {
    count = 0;
    int fi = 0;
    bool inQuote = false;
    for (int i = 0; line[i] != '\0' && line[i] != '\n' && line[i] != '\r'; i++) {
        char c = line[i];
        if (c == '"') {
            inQuote = !inQuote;
        } else if (c == ',' && !inQuote) {
            fields[count][fi] = '\0';
            count++;
            fi = 0;
        } else {
            if (fi < MAX_LEN - 1) {
                fields[count][fi++] = c;
            }
        }
    }
    fields[count][fi] = '\0';
    count++;
}

void joinFields(char fields[][MAX_LEN], int count, char* out, char delim) {
    int oi = 0;
    for (int i = 0; i < count; i++) {
        bool needQuote = false;
        for (int j = 0; fields[i][j] != '\0'; j++) {
            if (fields[i][j] == delim) { needQuote = true; break; }
        }
        if (needQuote) out[oi++] = '"';
        for (int j = 0; fields[i][j] != '\0'; j++) out[oi++] = fields[i][j];
        if (needQuote) out[oi++] = '"';
        if (i < count - 1) out[oi++] = delim;
    }
    out[oi++] = '\n';
    out[oi]   = '\0';
}

int readFile(const char* filename, char rows[][MAX_COLS][MAX_LEN], int maxRows) {
    ifstream fin(filename);
    if (!fin.is_open()) return 0;
    char line[MAX_COLS * MAX_LEN];
    int rowCount = 0;
    bool firstLine = true;
    while (fin.getline(line, sizeof(line))) {
        if (firstLine) { firstLine = false; continue; }
        if (line[0] == '\0') continue;
        int count = 0;
        splitLine(line, rows[rowCount], count);
        if (count > 0 && rowCount < maxRows) rowCount++;
    }
    fin.close();
    return rowCount;
}

void writeFile(const char* filename, char rows[][MAX_COLS][MAX_LEN],
               int rowCount, int colCount) {
    ofstream fout(filename);
    if (!fout.is_open()) return;
    char buf[MAX_COLS * MAX_LEN];
    for (int i = 0; i < rowCount; i++) {
        joinFields(rows[i], colCount, buf, ',');
        fout << buf;
    }
    fout.close();
}

void appendFile(const char* filename, char fields[][MAX_LEN], int fieldCount) {
    ofstream fout(filename, ios::app);
    if (!fout.is_open()) return;
    char buf[MAX_COLS * MAX_LEN];
    joinFields(fields, fieldCount, buf, ',');
    fout << buf;
    fout.close();
}

void updateFile(const char* filename, int keyCol, const char* keyVal,
                int targetCol, const char* newVal) {
    static char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(filename, rows, MAX_ROWS);
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][keyCol], keyVal) == 0) {
            strncpy(rows[i][targetCol], newVal, MAX_LEN - 1);
            rows[i][targetCol][MAX_LEN - 1] = '\0';
        }
    }
    ifstream fin(filename);
    char headerLine[MAX_COLS * MAX_LEN] = "";
    if (fin.is_open()) {
        fin.getline(headerLine, sizeof(headerLine));
        fin.close();
    }
    ofstream fout(filename);
    if (!fout.is_open()) return;
    fout << headerLine << "\n";
    char buf[MAX_COLS * MAX_LEN];
    for (int i = 0; i < total; i++) {
        int c = 0;
        for (c = 0; c < MAX_COLS; c++) {
            if (rows[i][c][0] == '\0') break;
        }
        if (c == 0) c = 1;
        joinFields(rows[i], c, buf, ',');
        fout << buf;
    }
    fout.close();
}

void deleteRecord(const char* filename, int keyCol, const char* keyVal) {
    static char rows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(filename, rows, MAX_ROWS);
    ifstream fin(filename);
    char headerLine[MAX_COLS * MAX_LEN] = "";
    if (fin.is_open()) {
        fin.getline(headerLine, sizeof(headerLine));
        fin.close();
    }
    ofstream fout(filename);
    if (!fout.is_open()) return;
    fout << headerLine << "\n";
    char buf[MAX_COLS * MAX_LEN];
    for (int i = 0; i < total; i++) {
        if (strcmp(rows[i][keyCol], keyVal) != 0) {
            int c = 0;
            for (c = 0; c < MAX_COLS; c++) {
                if (rows[i][c][0] == '\0') break;
            }
            if (c == 0) c = 1;
            joinFields(rows[i], c, buf, ',');
            fout << buf;
        }
    }
    fout.close();
}

bool checkRecordExists(const char* filename, int keyCol, const char* keyVal) {
    ifstream fin(filename);
    if (!fin.is_open()) return false;
    char line[MAX_COLS * MAX_LEN];
    bool first = true;
    while (fin.getline(line, sizeof(line))) {
        if (first) { first = false; continue; }
        char fields[MAX_COLS][MAX_LEN];
        int count = 0;
        splitLine(line, fields, count);
        if (keyCol < count && strcmp(fields[keyCol], keyVal) == 0) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

int findRow(const char* filename, int keyCol, const char* keyVal,
            char result[][MAX_LEN], int maxCols) {
    ifstream fin(filename);
    if (!fin.is_open()) return 0;
    char line[MAX_COLS * MAX_LEN];
    bool first = true;
    while (fin.getline(line, sizeof(line))) {
        if (first) { first = false; continue; }
        char fields[MAX_COLS][MAX_LEN];
        int count = 0;
        splitLine(line, fields, count);
        if (keyCol < count && strcmp(fields[keyCol], keyVal) == 0) {
            int copy = count < maxCols ? count : maxCols;
            for (int i = 0; i < copy; i++) {
                strncpy(result[i], fields[i], MAX_LEN - 1);
                result[i][MAX_LEN - 1] = '\0';
            }
            fin.close();
            return count;
        }
    }
    fin.close();
    return 0;
}