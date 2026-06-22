# Campus Analytics Engine — BS DS

A modular, menu-driven University Student Management System built using C++ procedural programming.

## Project Structure
- `file_handler/`: Global file I/O operations.
- `student_ops/`: Student data management.
- `course_ops/`: Enrollment and course logic.
- `attendance/`: Attendance tracking.
- `grades/`: Grade/GPA calculations.
- `fee_tracker/`: Fee and fine management.
- `reports/`: Analytics and summary generation.

## Compilation & Usage

1. Open terminal in the project root.
   
2. Compile: 
   `g++ -std=c++11 main.cpp file_handler/*.cpp student_ops/*.cpp course_ops/*.cpp attendance/*.cpp grades/*.cpp fee_tracker/*.cpp reports/*.cpp -o campus_engine`
   
3. Run: `./campus_engine`

Note: Ensure all `.txt` data files are present in the same directory as the main. 

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
