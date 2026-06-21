#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

const int MAX_ROWS = 500;
const int MAX_COLS = 10;
const int MAX_LEN  = 200;

int  readFile(const char* filename, char rows[][MAX_COLS][MAX_LEN], int maxRows);
void writeFile(const char* filename, char rows[][MAX_COLS][MAX_LEN], int rowCount, int colCount);
void appendFile(const char* filename, char fields[][MAX_LEN], int fieldCount);
void updateFile(const char* filename, int keyCol, const char* keyVal,
                int targetCol, const char* newVal);
void deleteRecord(const char* filename, int keyCol, const char* keyVal);
bool checkRecordExists(const char* filename, int keyCol, const char* keyVal);
int  findRow(const char* filename, int keyCol, const char* keyVal,
             char result[][MAX_LEN], int maxCols);
void splitLine(const char* line, char fields[][MAX_LEN], int& count);
void joinFields(char fields[][MAX_LEN], int count, char* out, char delim);

#endif
