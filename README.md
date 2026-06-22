# Campus Analytics Engine — BS AI

A robust, multi-file, menu-driven data analytics system designed for university management, focusing on C++ procedural programming fundamentals.

## Project Structure
```text
University-Student-Management-System/
├── main.cpp
├── file_handler/     (file_handler.h / .cpp)
├── student_ops/      (student_ops.h / .cpp)
├── course_ops/       (course_ops.h / .cpp)
├── attendance/       (attendance.h / .cpp)
├── grades/           (grades.h / .cpp)
├── fee_tracker/      (fee_tracker.h / .cpp)
├── reports/          (reports.h / .cpp)
└── *.txt             (Data storage files)

```

## How to Compile

Use the following command in your terminal to link all modules:

```bash
g++ -std=c++11 main.cpp \
    file_handler/file_handler.cpp \
    student_ops/student_ops.cpp \
    course_ops/course.cpp \
    attendance/attendance.cpp \
    grades/grades.cpp \
    fee_tracker/fee_tracker.cpp \
    reports/reports.cpp \
    -o campus_engine

```

## How to Run

```bash
./campus_engine

```

*Note: Ensure all `.txt` data files are present in the same directory as the executable.*

## Features

* **Student Management:** Add, search, update, soft-delete, and list active students.
* **Course Management:** Enrollment and dropping with validation checks.
* **Attendance:** Mark session presence, compute percentages, and generate shortage lists.
* **Grades:** Calculate marks, letter grades, and semester GPA.
* **Fee Tracker:** Payment recording, late fine calculation (2%/week), and receipt generation.
* **Reports Module:** - Comprehensive Student Summary Reports.
* Course Performance Analysis.
* Semester Academic Summaries.
* Merit Lists and Defaulter Tracking.



## Constraints

* **Paradigm:** Procedural Programming only.
* **Allowed:** Arrays, Strings, Loops, Functions, File I/O.
* **Forbidden:** No STL algorithms, No Classes, No `<ctime>`, No `<map>`.

```

```
