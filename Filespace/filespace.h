#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
using namespace std;

const int MAX_ROWS = 500;
const int MAX_COLS = 10;

// reads file
bool readTXT(string filename, string data[][MAX_COLS], int &rowCount, int &colCount);

// writes file
bool writeTXT(string filename, string header[], int headerCount,
              string data[][MAX_COLS], int rowCount, int colCount);

// appends row
bool appendTXT(string filename, string row[], int colCount);

// finds row
bool findRow(string filename, int colIndex, string value,
             string resultRow[], int &resultColCount);

// checks existence
bool rowExists(string filename, int colIndex, string value);

#endif