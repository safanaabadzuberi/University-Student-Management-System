#include <iostream>
#include <cstring>
#include "student_ops/student_ops.h"
#include "course_ops/course_ops.h"
#include "attendance/attendance.h"
#include "grades/grades.h"
#include "fee_tracker/fee_tracker.h"
#include "reports/reports.h" // Reports module integrated

using namespace std;

// ─── MENUS ────────────────────────────────────────────────────────────────────
static void studentMenu() {
    int ch;
    do {
        cout << "\n=== STUDENT MANAGEMENT ===\n"
             << "1. Add Student\n"
             << "2. Search by Roll No\n"
             << "3. Search by Name\n"
             << "4. Update Student\n"
             << "5. Deactivate Student\n"
             << "6. List Active Students\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: addStudent();         break;
            case 2: searchStudentByRoll(); break;
            case 3: searchStudentByName(); break;
            case 4: updateStudent();       break;
            case 5: softDeleteStudent();   break;
            case 6: listActiveStudents();  break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
}

static void courseMenu() {
    int ch;
    do {
        cout << "\n=== COURSE MANAGEMENT ===\n"
             << "1. Add Course\n"
             << "2. List All Courses\n"
             << "3. Enroll Student\n"
             << "4. Drop Course\n"
             << "5. List Enrolled Students\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: addCourse();         break;
            case 2: listCoursesMenu();   break;
            case 3: enrollStudentMenu(); break;
            case 4: dropCourseMenu();    break;
            case 5: listEnrolledMenu();  break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
}

static void attendanceMenu() {
    int ch;
    do {
        cout << "\n=== ATTENDANCE ===\n"
             << "1. Mark Attendance\n"
             << "2. View Attendance %\n"
             << "3. Shortage List\n"
             << "4. Undo Last Session\n"
             << "5. Daily Sheet\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: markAttendanceMenu();    break;
            case 2: attendancePercentMenu(); break;
            case 3: shortageListMenu();      break;
            case 4: undoSessionMenu();       break;
            case 5: dailySheetMenu();        break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
}

static void gradesMenu() {
    int ch;
    do {
        cout << "\n=== GRADES ===\n"
             << "1. Enter Marks\n"
             << "2. Grade Report\n"
             << "3. Class Statistics\n"
             << "4. Semester GPA\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: enterMarksMenu();    break;
            case 2: showGradeReportMenu();break;
            case 3: classStatsMenu();    break;
            case 4: semesterGPAMenu();   break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
}

static void feeMenu() {
    int ch;
    do {
        cout << "\n=== FEE TRACKER ===\n"
             << "1. Add Fee Record\n"
             << "2. Record Payment\n"
             << "3. Compute Late Fine\n"
             << "4. Generate Receipt\n"
             << "5. Fee Defaulters\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: addFeeRecordMenu();   break;
            case 2: recordPaymentMenu();  break;
            case 3: lateFineMenu();       break;
            case 4: receiptMenu();        break;
            case 5: defaultersMenu();     break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
}

static void reportsMenu() {
    int ch;
    do {
        cout << "\n=== REPORTS ===\n"
             << "1. Student Summary Report\n"
             << "2. Course Performance\n"
             << "3. Semester Academic Summary\n"
             << "4. Fee Defaulters\n"
             << "0. Back\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: studentSummaryMenu();   break;
            case 2: coursePerformanceMenu(); break;
            case 3: semesterSummaryMenu();  break;
            case 4: defaultersMenu();       break;
            case 0: break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
}

int main() {
    cout << "========================================\n";
    cout << "   CAMPUS ANALYTICS ENGINE — BS AI\n";
    cout << "========================================\n";
    int ch;
    do {
        cout << "\n=== MAIN MENU ===\n"
             << "1. Student Management\n"
             << "2. Course Management\n"
             << "3. Attendance\n"
             << "4. Grades\n"
             << "5. Fee Tracker\n"
             << "6. Reports\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: studentMenu();   break;
            case 2: courseMenu();    break;
            case 3: attendanceMenu();break;
            case 4: gradesMenu();    break;
            case 5: feeMenu();       break;
            case 6: reportsMenu();   break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (ch != 0);
    return 0;
}