#include "File_exe.h"

void displaySystemMessage(char* message, int waitTime) {
    clearTerminal();
    printf(message);
    sleep(waitTime);
    clearTerminal();
}

char* getUserID(char* username) {
    struct dataContainer2D users = queryFieldStrict("Patient_IDs", "name", username);

    if (users.error) {
        return "ERR";
    }

    return users.data[0][0]; // UserID
}

void displayTabulatedData(struct dataContainer2D data) {
    int tableLength = 0;
    int numRow = data.y + 1;
    char** displayedStrings = malloc (numRow * sizeof(char*));

    // Find Max Column Lengths
    int columnLengths[data.x];

    for (int i=0; i<data.x; i++) {
        int columnLength = strlen(data.fields[i]);

        for (int j=0; j<data.y; j++) {
            int currentLength = strlen(data.data[j][i]);

            if (currentLength > columnLength) {
                columnLength = currentLength;
            }
        }

        columnLengths[i] = columnLength;
    }

    int bufferLength = 256;
    char stringBuffer[bufferLength];

    // Format the Field Strings
    for (int i=0; i<data.x; i++) {
        int totalPadding = columnLengths[i] - strlen(data.fields[i]) + 10;

        int leftPadding = floor(totalPadding/2);
        
        for (int n=0; n<leftPadding; n++) {
            strncat(stringBuffer, " ", 2);
        } 

        strncat(stringBuffer, data.fields[i], strlen(data.fields[i]));

        int rightPadding = totalPadding - leftPadding;

        for (int n=0; n<rightPadding; n++) {
            strncat(stringBuffer, " ", 2);
        } 

        strncat(stringBuffer, "|", 2);
    }

    displayedStrings[0] = strdup(stringBuffer);
    int rowLength = strlen(stringBuffer);
    if (rowLength > tableLength) {
        tableLength = rowLength;
    }

    // Format the Strings

    for (int i=0; i<data.y; i++) {
        stringBuffer[0] = '\0';

        for (int j=0; j<data.x; j++) {
            int totalPadding = columnLengths[j] - strlen(data.data[i][j]) + 10;

            int leftPadding = floor(totalPadding/2);
            
            for (int n=0; n<leftPadding; n++) {
                strncat(stringBuffer, " ", 2);
            } 

            strncat(stringBuffer, data.data[i][j], strlen(data.data[i][j]));

            int rightPadding = totalPadding - leftPadding;

            for (int n=0; n<rightPadding; n++) {
                strncat(stringBuffer, " ", 2);
            } 

            strncat(stringBuffer, "|", 2);
        }

        displayedStrings[i+1] = strdup(stringBuffer);
        int rowLength = strlen(stringBuffer);
        if (rowLength > tableLength) {
            tableLength = rowLength;
        }
    }

    for (int i=0; i<tableLength; i++) {
        printf("-");
    }

    for (int i=0; i<numRow; i++) {
        printf("\n|%s\n", displayedStrings[i]);
        
        for (int i=0; i<tableLength; i++) {
            printf("-");
        }
    }

    free(displayedStrings);
}

void displayAppointments(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    if (appointments.error) {
        displaySystemMessage("Unable to Access Appointments...", 5);
        return;
    }

    clearTerminal();
    displayTabulatedData(appointments);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN");
}

void rescheduleAppointmentsMenu(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    if (appointments.error) {
        displaySystemMessage("Unable to Access Appointments...", 5);
        return;
    }
    
    char* header = "Cancel Appointment Menu";

    // Format the strings
    char* options[] = {};
    int noOptions = 4;

    displayMenu(header, options, noOptions);
}

void cancelAppointmentsMenu(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    if (appointments.error) {
        displaySystemMessage("Unable to Access Appointments...", 5);
        return;
    }

    char* header = "Cancel Appointment Menu";

    // Format the strings
    char* options[] = {};
    int noOptions = 4;

    displayMenu(header, options, noOptions);
}

void appointmentMenu(char* userID) {
    char* header = "Appointment Menu";
    char* options[] = {"View Appointments", "Reschedule Appointments", "Cancel Appointments", "Back"};
    int noOptions = 4;

    while (1) {
        int result = displayMenu(header, options, noOptions);

        if (result == 1) {
            displayAppointments(userID);
        } else if (result == 2) {
            rescheduleAppointmentsMenu(userID);
        } else if (result == 3) {
            cancelAppointmentsMenu(userID);
        } else if (result == 4) {
            return;
        }
    }

}

void displayEHR(char* userID) {
    struct dataContainer2D records = queryFieldStrict("EHR", "PatientUserID", userID);

    if (records.error) {
        displaySystemMessage("Unable to Access Bills...", 5);
        return;
    }
}

void displayBill(char* userID) {
    struct dataContainer2D bills = queryFieldStrict("Bills", "PatientUserID", userID);

    if (bills.error) {
        displaySystemMessage("Unable to Access Bills...", 5);
        return;
    }
}

void searchBill(char* userID) {
    struct dataContainer2D bills = queryFieldStrict("Bills", "PatientUserID", userID);

    if (bills.error) {
        displaySystemMessage("Unable to Access Bills...", 5);
        return;
    }
}

void billsMenu(char* userID) {
    char* header = "Bills Menu";
    char* options[] = {"View Bills", "Search Bills", "Back"};
    int noOptions = 3;

    while (1) {
        int result = displayMenu(header, options, noOptions);

        if (result == 1) {
            displayBills(userID);
        } else if (result == 2) {
            searchBills(userID);
        } else if (result == 3) {
            return;
        }
    }
}

int login(char username[], char password[]) {
    struct dataContainer2D userData = queryFieldStrict("Patient_IDs", "name", username);

     if (userData.error || !userData.y) {
        freeMalloc2D(userData);
        displaySystemMessage("Username not Found", 5);
        return 1;
     }

     for (int i=0; i<userData.y; i++) {
        for (int j=0; j<userData.x; j++) {
            if (!strncmp(password, userData.data[i][j], 256)) {
                freeMalloc2D(userData);
                displaySystemMessage("Login Sucessful", 5);

                return 0;
            }
        }
     }

    freeMalloc2D(userData);
    displaySystemMessage("Incorrect Password", 5);
    return 1;
}

int main() {
    // // LOGIN
    int bufferLength = 256;
    char username[bufferLength], password[bufferLength];
    printf("Enter your Username: ");
    fgets(username, 256, stdin);

    printf("Enter your Password: ");
    fgets(password, 256, stdin);

    int success = login(username, password);

    if (!success) {
        return 1;
    }

    char* userID = getUserID(username);

    // DEFAULT MENU
    char* header = "Patient Menu";
    char* options[] = {"Appointment", "Electronic Health Records", "Billing", "Logout"};
    int noOptions = 4;
    int result = displayMenu(header, options, noOptions);

    if (result == 1) {
        appointmentMenu(userID);
    } else if (result == 2) {
        displayEHR(userID);
    } else if (result == 3) {
        displayBills(userID);
    } else if (result == 4) {
        return 0;
    }

// •	Appointment Management - View, reschedule, and cancel appointments electronically.


// •	Electronic Health Records (EHR) Access (view only): Secure access to a portion of their medical history, prescriptions, appointment history, and billing information.


// •	Billing Management (view and search) - billing details and manage insurance claims.

}