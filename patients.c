#include "File_exe.h"

struct dataContainer2D getActiveAppointments(char* userID) {
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    struct dataContainer2D returnedValue;

    if (appointments.error) {
        returnedValue.error = 1;
        return returnedValue;
    }

    int count = 0;

    for (int i=0; i<appointments.y; i++) {
        if (!strncmp(appointments.data[i][7], "-", 1)) {
            count++;
        }
    }

    if (count == 0) {
        returnedValue.error = 1;
        return returnedValue;
    }

    returnedValue.data = malloc(count * sizeof(char**));

    int iter = 0;

    for (int i=0; i<appointments.y; i++) {
        if (!strncmp(appointments.data[i][7], "-", 1)) {

            returnedValue.data[iter] = malloc(appointments.x * sizeof(char*));
            
            for (int j=0; j<appointments.x; j++) {
                returnedValue.data[iter][j] = strdup(appointments.data[i][j]);
            }
            iter++;
        }
    }

    returnedValue.y = count;
    returnedValue.x = appointments.x;
    returnedValue.error = 0;

    returnedValue.fields = malloc(returnedValue.x * sizeof(char*));

    for (int i=0; i<appointments.x; i++) {
        returnedValue.fields[i] = strdup(appointments.fields[i]);
    }

    freeMalloc2D(appointments);

    return returnedValue;
}

struct dataContainer2D getAppointmentHistory(char* userID) {
    struct dataContainer2D appointments = queryFieldStrict("Appointments", "PatientUserID", userID);

    struct dataContainer2D returnedValue;

    if (appointments.error) {
        returnedValue.error = 1;
        return returnedValue;
    }

    int count = 0;

    for (int i=0; i<appointments.y; i++) {
        if (!strncmp(appointments.data[i][7], "rapt", 4)) {
            count++;
        }
    }

    if (count == 0) {
        returnedValue.error = 1;
        return returnedValue;
    }

    returnedValue.data = malloc(count * sizeof(char**));


    int iter = 0;

    for (int i=0; i<appointments.y; i++) {
        if (!strncmp(appointments.data[i][7], "rapt", 4)) {

            returnedValue.data[iter] = malloc(appointments.x * sizeof(char*));
            
            for (int j=0; j<appointments.x; j++) {
                returnedValue.data[iter][j] = strdup(appointments.data[i][j]);
            }
            iter++;
        }
    }

    returnedValue.y = count;
    returnedValue.x = appointments.x;
    returnedValue.error = 0;

    returnedValue.fields = malloc(returnedValue.x * sizeof(char*));

    for (int i=0; i<appointments.x; i++) {
        returnedValue.fields[i] = strdup(appointments.fields[i]);
    }

    freeMalloc2D(appointments);

    return returnedValue;
}

void displayAppointments(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = getActiveAppointments(userID);

    if (appointments.error) {
        displaySystemMessage("You Have Not Made Any Appointments", 3);
        return;
    }

    if (!appointments.y) {
        freeMalloc2D(appointments);
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
    struct dataContainer2D appointments = getActiveAppointments(userID);

    if (appointments.error) {
        displaySystemMessage("You Have Not Made Any Appointments", 3);
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

    while (1) {
        clearTerminal();
        int input = displayMenu(header, options, noOptions);

        if (input == noOptions) {
            freeMalloc2D(appointments);
            return;
        }

        char* option = options[input-1];

        struct dataContainer1D chosenAppointment = queryKey("Appointments", option);

        char* doctorID = chosenAppointment.data[1];
        char* date =  chosenAppointment.data[5];

        struct dataContainer2D doctorSchedule = queryFieldStrict("doctorSchedule", "DoctorID", doctorID);

        if (doctorSchedule.error) {
            displaySystemMessage("Doctor Unable to Reschedule", 3);
            freeMalloc2D(appointments);
            freeMalloc1D(chosenAppointment);
            return;
        }

        struct dataContainer2D doctorScheduleOnChosenDate = filterDataContainer(doctorSchedule, "Date", date);

        if (doctorScheduleOnChosenDate.error) {
            displaySystemMessage("Doctor Unable to Reschedule", 3);
            freeMalloc2D(appointments);
            freeMalloc1D(chosenAppointment);
            freeMalloc2D(doctorSchedule);
            return;
        }

        char* timeSlotOptions[4];

        int count = 0;

        struct dataContainer2D doctorAppointments = queryFieldStrict("Appointments", "StaffUserID", doctorID);
        struct dataContainer2D doctorAppointmentsOnChosenDate = filterDataContainer(doctorAppointments, "Date", date);
        struct dataContainer1D takenTimeSlots = getFieldValues(doctorAppointmentsOnChosenDate, "TimeSlots");

        for (int i=1; i<5; i++) {
            int taken = 0;
            char* timeslot = doctorScheduleOnChosenDate.data[0][i];
            
            if (!strncmp(timeslot, chosenAppointment.data[4], 256)) { // If Same as Current TimeSlot or Not Available then Skip
                continue;
            }

            for (int j=0; j<takenTimeSlots.x; j++) {
                if (!strncmp(timeslot, takenTimeSlots.data[j], 256)) {
                    taken = 1;
                    break;
                }
            }

            if (taken) {
                continue;
            }

            timeSlotOptions[count++] = timeslot;
        }

        if (count == 0) {
            displaySystemMessage("Doctor Unable to Reschedule", 3);
            freeMalloc2D(appointments);
            freeMalloc1D(chosenAppointment);
            freeMalloc2D(doctorAppointments);
            freeMalloc2D(doctorAppointmentsOnChosenDate);
            freeMalloc2D(doctorSchedule);
            freeMalloc2D(doctorScheduleOnChosenDate);
            freeMalloc1D(takenTimeSlots);
            return;
        }

        clearTerminal();
        input = displayMenu(secondHeader, timeSlotOptions, count);

        char* newTimeSlot = timeSlotOptions[input-1];

        char* certain = getString("Are you Certain (Y|N)? ");

        if (tolower(certain[0]) == 'y') {
            chosenAppointment.data[4] = strdup(newTimeSlot);

            updateData("Appointments", chosenAppointment.data);

            char confirmMessage[256];
            sprintf(confirmMessage, "Appointment %s Has Successfully been Rescheduled to %s", option, newTimeSlot);
            displaySystemMessage(confirmMessage, 2);
        }

        freeMalloc1D(chosenAppointment);
        freeMalloc2D(doctorAppointments);
        freeMalloc2D(doctorAppointmentsOnChosenDate);
        freeMalloc2D(doctorSchedule);
        freeMalloc2D(doctorScheduleOnChosenDate);
        freeMalloc1D(takenTimeSlots);
    }
    freeMalloc2D(appointments);
}

void cancelAppointmentsMenu(char* userID) {
    // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    while (1) {
        struct dataContainer2D appointments = getActiveAppointments(userID);

        if (appointments.error) {
            displaySystemMessage("You Have Not Made Any Appointments", 3);
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
        freeMalloc2D(data);

        char* certain = getString("Are you Certain (Y|N)? ");


        if (tolower(certain[0]) == 'y') {
            deleteKey("Appointments", option);
            displaySystemMessage("Appointment Successfully Canceled...", 2);

            if (appointments.y == 1) {
                freeMalloc2D(appointments);
                displaySystemMessage("No Appointments Found! Returning to Previous Menu...", 2);
                return;
            }
        }

        freeMalloc2D(appointments);
    }

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

    getString("PRESS ENTER TO RETURN");

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

    getString("PRESS ENTER TO RETURN");

    freeMalloc2D(data);
    freeMalloc1D(procedures);
}

void displayPrescriptions(char* prescriptionID) {
    struct dataContainer2D prescriptions = queryFieldStrict("prescription", "PrescriptionID", prescriptionID);

    if (prescriptions.error) {
            displaySystemMessage("Cannot Find Prescriptions", 2);
            return;
    }

    struct dataContainer1D medicineIDs = getFieldValues(prescriptions, "MedicineID");

    if (medicineIDs.error) {
            displaySystemMessage("Cannot Find Prescriptions", 2);
            freeMalloc2D(prescriptions);
            return;
    }

    struct dataContainer1D quantity = getFieldValues(prescriptions, "Quantity");

    if (quantity.error) {
            displaySystemMessage("Cannot Find Prescriptions", 2);
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
            displaySystemMessage("Cannot Find Prescriptions", 2);
            freeMalloc2D(prescriptions);
            freeMalloc1D(medicineIDs);
            freeMalloc1D(quantity);
            return;
        }

        sprintf(buffer, "%s x %s\0", medicine.data[1], quantity.data[i]);

        options[i] = strdup(buffer);

        freeMalloc1D(medicine);
    }

    clearTerminal();
    displayOptions(header, options, noOptions);

    getString("PRESS ENTER TO RETURN");

    freeMalloc2D(prescriptions);
    freeMalloc1D(medicineIDs);
    freeMalloc1D(quantity);
}

void prescriptionMenu(char* userID) {
    struct dataContainer2D appointments = getAppointmentHistory(userID);
    
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

    while (1) {
        clearTerminal();

        int choice = displayMenu(header, options, noOptions);

        if (choice == noOptions) {
            freeMalloc1D(appointmentIDs);
            freeMalloc2D(appointments);
            return;
        }
        
        char* chosenID = appointments.data[choice-1][6]; // PresciptionID 

        displayPrescriptions(chosenID);    
    }

    freeMalloc1D(appointmentIDs);
    freeMalloc2D(appointments);
}

void displayAppointmentHistory(char* userID) {
     // AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;ReportID;
    struct dataContainer2D appointments = getAppointmentHistory(userID);

    if (appointments.error) {
        displaySystemMessage("You Have Not Made Any Appointments", 3);
        return;
    }

    if (!appointments.y || !strncmp(appointments.data[0][7], "-", 2)) {
        displaySystemMessage("You Have Not Made Any Appointments", 3);
        return;
    }

    clearTerminal();
    displayTabulatedData(appointments);

    getString("PRESS ENTER TO RETURN");
    freeMalloc2D(appointments);
}

void EHRMenu(char* userID) {
    char* header = "Electronic Health Records";
    char* options[] = {"View Allergies", "View Past Procedures", "View Prescriptions", "View Appointment History", "Back"};
    int noOptions = 5;

    while (1) {
        clearTerminal();
        int result = displayMenu(header, options, noOptions);

        if (result == 1) {
            displayAllergies(userID);
        } else if (result == 2) {
            displayPastProcedures(userID);
        } else if (result == 3) {
            prescriptionMenu(userID);
        } else  if (result == 4) {
            displayAppointmentHistory(userID);
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

    if (userData.error) {
        displaySystemMessage("Username not Found", 2);
        return 1;
    }

    if (!userData.y) {
        displaySystemMessage("Username not Found", 2);
        freeMalloc2D(userData);
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

    int error = 0;
    char* username; 
    char* password;

    do {
        clearTerminal();
        username = getString("Enter your Username: ");
        password = getString("Enter your Password: ");

        error = loginPatient(username, password);

        if (!error) {
            break;
        }

        char* input = getString("Try Again (Y|N): ");

        if (tolower(input[0]) == 'n') {
            return 1;
        }
    } while (error);

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
}