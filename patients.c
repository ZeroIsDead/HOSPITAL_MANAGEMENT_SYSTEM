#include "File_exe.h"

int login(char* username, char* password, char* role) {
        
}

int main() {
    // DEFAULT MENU
    char* header = "Patient Menu";
    char* options[] = {"Appointment", "Electronic Health Records", "Billing"};
    int noOptions = 3;
    char* result = displayMenu(header, options, noOptions);
    printf(result);

// •	Login and logout from the system.


// •	Appointment Management - View, reschedule, and cancel appointments electronically.


// •	Electronic Health Records (EHR) Access (view only): Secure access to a portion of their medical history, prescriptions, appointment history, and billing information.


// •	Billing Management (view and search) - billing details and manage insurance claims.

}