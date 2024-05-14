#include "File_exe.h"

void displaySystemMessage(char* message, int waitTime) {
    clearTerminal();
    printf(message);
    sleep(waitTime);
    clearTerminal();
}

char* getUserID(char* username) {
    struct dataContainer2D users = queryFieldStrict("Patient_IDs", "PatientUserID", username);

    if (users.error) {
        return "ERR";
    }

    char* userID = users.data[0][0];
    freeMalloc2D(users);

    return userID;
}

void displayTabulatedData(struct dataContainer2D data) {
    const int minPadding = 5;
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

    // Clear String;
    stringBuffer[0] = '\0';

    // Format the Field Strings
    for (int i=0; i<data.x; i++) {
        int totalPadding = columnLengths[i] - strlen(data.fields[i]) + minPadding;

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

    // Format the Strings

    for (int i=0; i<data.y; i++) {
        stringBuffer[0] = '\0';

        // Create String
        for (int j=0; j<data.x; j++) {
            int totalPadding = columnLengths[j] - strlen(data.data[i][j]) + minPadding;

            // Left Padding
            int leftPadding = floor(totalPadding/2);
            
            for (int n=0; n<leftPadding; n++) {
                strncat(stringBuffer, " ", 2);
            } 

            strncat(stringBuffer, data.data[i][j], strlen(data.data[i][j]));

            // Right Padding
            int rightPadding = totalPadding - leftPadding;

            for (int n=0; n<rightPadding; n++) {
                strncat(stringBuffer, " ", 2);
            } 

            strncat(stringBuffer, "|", 2);
        }

        // Add String to Array
        displayedStrings[i+1] = strdup(stringBuffer);
    }

    int tableLength = strlen(displayedStrings[0]) + 1;

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
        displaySystemMessage("Unable to Access Appointments...", 3);
        return;
    }

    if (!appointments.y) {
        displaySystemMessage("You Have Not Made Any Appointments", 3);
        return;
    }

    clearTerminal();
    displayTabulatedData(appointments);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN");
    freeMalloc2D(appointments);
}

void rescheduleAppointmentsMenu(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    if (appointments.error) {
        freeMalloc2D(appointments);
        displaySystemMessage("Unable to Access Appointments...", 3);
        return;
    }

    if (!appointments.y) {
        freeMalloc2D(appointments);
        displaySystemMessage("You Have Not Made Any Appointments", 3);
        return;
    }
    
    char* header = "Reschedule Appointment Menu";

    // Format the strings
    int noOptions = appointments.y + 1;
    char* options[noOptions];

    for (int i=0; i<noOptions-1; i++) {
        options[i] = appointments.data[i][0];
    }
    options[noOptions-1] = "Back";

    char* secondHeader = "Reschedule";

    char* timeSlotOptions[8] = {"09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
    int secondNoOptions = 6;
    char* secondOptions[secondNoOptions];

    while (1) {
        clearTerminal();
        int input = displayMenu(header, options, noOptions);

        if (input == noOptions) {
            freeMalloc2D(appointments);
            return;
        }

        char* option = options[input-1];

        struct dataContainer1D chosenAppointment = queryKey("Appointments", option);

        clearTerminal();
        char* timeSlot = chosenAppointment.data[4];

        int index = 0;
        for (int i=0; i<7; i++) {
            if (strncmp(timeSlotOptions[i], timeSlot, strlen(timeSlotOptions[i]))) {
                secondOptions[index] = strdup(timeSlotOptions[i]);
                index++;
            }
        }

        input = displayMenu(secondHeader, secondOptions, secondNoOptions);

        char* newTimeSlotStart = secondOptions[input-1];
        char newTimeSlot[256];

        for (int i=0; i<7; i++) {
            if (!strncmp(timeSlotOptions[i], newTimeSlotStart, strlen(timeSlotOptions[i]))) {
                sprintf(newTimeSlot, "%s-%s", newTimeSlotStart, timeSlotOptions[i+1]);
                break;
            }
        }

        char* certain = getString("Are you Certain (Y|N)? ");

        if (!strncmp(certain, "Y", 1)) {
            chosenAppointment.data[4] = strdup(newTimeSlot);

            updateData("Appointments", chosenAppointment.data);

            char confirmMessage[256];
            sprintf(confirmMessage, "Appointment %s Has Successfully been Rescheduled to %s", option, newTimeSlot);
            displaySystemMessage(confirmMessage, 2);
        }
    }
    freeMalloc2D(appointments);
}

void cancelAppointmentsMenu(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    if (appointments.error) {
        freeMalloc2D(appointments);
        displaySystemMessage("Unable to Access Appointments...", 3);
        return;
    }

    if (!appointments.y) {
        freeMalloc2D(appointments);
        displaySystemMessage("You Have Not Made Any Appointments", 3);
        return;
    }

    char* header = "Cancel Appointment Menu";

    // Format the strings
    int noOptions = appointments.y + 1;
    char* options[noOptions];

    for (int i=0; i<noOptions-1; i++) {
        options[i] = appointments.data[i][0];
    }
    options[noOptions-1] = "Back";


    while (1) {
        clearTerminal();
        int input = displayMenu(header, options, noOptions);

        if (input == noOptions) {
            freeMalloc2D(appointments);
            return;
        }

        char* option = options[input-1];

        struct dataContainer2D data = queryFieldStrict("Appointments", "AppointmentID", option);

        clearTerminal();
        displayTabulatedData(data);

        char* certain = getString("Are you Certain (Y|N)? ");

        if (!strncmp(certain, "YES", sizeof(certain))) {
            displaySystemMessage("Appointment Successfully Canceled...", 2);
        }
    }
    freeMalloc2D(appointments);

}

void appointmentMenu(char* userID) {
    char* header = "Appointment Menu";
    char* options[] = {"View Appointments", "Reschedule Appointments", "Cancel Appointments", "Back"};
    int noOptions = 4;

    while (1) {
        clearTerminal();
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
        displaySystemMessage("Unable to Access Electronic Health Records...", 3);
        return;
    }

    if (!records.y) {
        displaySystemMessage("You Have No Records", 3);
        return;
    }

    clearTerminal();
    displayTabulatedData(records);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN");
    freeMalloc2D(records);
}

void displayBills(char* userID) {
    struct dataContainer2D bills = queryFieldStrict("Bills", "PatientUserID", userID);

    if (bills.error) {
        displaySystemMessage("Unable to Access Bills...", 3);
        return;
    }

    if (!bills.y) {
        displaySystemMessage("You Have No Bills", 3);
        return;
    }

    clearTerminal();
    displayTabulatedData(bills);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN");
    freeMalloc2D(bills);
}

void searchBills(char* userID) {
    char* billID = getString("Enter Bill ID: ");

    struct dataContainer2D bills = queryFieldStrict("Bills", "BillID", billID);

    if (bills.error) {
        displaySystemMessage("Unable to Access Bills...", 3);
        return;
    }

    if (!bills.y) {
        displaySystemMessage("Bills Not Found", 3);
        return;
    }

    clearTerminal();
    displayTabulatedData(bills);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN");
    freeMalloc2D(bills);
}

void billsMenu(char* userID) {
    char* header = "Bills Menu";
    char* options[] = {"View Bills", "Search Bills", "Back"};
    int noOptions = 3;

    while (1) {
        clearTerminal();
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

int loginPatient(char username[], char password[]) {
    struct dataContainer2D userData = queryFieldStrict("Patient_IDs", "PatientUserID", username);

    if (userData.error || !userData.y) {
        freeMalloc2D(userData);
        displaySystemMessage("Username not Found", 2);
        return 1;
    }

    int fieldIndex = -1;

    for (int i=0; i<userData.x; i++) {
        if (!strncmp("UserPW", userData.fields[i], 256)) {
            fieldIndex = i;
            break;
        }
    }

    if (fieldIndex == -1) {
        freeMalloc2D(userData);
        displaySystemMessage("FILE ERROR", 2);
        return 1;
    }

    for (int i=0; i<userData.y; i++) {
        printf("%s %s %d\n", password, userData.data[i][fieldIndex], strncmp(password, userData.data[i][fieldIndex], 256));
        if (!strncmp(password, userData.data[i][fieldIndex], 256)) {
            freeMalloc2D(userData);
            displaySystemMessage("Login Sucessful", 2);

            return 0;
        }
    }

    freeMalloc2D(userData);
    // displaySystemMessage("Incorrect Password", 5);
    return 1;
}

int main() {
    // // LOGIN
    clearTerminal();
    char* username = getString("Enter your Username: ");
    char* password = getString("Enter your Password: ");

    int error = loginPatient(username, password);

    if (error) {
        return 1;
    }

    // char* userID = getUserID(username);

    // DEFAULT MENU
    char* header = "Patient Menu";
    char* options[] = {"Appointment", "Electronic Health Records", "Billing", "Logout"};
    int noOptions = 4;

    while (1) {
        clearTerminal();
        int result = displayMenu(header, options, noOptions);

        if (result == 1) {
            appointmentMenu(username);
        } else if (result == 2) {
            displayEHR(username);
        } else if (result == 3) {
            billsMenu(username);
        } else if (result == 4) {
            return 0;
        }
    }

// •	Appointment Management - View, reschedule, and cancel appointments electronically.


// •	Electronic Health Records (EHR) Access (view only): Secure access to a portion of their medical history, prescriptions, appointment history, and billing information.


// •	Billing Management (view and search) - billing details and manage insurance claims.

}