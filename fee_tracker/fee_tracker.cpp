#include "fee_tracker.h"
#include "../student_ops/student_ops.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <iomanip>

using namespace std;

static int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

static bool isLeap(int y) {
    return (y%4==0 && y%100!=0) || (y%400==0);
}

static int toDays(const char* date) {
    int dd = (date[0]-'0')*10 + (date[1]-'0');
    int mm = (date[3]-'0')*10 + (date[4]-'0');
    int yyyy = (date[6]-'0')*1000 + (date[7]-'0')*100 +
               (date[8]-'0')*10   + (date[9]-'0');
    int days = 0;
    for (int y = 1; y < yyyy; y++) {
        days += isLeap(y) ? 366 : 365;
    }
    for (int m = 1; m < mm; m++) {
        days += monthDays[m-1];
        if (m == 2 && isLeap(yyyy)) days++;
    }
    days += dd;
    return days;
}

static bool validateDate(const char* d) {
    if (strlen(d) != 10) return false;
    if (d[2]!='-' || d[5]!='-') return false;
    for (int i=0;i<10;i++) {
        if (i==2||i==5) continue;
        if (d[i]<'0'||d[i]>'9') return false;
    }
    return true;
}

int daysBetween(const char* d1, const char* d2) {
    return toDays(d2) - toDays(d1);
}

double computeLateFine(const char* roll, const char* semester) {
    char feeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(FEES_FILE, feeRows, MAX_ROWS);
    for (int i = 0; i < total; i++) {
        if (strcmp(feeRows[i][1], roll)==0 && strcmp(feeRows[i][2], semester)==0) {
            if (feeRows[i][6][0] == '\0' || strcmp(feeRows[i][6],"N/A")==0)
                return 0.0;
            int diff = daysBetween(feeRows[i][5], feeRows[i][6]);
            if (diff <= 0) return 0.0;
            double totalFee = 0.0;
            int j=0; bool dec=false; double d=0.1;
            for (; feeRows[i][3][j]!='\0'; j++) {
                char c = feeRows[i][3][j];
                if (c=='.') { dec=true; continue; }
                if (c>='0'&&c<='9') {
                    if (!dec) totalFee=totalFee*10+(c-'0');
                    else { totalFee+=(c-'0')*d; d*=0.1; }
                }
            }
            int weeks = diff / 7;
            return totalFee * 0.02 * weeks;
        }
    }
    return 0.0;
}

void recordPayment(const char* roll, const char* semester,
                   double amount, const char* payDate, const char* method) {
    if (!validateDate(payDate)) {
        cout << "Invalid date format! Use DD-MM-YYYY\n";
        return;
    }
    char feeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(FEES_FILE, feeRows, MAX_ROWS);
    bool found = false;
    for (int i = 0; i < total; i++) {
        if (strcmp(feeRows[i][1], roll)==0 && strcmp(feeRows[i][2], semester)==0) {
            char amtStr[MAX_LEN];
            sprintf(amtStr, "%.0f", amount);
            updateFile(FEES_FILE, 1, roll, 4, amtStr);
            updateFile(FEES_FILE, 1, roll, 6, payDate);
            updateFile(FEES_FILE, 1, roll, 7, method);
            int diff = daysBetween(feeRows[i][5], payDate);
            updateFile(FEES_FILE, 1, roll, 8, diff > 0 ? "paid_late" : "paid");
            found = true;
            break;
        }
    }
    if (!found) cout << "Fee record not found!\n";
    else        cout << "Payment recorded!\n";
}

void generateReceipt(const char* roll, const char* semester) {
    char feeRow[MAX_COLS][MAX_LEN];
    char feeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(FEES_FILE, feeRows, MAX_ROWS);
    int found = -1;
    for (int i = 0; i < total; i++) {
        if (strcmp(feeRows[i][1], roll)==0 && strcmp(feeRows[i][2], semester)==0) {
            for (int k=0;k<MAX_COLS;k++) strncpy(feeRow[k], feeRows[i][k], MAX_LEN-1);
            found = i;
            break;
        }
    }
    if (found < 0) { cout << "No fee record found!\n"; return; }
    double totalFee=0, paid=0;
    for (int k=0; feeRow[3][k]!='\0'; k++) {
        char c=feeRow[3][k];
        if (c>='0'&&c<='9') totalFee=totalFee*10+(c-'0');
    }
    bool dec=false; double d=0.1;
    for (int k=0; feeRow[4][k]!='\0'; k++) {
        char c=feeRow[4][k];
        if (c=='.'){dec=true;continue;}
        if(c>='0'&&c<='9'){if(!dec)paid=paid*10+(c-'0');else{paid+=(c-'0')*d;d*=0.1;}}
    }
    double fine    = computeLateFine(roll, semester);
    double balance = totalFee + fine - paid;
    char sRow[MAX_COLS][MAX_LEN];
    findRow(STUDENTS_FILE, 0, roll, sRow, MAX_COLS);
    cout << "\n" << string(50,'=') << "\n";
    cout << setw(35) << "FEE RECEIPT\n";
    cout << string(50,'-') << "\n";
    cout << left << setw(20) << "Student:" << sRow[1] << "\n";
    cout << setw(20) << "Roll No:"    << roll    << "\n";
    cout << setw(20) << "Semester:"   << semester << "\n";
    cout << setw(20) << "Due Date:"   << feeRow[5]<< "\n";
    cout << setw(20) << "Paid Date:"  << feeRow[6]<< "\n";
    cout << string(50,'-') << "\n";
    cout << setw(35) << "Tuition Fee:" << right << setw(10) << totalFee << "\n";
    cout << left  << setw(35) << "Late Fine:" << right << setw(10) << fine << "\n";
    cout << left  << setw(35) << "Total Due:" << right << setw(10) << (totalFee+fine) << "\n";
    cout << left  << setw(35) << "Amount Paid:" << right << setw(10) << paid << "\n";
    cout << string(50,'-') << "\n";
    cout << left  << setw(35) << "Balance:" << right << setw(10) << balance << "\n";
    cout << string(50,'=') << "\n";
}

void getDefaulters() {
    char feeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(FEES_FILE, feeRows, MAX_ROWS);
    char dRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    double dBal[MAX_ROWS];
    int dc = 0;
    for (int i = 0; i < total; i++) {
        if (strcmp(feeRows[i][8], "paid")==0) continue;
        double totalFee=0, paid=0;
        for (int k=0; feeRows[i][3][k]!='\0'; k++) {
            char c=feeRows[i][3][k];
            if (c>='0'&&c<='9') totalFee=totalFee*10+(c-'0');
        }
        bool dec=false; double d=0.1;
        for (int k=0; feeRows[i][4][k]!='\0'; k++) {
            char c=feeRows[i][4][k];
            if (c=='.'){dec=true;continue;}
            if(c>='0'&&c<='9'){if(!dec)paid=paid*10+(c-'0');else{paid+=(c-'0')*d;d*=0.1;}}
        }
        double fine = computeLateFine(feeRows[i][1], feeRows[i][2]);
        double bal  = totalFee + fine - paid;
        if (bal > 0) {
            for (int k=0;k<MAX_COLS;k++) strncpy(dRows[dc][k], feeRows[i][k], MAX_LEN-1);
            dBal[dc] = bal;
            dc++;
        }
    }
    for (int i = 0; i < dc-1; i++) {
        for (int j = 0; j < dc-i-1; j++) {
            if (dBal[j] < dBal[j+1]) {
                double tmp = dBal[j]; dBal[j]=dBal[j+1]; dBal[j+1]=tmp;
                char tmpR[MAX_COLS][MAX_LEN];
                for (int k=0;k<MAX_COLS;k++) {
                    strncpy(tmpR[k], dRows[j][k], MAX_LEN-1);
                    strncpy(dRows[j][k],dRows[j+1][k], MAX_LEN-1);
                    strncpy(dRows[j+1][k], tmpR[k], MAX_LEN-1);
                }
            }
        }
    }
    cout << "\n--- Fee Defaulters ---\n";
    printf("%-14s %-6s %-12s %-12s\n", "Roll No","Sem","Balance","Fine");
    cout << string(50,'-') << "\n";
    for (int i = 0; i < dc; i++) {
        int diff = 0;
        if (dRows[i][6][0] != '\0' && strcmp(dRows[i][6],"N/A")!=0)
            diff = daysBetween(dRows[i][5], dRows[i][6]);
        else
            diff = 0;
        printf("%-14s %-6s %-12.2f %-12d wks overdue\n",
                dRows[i][1], dRows[i][2], dBal[i], diff/7);
    }
}

void recordPaymentMenu() {
    char roll[MAX_LEN], sem[MAX_LEN], date[MAX_LEN], method[MAX_LEN];
    double amount;
    cout << "\n--- Record Payment ---\n";
    cout << "Roll No: ";        cin >> roll;
    cout << "Semester: ";        cin >> sem;
    cout << "Amount Paid: ";     cin >> amount;
    cout << "Payment Date (DD-MM-YYYY): "; cin >> date;
    cin.ignore();
    cout << "Payment Method: ";  cin.getline(method, MAX_LEN);
    recordPayment(roll, sem, amount, date, method);
}

void lateFineMenu() {
    char roll[MAX_LEN], sem[MAX_LEN];
    cout << "\nRoll No: ";  cin >> roll;
    cout << "Semester: ";   cin >> sem;
    double fine = computeLateFine(roll, sem);
    printf("Late Fine: %.2f PKR\n", fine);
}

void receiptMenu() {
    char roll[MAX_LEN], sem[MAX_LEN];
    cout << "\nRoll No: ";  cin >> roll;
    cout << "Semester: ";   cin >> sem;
    generateReceipt(roll, sem);
}

void defaultersMenu() {
    getDefaulters();
}

void addFeeRecordMenu() {
    char roll[MAX_LEN], sem[MAX_LEN], totalFee[MAX_LEN], dueDate[MAX_LEN];
    cout << "\n--- Add Fee Record ---\n";
    cout << "Roll No: ";               cin >> roll;
    cout << "Semester: ";              cin >> sem;
    cout << "Total Fee: ";             cin >> totalFee;
    cout << "Due Date (DD-MM-YYYY): "; cin >> dueDate;
    char feeRows[MAX_ROWS][MAX_COLS][MAX_LEN];
    int total = readFile(FEES_FILE, feeRows, MAX_ROWS);
    char fid[MAX_LEN];
    sprintf(fid, "F%03d", total + 1);
    char fields[MAX_COLS][MAX_LEN];
    strncpy(fields[0], fid,      MAX_LEN-1);
    strncpy(fields[1], roll,     MAX_LEN-1);
    strncpy(fields[2], sem,      MAX_LEN-1);
    strncpy(fields[3], totalFee, MAX_LEN-1);
    strncpy(fields[4], "0",      MAX_LEN-1);
    strncpy(fields[5], dueDate,  MAX_LEN-1);
    strncpy(fields[6], "N/A",    MAX_LEN-1);
    strncpy(fields[7], "N/A",    MAX_LEN-1);
    strncpy(fields[8], "unpaid", MAX_LEN-1);
    appendFile(FEES_FILE, fields, 9);
    cout << "Fee record added!\n";
}