#include "File_exe.h"

char* getUserID(char* username) {
    struct dataContainer2D users = queryFieldStrict("Patient_IDs", "PatientUserID", username);

    if (users.error) {
        return "ERR";
    }

    char* userID = users.data[0][0];
    freeMalloc2D(users);

    return userID;
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

        if (tolower(certain[0]) == 'y') {
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


        if (tolower(certain[0]) == 'y') {
            deleteKey("Appointments", option);
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

void displayAllergies(char* userID) {
    struct dataContainer2D data = queryFieldStrict("allergies", "PatientUserID", userID);

    if (data.error) {
        displaySystemMessage("No Allergies Found...", 2);
        return;
    }

    struct dataContainer1D allergies = getFieldValues(data, "Allergies");

    if (!strncmp(allergies.data[0], "None", 4)) {
        freeMalloc1D(allergies);
        freeMalloc2D(data);
        displaySystemMessage("No Allergies Found...", 2);
        return;
    } 

    clearTerminal();

    displayOptions("Allergies", allergies.data, allergies.x);

    getString("PRESS ENTER TO ENTER");

    freeMalloc2D(data);
    freeMalloc1D(allergies);

}

void displayPastProcedures(char* userID) {
    struct dataContainer2D data = queryFieldStrict("pastProcedures", "PatientUserID", userID);

    if (data.error) {
        displaySystemMessage("No Procedures Found...", 2);
        return;
    }

    struct dataContainer1D procedures = getFieldValues(data, "PastProcedures");

    if (!strncmp(procedures.data[0], "None", 4)) {
        freeMalloc1D(procedures);
        freeMalloc2D(data);
        displaySystemMessage("No Procedures Found...", 2);
        return;
    } 

    clearTerminal();

    displayOptions("Past Procedures", procedures.data, procedures.x);

    getString("PRESS ENTER TO ENTER");

    freeMalloc2D(data);
    freeMalloc1D(procedures);
}

void displayPrescriptions(char* prescriptionID) {
    struct dataContainer2D prescriptions = queryFieldStrict("prescription", "PrescriptionID", prescriptionID);

    if (prescriptions.error) {
            displaySystemMessage("Cannot Find Prescriptions2", 2);
            return;
    }

    struct dataContainer1D medicineIDs = getFieldValues(prescriptions, "MedicineID");

    if (medicineIDs.error) {
            displaySystemMessage("Cannot Find Prescriptions3", 2);
            freeMalloc2D(prescriptions);
            return;
    }

    struct dataContainer1D quantity = getFieldValues(prescriptions, "Quantity");

    if (quantity.error) {
            displaySystemMessage("Cannot Find Prescriptions4", 2);
            freeMalloc2D(prescriptions);
            freeMalloc1D(medicineIDs);
            return;
    }

    char* header = "Prescriptions";
    char** options = malloc(prescriptions.y * sizeof(char*));
    int noOptions = prescriptions.y;

    char buffer[256];

    for (int i=0; i<noOptions; i++) {
        struct dataContainer1D medicine = queryKey("Inventory", medicineIDs.data[i]);

        if (medicine.error) {
            displaySystemMessage("Cannot Find Prescriptions5", 2);
            freeMalloc2D(prescriptions);
            freeMalloc1D(medicineIDs);
            freeMalloc1D(quantity);
            return;
        }


        sprintf(buffer, "%s x %s\0", medicine.data[1], quantity.data[i]);

        options[i] = strdup(buffer);
    }

    clearTerminal();
    displayOptions(header, options, noOptions);

    getString("PRESS ENTER TO RETURN");

    freeMalloc2D(prescriptions);
    freeMalloc1D(medicineIDs);
    freeMalloc1D(quantity);
}

void prescriptionMenu(char* userID) {
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);
    
    if (appointments.error) {
        displaySystemMessage("No Prescriptions Found...", 2);
        return;
    }

    struct dataContainer1D appointmentIDs = getFieldValues(appointments, "AppointmentID");

    if (appointmentIDs.error) {
        displaySystemMessage("No Prescriptions Found...", 2);
        freeMalloc2D(appointments);
        return;
    }

    
    char* header = "Prescription Menu";
    int noOptions = appointmentIDs.x + 1;
    char** options = malloc(noOptions * sizeof(char*));

    for (int i=0; i<appointmentIDs.x; i++) {
        options[i] = strdup(appointmentIDs.data[i]);
    }

    options[appointmentIDs.x] = "Back";

    clearTerminal();

    int choice = displayMenu(header, options, noOptions);

    if (choice == appointmentIDs.x) {
        freeMalloc1D(appointmentIDs);
        freeMalloc2D(appointments);
        return;
    }

    char* chosenID = appointments.data[choice-1][6]; // PresciptionID 

    displayPrescriptions(chosenID);

    freeMalloc1D(appointmentIDs);
    freeMalloc2D(appointments);
}

void EHRMenu(char* userID) {
    char* header = "Electronic Health Records";
    char* options[] = {"View Allergies", "View Past Procedures", "View Prescriptions", "Back"};
    int noOptions = 4;

    while (1) {
        clearTerminal();
        int result = displayMenu(header, options, noOptions);

        if (result == 1) {
            displayAllergies(userID);
        } else if (result == 2) {
            displayPastProcedures(userID);
        } else if (result == 3) {
            prescriptionMenu(userID);
        } else {
            return;
        }
    }
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

    getString("PRESS ENTER TO RETURN");
    freeMalloc2D(bills);
}

void searchBills(char* userID) {
    char* billID = getString("Enter Bill ID: ");

    struct dataContainer2D bills = queryFieldStrict("Bills", "BillsID", billID);

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
    displaySystemMessage("Incorrect Password", 2);
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
            EHRMenu(username);
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