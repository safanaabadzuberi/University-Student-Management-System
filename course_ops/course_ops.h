#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include "../file_handler/file_handler.h"

#define COURSES_FILE     "courses.txt"
#define ENROLLMENTS_FILE "enrollments.txt"
#define GRADES_FILE      "grades.txt"

enum EnrollResult {
    ENROLL_SUCCESS,
    ENROLL_STUDENT_INACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_NO_SEATS,
    ENROLL_ALREADY_ENROLLED,
    ENROLL_CREDIT_OVERLOAD,
    ENROLL_PREREQ_NOT_MET
};

EnrollResult enrollStudent(const char* roll, const char* courseCode, const char* semester);
void         dropCourse(const char* roll, const char* courseCode, const char* semester);
int          getCreditLoad(const char* roll, const char* semester);
bool         checkPrerequisite(const char* roll, const char* courseCode);
void         listEnrolledStudents(const char* courseCode);

void addCourse();
void enrollStudentMenu();
void dropCourseMenu();
void listCoursesMenu();
void listEnrolledMenu();
void enrollResultMessage(EnrollResult r);

#endif
