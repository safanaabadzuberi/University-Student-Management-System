#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include "../file_handler/file_handler.h"

#define FEES_FILE "fees.txt"

int    daysBetween(const char* d1, const char* d2);
double computeLateFine(const char* roll, const char* semester);
void   recordPayment(const char* roll, const char* semester,
                     double amount, const char* payDate, const char* method);
void   generateReceipt(const char* roll, const char* semester);
void   getDefaulters();

void recordPaymentMenu();
void lateFineMenu();
void receiptMenu();
void defaultersMenu();
void addFeeRecordMenu();

#endif
