#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include "../file_handler/file_handler.h"

#define ATTENDANCE_FILE "attendance_log.txt"

void   markAttendance(const char* courseCode, const char* date, const char* semester);
double getAttendancePct(const char* roll, const char* courseCode);
void   getShortageList(const char* courseCode);
bool   undoLastSession(const char* courseCode, const char* date);
void   printDailySheet(const char* courseCode, const char* date);

void markAttendanceMenu();
void attendancePercentMenu();
void shortageListMenu();
void undoSessionMenu();
void dailySheetMenu();

#endif
