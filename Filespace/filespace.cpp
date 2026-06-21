#include <iostream>
#include <fstream>
#include "filehandler.h"
using namespace std;

// parses one line
void parseLine(string line, string row[], int &colCount) {
    colCount = 0;
    string field = "";
    bool inQuotes = false;
    int i = 0;
    int len = line.length();

    while (i < len) {
        char ch = line[i];

        if (ch == '"') {
            // toggle quote
            inQuotes = !inQuotes;
        }
        else if (ch == ',' && !inQuotes) {
            // field done
            row[colCount] = field;
            colCount++;
            field = "";
        }
        else {
            // build field
            field = field + ch;
        }
        i++;
    }

    // last field
    row[colCount] = field;
    colCount++;
}

// wraps if needed
string wrapField(string field) {
    bool hasComma = false;
    int i = 0;
    int len = field.length();

    while (i < len) {
        if (field[i] == ',') {
            hasComma = true;
        }
        i++;
    }

    if (hasComma) {
        return "\"" + field + "\"";
    }
    return field;
}

// reads file
bool readTXT(string filename, string data[][MAX_COLS], int &rowCount, int &colCount) {
    ifstream inFile(filename.c_str());

    if (!inFile.is_open()) {
        rowCount = 0;
        colCount = 0;
        return false;
    }

    string line;
    bool isHeader = true;
    rowCount = 0;

    while (getline(inFile, line)) {
        if (line.length() == 0) {
            // skip empty
            continue;
        }

        if (isHeader) {
            // skip header
            string headerRow[MAX_COLS];
            int he
            aderCols = 0;
            parseLine(line, headerRow, headerCols);
            colCount = headerCols;
            isHeader = false;
            continue;
        }

        if (rowCount < MAX_ROWS) {
            string parsedRow[MAX_COLS];
            int parsedCols = 0;
            parseLine(line, parsedRow, parsedCols);

            int j = 0;
            while (j < parsedCols) {
                // copy field
                data[rowCount][j] = parsedRow[j];
                j++;
            }
            rowCount++;
        }
    }

    inFile.close();
    return true;
}

// writes file
bool writeTXT(string filename, string header[], int headerCount,
              string data[][MAX_COLS], int rowCount, int colCount) {
    ofstream outFile(filename.c_str());

    if (!outFile.is_open()) {
        return false;
    }

    int i = 0;
    while (i < headerCount) {
        // write header
        outFile << wrapField(header[i]);
        if (i < headerCount - 1) {
            outFile << ",";
        }
        i++;
    }
    outFile << "\n";

    int r = 0;
    while (r < rowCount) {
        int c = 0;
        while (c < colCount) {
            // write field
            outFile << wrapField(data[r][c]);
            if (c < colCount - 1) {
                outFile << ",";
            }
            c++;
        }
        outFile << "\n";
        r++;
    }

    outFile.close();
    return true;
}

// appends row
bool appendTXT(string filename, string row[], int colCount) {
    ofstream outFile(filename.c_str(), ios::app);

    if (!outFile.is_open()) {
        return false;
    }

    int i = 0;
    while (i < colCount) {
        // write field
        outFile << wrapField(row[i]);
        if (i < colCount - 1) {
            outFile << ",";
        }
        i++;
    }
    outFile << "\n";

    outFile.close();
    return true;
}

// finds row
bool findRow(string filename, int colIndex, string value,
             string resultRow[], int &resultColCount) {
    string data[MAX_ROWS][MAX_COLS];
    int rowCount = 0;
    int colCount = 0;

    bool opened = readTXT(filename, data, rowCount, colCount);

    if (!opened) {
        resultColCount = 0;
        return false;
    }

    int i = 0;
    while (i < rowCount) {
        if (data[i][colIndex] == value) {
            // match found
            int j = 0;
            while (j < colCount) {
                resultRow[j] = data[i][j];
                j++;
            }
            resultColCount = colCount;
            return true;
        }
        i++;
    }

    resultColCount = 0;
    return false;
}

// checks existence
bool rowExists(string filename, int colIndex, string value) {
    string data[MAX_ROWS][MAX_COLS];
    int rowCount = 0;
    int colCount = 0;

    bool opened = readTXT(filename, data, rowCount, colCount);

    if (!opened) {
        return false;
    }

    int i = 0;
    while (i < rowCount) {
        if (data[i][colIndex] == value) {
            // found match
            return true;
        }
        i++;
    }

    return false;
}