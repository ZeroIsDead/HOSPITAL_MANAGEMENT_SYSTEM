#include "File_exe.h"
char* NurseName;

void NurseMenue(char* name);
int main();
void StaffPortal();

///////////////////patient.c///////////////////////
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
        displaySystemMessage("Bills Not Found", 3);
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

//////////////////doctor.c/////////////////////////

void displayTabulatedData1(struct dataContainer2D data)
{   const int minPadding = 5;
    int numRow = data.y + 1;
    // Allocate memory for numRow number of strings
    char** displayedStrings = malloc (numRow * sizeof(char*));

    // Find the maximum length of each column
    int columnLengths[data.x];

    for (int i=0; i<data.x; i++) 
    {
        int columnLength = strlen(data.fields[i]);

        // Iterate over each row and find the maximum length of each column
        for (int j=0; j<data.y; j++) 
        {
            int currentLength = strlen(data.data[j][i]);

            if (currentLength > columnLength) 
            {
                columnLength = currentLength;
            }
        }

        columnLengths[i] = columnLength;
    }

    // Create a buffer to store the formatted string
    int bufferLength = 256;
    char stringBuffer[bufferLength];

    // Clear the string
    stringBuffer[0] = '\0';

    strncat(stringBuffer, "  No. |", 30);

    // Format the Field Strings
    // Iterate over each column and format the string
    for (int i=0; i<data.x; i++) 
    {
        int totalPadding = columnLengths[i] - strlen(data.fields[i]) + minPadding;

        // Calculate the left padding
        int leftPadding = floor(totalPadding/2);
        
        // Add the left padding to the string
        for (int n=0; n<leftPadding; n++) 
        {
            strncat(stringBuffer, " ", 2);
        } 

        // Add the field name to the string
        strncat(stringBuffer, data.fields[i], strlen(data.fields[i]));

        // Calculate the right padding
        int rightPadding = totalPadding - leftPadding;

        // Add the right padding to the string
        for (int n=0; n<rightPadding; n++) {
            strncat(stringBuffer, " ", 2);
        } 

        // Add the delimiter to the string
        strncat(stringBuffer, "|", 2);
    }

    // Add the string to the array
    displayedStrings[0] = strdup(stringBuffer);

    // Format the Strings
    // Iterate over each row and format the string
    for (int i=0; i<data.y; i++) 
    {
        stringBuffer[0] = '\0';

        sprintf(stringBuffer, "  %d.  |", i+1);

        // Create the string
        for (int j=0; j<data.x; j++) {
            int totalPadding = columnLengths[j] - strlen(data.data[i][j]) + minPadding;

            // Left Padding
            int leftPadding = floor(totalPadding/2);
            
            // Add the left padding to the string
            for (int n=0; n<leftPadding; n++) {
                strncat(stringBuffer, " ", 2);
            } 

            // Add the field value to the string
            strncat(stringBuffer, data.data[i][j], strlen(data.data[i][j]));

            // Right Padding
            int rightPadding = totalPadding - leftPadding;

            // Add the right padding to the string
            for (int n=0; n<rightPadding; n++) {
                strncat(stringBuffer, " ", 2);
            } 

            // Add the delimiter to the string
            strncat(stringBuffer, "|", 2);
        }

        // Add the string to the array
        displayedStrings[i+1] = strdup(stringBuffer);
    }

    // Print the table
    int tableLength = strlen(displayedStrings[0]) + 1;

    // Print the top row
    for (int i=0; i<tableLength; i++) {
        printf("-");
    }

    // Print each row
    for (int i=0; i<numRow; i++) {
        printf("\n|%s\n", displayedStrings[i]);

        // Print the bottom row
        for (int i=0; i<tableLength; i++) {
            printf("-");
        }
    }

    // Free the memory
    free(displayedStrings);
}

char* getUserID() 
{
    char* username;
    struct dataContainer2D userData;
    int valid = 0;

    do 
    {
        username = getString("Enter patient`s name: ");
        userData = queryFieldStrict("Patient_IDs", "Name" ,username);

        if (userData.error == 1)  // userData.error will be 1 if the username is not found
        {
            displaySystemMessage("Username not found!", 2);
        }
        else
        {
            valid = 1;
        }

    } while (!valid);

    freeMalloc2D(userData);

    return userData.data[0][0];
}

char* getValidUsername()
{
    char* username;
    struct dataContainer1D userData;
    int valid = 0;

    do 
    {
        clearTerminal();
        username = getString("Enter your username: ");
        userData = queryKey("Staff_IDs", username);

        if (userData.error == 1)  // userData.error will be 1 if the username is not found
        {
            displaySystemMessage("Username not found!", 2);
        }
        else
        {
            valid = 1;
        }

    }while(!valid);

    int valid2 = 0;

    do
    {
        char* user_pw = getString("Enter your password: ");

        if (!strcmp(user_pw, userData.data[1]))
        {
            valid2 = 1;
        }
        else
        {
            displaySystemMessage("Wrong password, please try again!", 2);
        }
    }
    while (!valid2);
    
    displaySystemMessage("Login successful!", 2);

    freeMalloc1D(userData);

    return username;
}

int is_valid_date(const char* date)
{
  int year, month, day;

  // Check length
  if (strlen(date) != 10) {
    return 0;
  }

  // Check format (yyyy-mm-dd)
  if (date[4] != '-' || date[7] != '-') {
    return 0;
  }

  // Extract year, month, day (assuming valid format)
  sscanf(date, "%d-%d-%d", &year, &month, &day);

  // Check year range (adjust as needed)
  if (year < 2024 || year > 2025) {
    printf("We in 2024 only!");
    return 0;
  }

  // Check month validity
  if (month < 1 || month > 12) {
    return 0;
  }

  // Check day validity (basic check, can be improved for leap years)
  if (day < 1 || day > 31) {
    return 0;
  }

  // Further checks for specific month day limits (e.g., February) can be added here

  return 1;
}

char* get_new_time()
{
    //getinput
    int new_start_hour;
    int new_start_minutes;
    int new_end_hour;
    int new_end_minutes;
    
    ///START HOUR///
    int valid = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        sprintf(new_time_format, " [ _ _ : _ _ ]\nEnter New Hour ( 0-23 ): ");
        new_start_hour = getInt(new_time_format);

        if (new_start_hour > 0 && new_start_hour < 24)
        {
            valid = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (24h format only)!", 2);
        }

    }while(!valid);

    ///START MINUTES///
    int valid2 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        sprintf(new_time_format, " [ %02d: _ _ ]\nEnter New Minutes: ", new_start_hour);
        new_start_minutes = getInt(new_time_format);

        if (new_start_minutes >= 0 && new_start_minutes < 60)
        {
            valid2 = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (0-59) only)!", 2);
        }

    }while(!valid2);

    ///END HOUR///
    int valid3 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Slots start : %02d:%02d\n", new_start_hour, new_start_minutes);
        sprintf(new_time_format, " [ _ _ : _ _ ]\nEnter End Hour ( %02d-23 ): ", new_start_hour);
        new_end_hour = getInt(new_time_format);

        if (new_end_hour > 0 && new_end_hour < 24 && new_end_hour > new_start_hour) 
        {
            valid3 = 1;
        }
        else
        {   
            char new_error_message[256];
            sprintf(new_error_message, " Invalid Time input! Your new slots can ends at ( %02d-23 ) only! : ",new_start_hour);
            displaySystemMessage(new_error_message, 2);
        }

    }while(!valid3);

    ///END MINUTES///
    int valid4 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Slots start at: %02d:%02d\n", new_start_hour, new_start_minutes);
        sprintf(new_time_format, " [ %02d: _ _ ]\nEnter New Minutes: ", new_end_hour);
        new_end_minutes = getInt(new_time_format);

        if (new_end_minutes >= 0 && new_end_minutes < 60)
        {
            valid4 = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (0-59) only)!", 2);
        }

    }while(!valid4);

    clearTerminal();
    printf("New slots start at:[ %02d:%02d ]\n", new_start_hour, new_start_minutes);
    printf("New slots ends at: [ %02d:%02d ]\n", new_end_hour, new_end_minutes);

    char new_time[256];
    sprintf(new_time, "%02d:%02d-%02d:%02d", new_start_hour, new_start_minutes, new_end_hour, new_end_minutes);

    char* new_time_ptr = malloc(strlen(new_time) + 1);
    if(new_time_ptr == NULL) 
    {
        displaySystemMessage("Memory allocation failed", 2);
        return NULL;
    }
    strcpy(new_time_ptr, new_time);
    return new_time_ptr;
}

void DoctorEHRMenu() 
{   
    char* userID = getUserID();

    char* header = "Electronic Health Records";
    char* options[] = {"View Allergies", "View Past Procedures", "View Prescriptions", "Back"};
    int noOptions = 4;

    while (1) 
    {
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

/////////////////////////////////////////////////////////////////////////////////

char* gettime(char* d_choices[], int index)
{
    char* token;
    int start_hour, start_minutes, end_hour, end_minutes;

    char time_slot[256];

    sprintf(time_slot, "%s", d_choices[index]);

    token = strtok(time_slot, ":-");

    if (token != NULL) {
        start_hour = atoi(token);
        token = strtok(NULL, ":-");
    }
    if (token != NULL) {
        start_minutes = atoi(token);
        token = strtok(NULL, ":-");
    }
    if (token != NULL) {
        end_hour = atoi(token);
        token = strtok(NULL, ":-");
    }
    if (token != NULL) {
        end_minutes = atoi(token);
    }

    
    int new_start_hour;
    int new_start_minutes;
    int new_end_hour;
    int new_end_minutes;
    
    ///START HOUR///
    int valid = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Current slots start: %02d:%02d\n", start_hour, start_minutes);
        sprintf(new_time_format, " [ _ _ : %02d ]\nNew Hour ( 0-23 ): ", start_minutes);
        new_start_hour = getInt(new_time_format);

        if (new_start_hour > 0 && new_start_hour < 24)
        {
            valid = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (24h format only)!", 2);
        }

    }while(!valid);

    ///START MINUTES///
    int valid2 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Current slots start : %02d:%02d\n", new_start_hour, start_minutes);
        sprintf(new_time_format, " [ %02d: _ _ ]\nNew Minutes: ", new_start_hour);
        new_start_minutes = getInt(new_time_format);

        if (new_start_minutes >= 0 && new_start_minutes < 60)
        {
            valid2 = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (0-59) only)!", 2);
        }

    }while(!valid2);

    ///END HOUR///
    int valid3 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Slots start : %02d:%02d\n", new_start_hour, new_start_minutes);
        printf("Current slots ends : %02d:%02d\n", end_hour, end_minutes);
        sprintf(new_time_format, " [ _ _ : %02d ]\nNew Hour ( %02d-23 ): ", end_minutes, new_start_hour);
        new_end_hour = getInt(new_time_format);

        if (new_end_hour > 0 && new_end_hour < 24 && new_end_hour > new_start_hour) 
        {
            valid3 = 1;
        }
        else
        {   
            char new_error_message[256];
            sprintf(new_error_message, " Invalid Time input! Your new slots can ends at ( %02d-23 ) only! : ",new_start_hour);
            displaySystemMessage(new_error_message, 2);
        }

    }while(!valid3);

    ///END MINUTES///
    int valid4 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Slots start at: %02d:%02d\n", new_start_hour, new_start_minutes);
        printf("Current slots ends : %02d:%02d\n", end_hour, end_minutes);
        sprintf(new_time_format, " [ %02d: _ _ ]\nNew Minutes: ", new_end_hour);
        new_end_minutes = getInt(new_time_format);

        if (new_end_minutes >= 0 && new_end_minutes < 60)
        {
            valid4 = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (0-59) only)!", 2);
        }

    }while(!valid4);

    printf("New slots start at: %02d:%02d\n", new_start_hour, new_start_minutes);
    printf("New slots ends at: %02d:%02d\n", new_end_hour, new_end_minutes);

    char new_time[256];
    sprintf(new_time, "%02d:%02d-%02d:%02d", new_start_hour, new_start_minutes, new_end_hour, new_end_minutes);

    char* new_time_ptr = malloc(strlen(new_time) + 1);
    if(new_time_ptr == NULL) 
    {
        displaySystemMessage("Memory allocation failed", 2);
        return NULL;
    }
    strcpy(new_time_ptr, new_time);
    return new_time_ptr;
    
}

void ammend_slots(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{
    /*Index note
    appointments.data[0][1] = Slots1
    appointments.data[0][2] = Slots2
    appointments.data[0][3] = Slots3
    appointments.data[0][4] = Slots4
    */

    char* d_menu = "Which time slots to amend?";
    char* d_choices[appointments.x];
    int choice_index = 0;
    
    for (int j = 1 ; j < appointments.x - 1; j++)
    {
        d_choices[choice_index] = appointments.data[0][j];
        choice_index++;
    }
    d_choices[choice_index] = "Cancel";

    //print menu
    clearTerminal();
    int d_output = displayMenu(d_menu, d_choices, choice_index+1);

    if (d_output >= 1 && d_output <= 4) 
    {
        int index = d_output - 1;
        char* newtime = gettime(d_choices, index);
        appointments.data[0][index+1] = newtime;
    }
    else 
    {
        return;
    }
    
    char new_time[256];
    sprintf(new_time, "Uploading new time: %s\nPlease wait....", new_time);
    displaySystemMessage(new_time, 2);

    update_non_primary_Data("doctorSchedule", appointments.data[0], search_date, 5);

    displaySystemMessage("Update Successful! ", 2);

    printf("Updated Slots: \n");
    displayTabulatedData1(appointments);
    printf("\n\n");
    getString("PRESS ENTER TO RETURN...");
    

}

void View_My_Reports(char* doctor_username)
{   
    struct dataContainer2D  d_appointments = queryFieldStrict("Appointments", "StaffUserID",doctor_username);
    char* d_reports[d_appointments.y+1];
    
    int i = 0;
    int noOptions = 0;
    while (i < d_appointments.y)
    {
        if (d_appointments.data[i][7][0] == '-') {
            i++;
            continue;
        }
        d_reports[i] = strdup(d_appointments.data[i][7]);
        noOptions++;
        i++;
    }
    d_reports[noOptions] = strdup("Back");
    
    //cooking for menu
    char* header = "My Reports";
    noOptions += 1;

    //print menu
    clearTerminal();
    int result = displayMenu(header, d_reports, noOptions);

    char* key = "r";
    if (result > 0 && result < noOptions)
    {
        key = d_appointments.data[result-1][7];
    }
    else
    {
        freeMalloc2D(d_appointments);
        return;
    }

    //cooking for report printing
    struct dataContainer1D matched_report = queryKey("Reports", key);

    char* CaseName = strdup(matched_report.data[1]);
    char* DiagnosticComments = strdup(matched_report.data[2]);

    char CaseHeader[256];
    char display_CaseName[256];
    char display_DiagnosticComments[1045];

    sprintf(CaseHeader, "Report for appointment %s", matched_report.data[0]);
    sprintf(display_CaseName, "Case Name: %s", CaseName);
    sprintf(display_DiagnosticComments, "Diagnostic Comments: %s", DiagnosticComments);
    char* options[] = {display_CaseName, display_DiagnosticComments};

    clearTerminal();
    displayUnorderedOptions(CaseHeader, options, 2);
    printf("\n");
    getString("PRESS ENTER TO RETURN...");

    freeMalloc2D(d_appointments);
    freeMalloc1D(matched_report);

}

void My_Reports(struct dataContainer1D appointment) 
{
    char* CaseName;
    char* DiagnosticComments;
    
    clearTerminal();
    printf("Report for appointment %s\n\n", appointment.data[0]);
    CaseName = getString("Enter case name: ");
    DiagnosticComments = getString("Enter diagnostic comments: ");

    //cooking for UI
    char CaseHeader[256];
    char display_CaseName[256];
    char display_DiagnosticComments[256];


    sprintf(CaseHeader, "Report for appointment %s", appointment.data[0]);
    sprintf(display_CaseName, "Case Name: %s", CaseName);
    sprintf(display_DiagnosticComments, "Diagnostic Comments: %s", DiagnosticComments);
    char* options[] = {display_CaseName, display_DiagnosticComments};

    //display my_report()
    clearTerminal();
    displayUnorderedOptions(CaseHeader, options, 2);
    char* comfirmation = getString("Are you sure you want to submit this report? (y/n): ");
    
    char reportID[8];
    sprintf(reportID, "r%s", appointment.data[0]);

    char* input[] = {reportID, CaseName, DiagnosticComments}; 

    appointment.data[7] = reportID;    

    if ((strcmp(comfirmation, "y") == 0))
    {
        displaySystemMessage("Submitting Report....", 2);
        write_new_data("Reports", 3, input);
        updateData("Appointments", appointment.data);
        displaySystemMessage("Your Submission is Successful! Returning to Main Menu...", 2);
    }
    else
    {   
        displaySystemMessage("Returning to Main Menu...", 2);
        return;
    }
}

void Write_New_Report()
{
    char* appointmentID;
    struct dataContainer1D appointments;
    int valid = 0;
    
    do 
    {   
        clearTerminal();
        appointmentID = getString("Enter appointment ID for your report: ");
        appointments = queryKey("Appointments", appointmentID);  

        if (appointments.error == 1)
        {
            displaySystemMessage("Appointment ID does not exist ! ", 2);
        }
        else if (appointments.data[7] == "-")
        {
            char appointmentexist[256];
            sprintf(appointmentexist, "Appointment %s already has a report!!", appointmentID);
            displaySystemMessage(appointmentexist, 2);
        }
        else
        {
            valid = 1;
        }

    } while (!valid);

    My_Reports(appointments);
    freeMalloc1D(appointments);
}

void My_reports_menu(char* doctor_username)
{
    clearTerminal();
    char* header = "My Reports";
    char* options[] = {"View My Reports", "Write New Report", "Back"};
    int noOptions = 3;

    int result = displayMenu(header, options, noOptions);

    if (result == 1) 
    {
        View_My_Reports(doctor_username);
    } 
    else if (result == 2)
    {
        Write_New_Report();
    } 
    else if (result == 3) 
    {
        return;
    }
}

void create_new_Schedule(char* doctor_username)
{   
    int valid_date = 0;
    char* new_date;
    do
    {    
        clearTerminal();
        new_date = getString("Enter date for new schedule (yyyy-mm-dd): ");
        
        struct dataContainer2D existing_schedule = queryFieldStrict("doctorSchedule", "Date", new_date);

        if (is_valid_date(new_date)) 
        {
            if (existing_schedule.error == 1)
            {
                valid_date = 1;
            }
            else
            {
                displaySystemMessage("You have already created a schedule for this day!\nGo to <Change My Schedule>", 2);
                clearTerminal();
                printf("\n\n");
                getString("Press any key to return...");

                freeMalloc2D(existing_schedule);

                return;
            }
        }
        else 
        {
            displaySystemMessage("Invalid date!!!", 2);
        }
    }while(!valid_date);

    //do prettier UI afterwards
    char* new_time1 = get_new_time();
    getString("\n\nPress any key to continue...");
    char* new_time2 = get_new_time();
    getString("\n\nPress any key to continue...");
    char* new_time3 = get_new_time();
    getString("\n\nPress any key to continue...");
    char* new_time4 = get_new_time();
    getString("\n\nPress any key to continue...");


    char* inputs[] = {doctor_username, new_time1, new_time2, new_time3, new_time4, new_date};
    write_new_data("doctorSchedule", 6 , inputs);
}

//delete entire day
void delete_entire_day(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{   
    clearTerminal();
    displayTabulatedData(appointments);
    printf("\nDo you sure to remove your entire schdule for this day?( y for yes / Press anykey to return)\n");
    char* input = getString("Your input: ");

    if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0)
    {
        char* username = getString("Enter your username for comfirmation (Press anykey to return): ");

        if (strcmp(username, doctor_username) == 0)
        {    
            delete_non_primary_Key("doctorSchedule", doctor_username, search_date, 5);
        }
        else
        {
            clearTerminal();
            printf("\n\n");
            getString("WRONG USERNAME, PRESS ENTER TO RETURN...");
        }
    
    }
    else
    {
        clearTerminal();
        printf("\n\n");
        getString("PRESS ENTER TO RETURN...");
    }
    
}

//delete a specific slot into NULL
void delete_slots(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{       
    char* d_menu = "Choose The Slot To Delete";
    char* d_choices[appointments.x];
    int choice_index = 0;
    
    for (int j = 1 ; j < appointments.x - 1; j++)
    {
        d_choices[choice_index] = appointments.data[0][j];
        choice_index++;
    }
    d_choices[choice_index] = "Cancel";

    int d_output = displayMenu(d_menu, d_choices, choice_index+1);
    
    int index;
    if (d_output == 1)
     {   
        index = 1;
        appointments.data[0][index] = "NULL";
     }
     else if (d_output == 2)
     {
        index = 2;
        appointments.data[0][index] = "NULL";
     }
     else if (d_output == 3)
     {
        index = 3;
        appointments.data[0][index] = "NULL";
     }
     else if (d_output == 4)
     {
        index = 4;
        appointments.data[0][index] = "NULL";

     }
     else if (d_output == 5)
     {
        return;
     }

    displaySystemMessage("Uploading new time....", 2);

    update_non_primary_Data("doctorSchedule", appointments.data[0], search_date, 5);

    displaySystemMessage("Update Successful! ", 2);

    printf("Updated Slots: \n");
    displayTabulatedData(appointments);
    printf("\n\n");
    getString("PRESS ENTER TO RETURN...");
}

//menu to choose which slot to append
void append_slots_menu(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{
    char* d_menu = "What you want to do?";
    char* d_choices[] = {"Ammend Selected Slots", "Delete Slots","Delete Entire Day", "Return to My Schedule"};
    int noOptions = 4;

    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);
        clearTerminal();

        if (d_output == 1)
        {
            ammend_slots(appointments, doctor_username, search_date);
        }
        else if (d_output == 2)
        {   
            delete_slots(appointments, doctor_username, search_date);
        }
        else if (d_output == 3)
        {
            delete_entire_day(appointments, doctor_username, search_date);
        }
        else if (d_output == 4)
        {
            return;
        }
        
    }

}

void search_case_name() {
    struct dataContainer2D report;

    while (1) {
        char* caseName = getString("Enter Case Name: ");


        report = queryFieldStrict("Reports", "CaseName", caseName);

        if (report.error) {
            clearTerminal();
            displaySystemMessage("Case Not Found!", 2);
            continue;
        } 

        break;
    }

    clearTerminal();
    displayTabulatedData1(report);
    getString("\nPress Enter to Return...");
    freeMalloc2D(report);
}

void EHR_access(char* doctor_username)
{
    char* d_menu = "Electronic Health Records";
    char* d_choices[] = {"Search for patient", "Search via Case Name", "Back"};
    int noOptions = 3;
    
    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if (d_output == 1)
        {
            //patient access
            DoctorEHRMenu();
        }
        else if (d_output == 2)
        {   
            /*Search Case*/
            clearTerminal();
            search_case_name();
        }
        else if (d_output == 3)
        {
            return;
        }
    }
}

void allappointments(char* doctor_username)
{
    /*All Appointments History*/
    struct dataContainer2D d_appointments = queryFieldStrict("Appointments", "StaffUserID",doctor_username);
    
    clearTerminal();
    displayTabulatedData1(d_appointments);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN...");
    
    freeMalloc2D(d_appointments);
}

void search_Appointments(char* doctor_username)
{   
    struct dataContainer2D d_appointments;
    struct dataContainer2D appointments;
    char* search_date;
    int valid = 0;

    do
    {   
        clearTerminal();
        search_date = getString("Please Enter the Appointment date (yyyy-mm-dd): ");
        d_appointments = queryFieldStrict("Appointments", "Date", search_date);

        if (d_appointments.error == 1)
        {
            displaySystemMessage("No appointment for that day!", 2);
        }
        else
        {
            valid = 1;
        }

    }while(!valid);

    //get the specific dr`s all appointment
    appointments = filterDataContainer(d_appointments, "StaffUserID", doctor_username);

    displayTabulatedData(appointments);

    printf("\n");
    getString("PRESS ENTER TO RETURN...");
    
    freeMalloc2D(d_appointments);
    freeMalloc2D(appointments);
}

void create_appointment(char* doctor_username)
{   
    struct dataContainer2D all_schedule, chosen_day;
    
    all_schedule = queryFieldStrict("doctorSchedule", "DoctorID", doctor_username);

    if (all_schedule.error) 
    {
        displaySystemMessage("Error , no records found", 2);
    }

    //get date
    char* search_date;
    int valid = 0;
    do
    {   
        clearTerminal();
        search_date = getString("Enter date for new appointment (yyyy-mm-dd) : ");
        chosen_day = filterDataContainer(all_schedule, "Date", search_date);

        if (chosen_day.error)
        {
            printf("You havent created any schedule for that day.\n");
            printf("Go to <My Availability> and create a new schedule.\n");
            getString("\nRETURNING...");
        }
        else
        {
            valid = 1;
        }

    }while(!valid);


    //Generate new appointmentID
    struct dataContainer2D all_appointments;
    char* previous_appointmentID;
    char new_appointmentID[256];
    int prev_count;

    all_appointments = getData("Appointments");
    if (all_appointments.error == 1)
    {
         displaySystemMessage("Get data error!",2);
    }
    previous_appointmentID = strdup(all_appointments.data[all_appointments.y-1][0]);
    prev_count = atoi(previous_appointmentID+3);
    sprintf(new_appointmentID, "apt%04d", prev_count+1);
    freeMalloc2D(all_appointments);
    
    //Check if appointment already exists
    struct dataContainer2D d_appointment, particular_day;

    d_appointment = queryFieldStrict("Appointments", "StaffUserID", doctor_username);
    particular_day = filterDataContainer(d_appointment, "Date", search_date);
    if (particular_day.error == 0)
    {
        clearTerminal();
        printf("You already have an appointment on that day!\n");
        displayTabulatedData(particular_day);
        char comfirmation = *getString("Confirm to add a new appointment? (y/n): ");
        if (comfirmation == 'y' || comfirmation == 'Y')
        {
            clearTerminal();
        }
        else
        {
            clearTerminal();
            getString("\nRETURNING... PRESS ENTER TO CONTINUE");
            freeMalloc2D(d_appointment);
            freeMalloc2D(particular_day);
            freeMalloc2D(all_schedule);
            freeMalloc2D(chosen_day);
            return;
        }
    }

    //choose slots
    char* slots_menu = "Choose a slot for new appointment";
    char* slots_choices[5];
    int choice_index = 5;

    for (int i = 0; i < 4 ; i++)
    {
        slots_choices[i] = strdup(chosen_day.data[0][i+1]);

    }
    slots_choices[4] = "Back";

    clearTerminal();
    int slot_choice = displayMenu(slots_menu, slots_choices, choice_index);
    
    char* picked_slot;
    if (slot_choice == 1)
    {
        picked_slot = slots_choices[0];
    }
    else if (slot_choice == 2)
    {
        picked_slot = slots_choices[1];
    }
    else if (slot_choice == 3)
    {
        picked_slot = slots_choices[2];
    }
    else if (slot_choice == 4)
    {
        picked_slot = slots_choices[3];
    }
    else if (slot_choice == 5)
    {
        freeMalloc2D(d_appointment);
        freeMalloc2D(particular_day);
        freeMalloc2D(all_schedule);
        freeMalloc2D(chosen_day);
        return;
    }

    //cooking for header
    char display_new_appoinmentID[50], display_drID[50], display_date[50], display_picked_slot[50], display_picked_room[50], display_patient[50];
    sprintf(display_new_appoinmentID,"New Appointment: %s", new_appointmentID);
    sprintf(display_drID, "Doctor ID: %s", doctor_username);
    sprintf(display_date, "Date: %s", search_date);
    sprintf(display_picked_slot, "Slot: %s", picked_slot);
    
    char* appointment_detials1[] = {display_drID, display_date, display_picked_slot};
    int no_appointment_details1 = 3;
    
    //get room
    int valid2 = 0;
    char* picked_room;
    do
    {
        clearTerminal();
        displayUnorderedOptions(display_new_appoinmentID, appointment_detials1, no_appointment_details1);
        picked_room = getString("Enter room: ");
        sprintf(display_picked_room, "Room: %s", picked_room);  

        if (strcmp(picked_room, "101") == 0 || strcmp(picked_room, "102") == 0 || strcmp(picked_room, "103") == 0 || strcmp(picked_room, "201") == 0 || strcmp(picked_room, "202") == 0 || strcmp(picked_room, "203") == 0 || strcmp(picked_room, "301") == 0 || strcmp(picked_room, "302") == 0 || strcmp(picked_room, "303") == 0)
        {
            valid2 = 1;
        }
        else 
        {
            displaySystemMessage("Room does not exist!", 2);
        }   

    }while(!valid2);

    //appointment details
    char* appointment_detials2[] = {display_drID, display_date, display_picked_slot, display_picked_room};
    int no_appointment_details2 = 4;
    
    clearTerminal();
    displayUnorderedOptions(display_new_appoinmentID, appointment_detials2, no_appointment_details2);
    char* patientID = getUserID("Enter Patient Name: ");
    sprintf(display_patient, "Patient: %s", patientID);  

    //Comfirmation

    char* appointment_detials3[] = {display_drID, display_date, display_picked_slot, display_picked_room, display_patient};
    int no_appointment_details3 = 5;

    clearTerminal();
    displayUnorderedOptions(display_new_appoinmentID, appointment_detials3, no_appointment_details3);
    char comfirmation = *getString("Confirm to add a new appointment? (y/n): ");
    
    if (comfirmation == 'y' || comfirmation == 'Y')
    {
        //AppointmentID;StaffUserID;PatientUserID;RoomNo;TimeSlots;Date;PrescriptionID;ReportID;
        char* input[] = {new_appointmentID, doctor_username, patientID, picked_room, picked_slot, search_date,"", ""};
        int input_size = 8;

        write_new_data("Appointments", input_size, input);
        displaySystemMessage("New Appointment Added Successfully!", 2);
        getString("\n\nPRESS ENTER TO CONTINUE");
    }
    else
    {
        displaySystemMessage("Returning to Main Menu...", 2);
        getString("\n\nPRESS ENTER TO CONTINUE  ");
    }

    freeMalloc2D(d_appointment);
    freeMalloc2D(particular_day);
    freeMalloc2D(all_schedule);
    freeMalloc2D(chosen_day);
}

char* Availability(char* doctor_username)
{
    struct dataContainer2D d_appointments;
    struct dataContainer2D appointments;
    
    char* search_date;
    int valid = 0;
    do
    {   
        clearTerminal();
        search_date = getString("Please enter the date you wish to read (yyyy-mm-dd): ");
        d_appointments = queryFieldStrict("doctorSchedule", "Date", search_date);

        if (is_valid_date(search_date)) 
        {
            if (d_appointments.error == 1)
            {
               displaySystemMessage("No schedule for this day!", 2);
            }
            else
            {
                valid = 1;
            }
        }
        else 
        {
            displaySystemMessage("Invalid date!!!", 2);
        }

    }while(!valid);

    //get the specific dr`s all appointment
    appointments = filterDataContainer(d_appointments, "DoctorID", doctor_username);

    displayTabulatedData(appointments);
    printf("Do you want to append your schdule for this day? ( y for yes / Press anykey to return) \n");
    char* append = getString(" Your input: ");

    if (strcmp(append, "y") == 0)
    {
        append_slots_menu(appointments, doctor_username, search_date);
    }
    else
    {
        clearTerminal();
        printf("\n\n");
        getString("PRESS ENTER TO RETURN...");
    }

    freeMalloc2D(d_appointments);
    freeMalloc2D(appointments);
}

void availability_menu(char* doctor_username)
{
    char* d_menu = "My Availability";
    char* d_choices[] = {"Manage Existing Schedule", "New Schedule","Back"};
    int noOptions = 3;

    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if (d_output == 1)
        {
            Availability(doctor_username);
        }
        else if (d_output == 2)
        {   
            create_new_Schedule(doctor_username);
        }
        else if (d_output == 3)
        {
            return;
        }
    }
}

void my_schedule(char* doctor_username) 
{
    char* d_menu = "My Schedule";
    char* d_choices[] = {"All Appointments History", "Search Appointments", "Create Appointment", "Manage Availability", "Back"};
    int noOptions = 5;

    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if (d_output == 1)
        {
            allappointments(doctor_username);
        }
        else if (d_output == 2)
        {   
            search_Appointments(doctor_username);
        }
        else if (d_output == 3)
        {
            create_appointment(doctor_username);
        }
        else if (d_output == 4)
        {
            availability_menu(doctor_username); 
        }
        else if (d_output == 5)
        {
            return;
        }
    }
}

/// //////////////nurse.c//////////////////////////////
void NurseBack(){

    int back = 1;
    back = getInt("\nPlease Enter 0 to go back: ");

    if (back != 0 ){

        displaySystemMessage("Please enter the corrent input!: ",2);
        
        NurseBack();
        return;
    }
    else{
     
        return;
    
    }
}


char* NurseInventoryId(){
    struct dataContainer2D dataN = getData("Inventory");
    char* inventoryID = dataN.data[dataN.y-1][0];
    char number[3];
    
    //Separating the string "021" from "med021"
    for (int i = 0; i < 3; i++){
        number[i] = inventoryID[i+3];
    }
    
    //string to int and adding 1 to it 
    int num = atoi(number) + 1;
    char newNum[3];
    char newId[7];

    //Creating new Inventory ID
    if (num < 99){
        strcpy(newId, "med0");
        strcat(newId, itoa(num, newNum, 10));
    }
    else{
        strcpy(newId, "med");
        strcat(newId, itoa(num, newNum, 10));    
    }

    freeMalloc2D(dataN);
    return strdup(newId);
}

// Displaying Welcome banner to the specific nurse name
char* NurseWelcomeMessage(char name []){ 
    
    int welcomeSize = 8 + strlen(name) + 1;
    char welcome[welcomeSize];
    
    strcpy(welcome, "Welcome ");
    strcat(welcome, name);
    
    welcome[welcomeSize-1] = '\0';

    return strdup(welcome);
    
}

// Nurse Login Function
char* NurseLogin(){
    
    displaySystemMessage("Opening Nurse Log in Portal",2);
    
    //get userName and concrent data
    char* userName = getString("Enter Your UserName: ");
    struct dataContainer1D userNameCheck = queryKey("Staff_IDs", userName);
    
    //Establishing end poin of string
    if (userNameCheck.error == 0){
        userNameCheck.data[3][5] = '\0';
    }

    //validation of user name
    while (userNameCheck.error == 1 || strcmp(userNameCheck.data[3],"Nurse")){
        if (userNameCheck.error == 1)
        {
            displaySystemMessage("Invalid UserName",3);
        }else{
            displaySystemMessage("User is not a Nurse",3);
        }
        userName = getString("Enter Your UserName: ");
        userNameCheck = queryKey("Staff_IDs", userName);
        userNameCheck.data[3][5] = '\0';
    }

    //get password
    char* password = getString("Enter Your Password: ");

    //validation of password
    while(strcmp(userNameCheck.data[1],password)){
        displaySystemMessage("Wrong Password, Please try agian",3);
        password = getString("Entrer Your Password: ");    
    }       

    //
    return strdup(userNameCheck.data[2]);
    
}

void NCurrentDoctorSchedules(){
    
    // Display + input of Doctor Schedule Menu
    char* option[] = {"All Doctor Schedule", "Specific Doctro Schedule", "Back"};
    int output = displayMenu("Doctor Schedule", option ,3);
    clearTerminal();
    
    //Display All Doctor Schedule
    if (output == 1){
        clearTerminal();
        
        struct dataContainer2D allDocSchedule = getData("doctorSchedule");
        displayTabulatedData(allDocSchedule);
        
        printf("\n");
        
        NurseBack();
        clearTerminal();
        NCurrentDoctorSchedules();
        freeMalloc2D(allDocSchedule);
        return;
    }

    // Searching and Displaying Specific Doctor Schedule
    else if (output == 2){
        do{
            char* docName = getString("Please enter the name of the doc: ");
            struct dataContainer2D specificDocSche = queryFieldStrict("doctorSchedule","DoctorID",docName);
            if(specificDocSche.error == 1){
                displaySystemMessage("Wrong input, Please enter a doctor name: ",2);
            }
            else{
                displayTabulatedData(specificDocSche);
                freeMalloc2D(specificDocSche);
                break;
            }
        }
        while(1);
        
        NurseBack();
        clearTerminal();
        NCurrentDoctorSchedules();
        return;
    }

    // Returning to the Nurse Menu
    else{

        displaySystemMessage("Loading Main Nurse Menue....", 2);
        NurseMenue(NurseName);
        return;
    }
}

void NAvailableDoctor(){
    char* doctorName;
    struct dataContainer2D doctorSchedule;
    
    do{
        
        doctorName = getString("Enter the Doctor name: ");

        doctorSchedule = queryFieldStrict("doctorSchedule", "DoctorID", doctorName);
        if (doctorSchedule.error == 1){
            displaySystemMessage("Doctor does not exist in the database, please enter a valid name....",2);
        }
        else{
            break;
        }
    }
    while(1);
    
    struct dataContainer1D doctorWorkingDate = getFieldValues(doctorSchedule, "Date");
    
    char* date;
    struct dataContainer2D doctorTimeDay;
    
    do{
        displayOptions("Doctor Dates",doctorWorkingDate.data,doctorWorkingDate.x);
        date = getString("Please enter the date you are looking for: ");
        doctorTimeDay = filterDataContainer(doctorSchedule, "Date", date);

        if(doctorTimeDay.error == 1){
            displaySystemMessage("Either Doctor is not working on this day or Entered date is invalid...", 2);
        }
        else{
            break;
        }
    }
    while(1);

    struct  dataContainer2D bookedAppointements = queryFieldStrict("Appointments", "StaffUserID", doctorName);
    bookedAppointements = filterDataContainer(bookedAppointements, "Date", date);

    if(bookedAppointements.error == 1){

        displayTabulatedData(doctorTimeDay);
        freeMalloc2D(bookedAppointements);
        freeMalloc2D(doctorSchedule);
        freeMalloc1D(doctorWorkingDate);
        freeMalloc2D(bookedAppointements);
        NurseBack();
        NurseMenue(NurseName);
        return;
    }
    else{
        int count = 0;
        
        for (int i = 0; i < bookedAppointements.y; i++){
            for (int j = 1; j < 5; j++){
                if ( strcmp(bookedAppointements.data[i][4],doctorTimeDay.data[0][j]) == 0){
                    doctorTimeDay.data[0][j] = "0";
                    count++;
                }
            }
        }
        freeMalloc2D(bookedAppointements);
        freeMalloc2D(doctorSchedule);
        freeMalloc1D(doctorWorkingDate);
        
        printf("\n\n");
        
        if (count == 4){
            displaySystemMessage("All time slots books, Please pick another date....",5);
            freeMalloc2D(bookedAppointements);
            main();
            
        }
        else{
            clearTerminal();
            printf("Available Time on %s for Doctor %s\n", date, doctorName);
            displayTabulatedData(doctorTimeDay);
            NurseBack();
            NurseMenue(NurseName);
        }
    }
    
}

//View Current inventory
void NViewStationInventory(){
    clearTerminal();
    struct dataContainer2D currentInventory = getData("Inventory");
    displayTabulatedData(currentInventory);
    NurseBack();
    clearTerminal();
    freeMalloc2D(currentInventory);
    NurseMenue(NurseName);
}

void NurseAddNewIteam(){
    
    //Get New Medicine Name 
    char* newMed;
    
    do{
        newMed = getString("Enter the medicine name: ");
        if (!strcmp(newMed,"")){
            displaySystemMessage("No Medicine Name was entered",2);
        }
        else{
            break;
        }
    }
    while(1);
    //Check if newMed already exists in Inventory.txt
    struct dataContainer2D checkMed = queryFieldStrict("Inventory","Medicine Name", newMed);
    if (checkMed.error == 0){
        displaySystemMessage("Medicine already Exists, Please enter new Medicine", 2);
        freeMalloc2D(checkMed);
        NurseAddNewIteam();
        return;
    }

    //If newMed is new
    else{

        //Create new Unique ID
        char* newID = NurseInventoryId();
        
        //Get Price and check if entered value is a float
        float price;
        do {
            price = getFloat("Enter Price: ");
            if (price == -1.0){
                displaySystemMessage("Entered Value is not a Float",2);
            }
            else if(price == '\0'){
                displaySystemMessage("No Price was entered....",2);
            }
            else{
                break;
            }
        }
        while(1);

        //Convert float to string
        char strPrice[10];
        snprintf(strPrice, sizeof(strPrice), "%.6g", price);

        //Get Specification of the medicine
        char* specification;
        do{
            specification = getString("Enter Specification: ");
            if (!strcmp(specification,"")){
                displaySystemMessage("No Specificati2on was added",2);
            }
            else{
                break;
            }
        }
        while(1);

        //Get quanity and check if entered value is an integer
        int quantity;
        do{
            quantity = getInt("Enter Quantity: ");
            if(quantity == -1){
                displaySystemMessage("Enterned value is not an integer please try again",2);
            }
            else if (quantity == '\0'){
                displaySystemMessage("No Quantity was entered",2);
            }
            else{
                break;
            }
        }
        while(1);

        //Convert int to string
        char strQuantity[5];
        itoa(quantity, strQuantity, 10);
        
        //Create array of all entered elements in string form
        char* newIteam [] = {newID, newMed, strPrice, specification, strQuantity};

        //update inventory.txt
        int write = write_new_data("Inventory", 5, newIteam);
        
        //Check if update successful or not 
        if (write == 0){
            displaySystemMessage("New Iteams Updated Successfully",2);
        }
        else{
            displaySystemMessage("ERROR!!!",2);
        }
    }

}

//Function to Update Existing
void NUpdateExistingInventory(){
    
    //Getting medId from user
    char* medId = getString("Enter the medicineID you want to update: ");
    struct dataContainer1D iteam = queryKey("Inventory",medId);
    
    //Error handling is iteam medId is not found;
    if (iteam.error == 1){
        displaySystemMessage("Iteam not found",2);
        freeMalloc1D(iteam);
        NUpdateExistingInventory();
        return;
    }
    else{
        
        //Getting new quantity from user
        int newQuantity;
        do{
            newQuantity = getInt("Enter the new quantity of iteams: ");

            if(newQuantity == '\0'){
                displaySystemMessage("No quantity was entered",2);
            }
            else{
                break;
            }
        }
        while(1);
        
        //Converting string to int
        char strNewQutity [5];
        itoa(newQuantity, strNewQutity, 10);
        
        //Updateing iteam with new Quantity
        iteam.data[4] = strNewQutity;
        updateData("Inventory", iteam.data);
        
        freeMalloc1D(iteam);
        
        displaySystemMessage("Updated! Returning you to main menue",2);
        return;
    }
}

void NUpdateStationInventory(){
    char* InventoryBanner = "Nurse Inventory Management";
    char* options[] = {"Enter New Iteam to Inventory", "Update a Current Inventory","Back"};
    int output = displayMenu(InventoryBanner,options,3);

    //Enter New iteam
    if (output == 1){
        NurseAddNewIteam();
        clearTerminal();
        NurseMenue(NurseName);
        return;
    }

    // Update existing iteam
    else if (output == 2){
        NUpdateExistingInventory();
        clearTerminal();
        NurseMenue(NurseName);
        return;
    }

    //Back to Nurse Menue 
    else{
        clearTerminal();
        NurseMenue(NurseName);
        return;
    }
}

void NViewPatientReport(){
    
    char* appointmentNumber = getString("Enter Appointment Number: ");
    struct dataContainer1D appointmentData = queryKey("Appointments",appointmentNumber);
    
    
    while (appointmentData.error == 1){
        displaySystemMessage("Appointment Does not exist, Please try again.....",3);
        appointmentNumber = getString ("Enter Appointment Number: ");
        appointmentData = queryKey("Appointments",appointmentNumber);
    }
   
    struct dataContainer1D patientName = queryKey("Patient_IDs", appointmentData.data[2]);
    struct dataContainer2D patientReport = queryFieldStrict("Reports","ReportID",appointmentData.data[7]);
    clearTerminal();

    printf("Name of Patient:    %s\n",patientName.data[2]);
    printf("Age:                %s\n", patientName.data[3]);
    printf("Insurance Company:  %s\n\n",patientName.data[6]);

    displayTabulatedData(patientReport);
    
    freeMalloc1D(appointmentData);
    freeMalloc1D(patientName);
    freeMalloc2D(patientReport);
    
    NurseBack();
    clearTerminal();
    NurseMenue(NurseName);
    return;
}

int NPatientNumber(struct dataContainer2D array, int num1, int num2){
    
    int sum = 0; 
    for (int i = 0; i < array.y; i++){
        if (atoi(array.data[i][3]) >= num1 && atoi(array.data[i][3]) < num2){
            sum++;
        }
    }
    return sum;

}

void NViewUnitReport(){
    
    char* unitHeader = "Unit Report";
    char* unitOptions[] = {"Number of Patient in Ward","Number of Patient in ICU","Number of Patient in Emergency Room", "Back"};

    int output2 = displayMenu(unitHeader, unitOptions, 4);
    clearTerminal();

    struct dataContainer2D data = getData("Appointments");
    int patientNumber;
    if(output2 == 1){
        patientNumber = NPatientNumber(data, 100, 200);
        printf("There are %d Patient Admitted in Ward\n", patientNumber);
    }
    else if(output2 == 2){
        patientNumber = NPatientNumber(data, 200, 300);
        printf("There are %d Patient Admitted in ICU\n", patientNumber);
    }
    else if (output2 == 3){
        patientNumber = NPatientNumber(data, 300, 400);
        printf("There are %d Patient Admitted in Emergency Room\n", patientNumber);
    }
    else{
        freeMalloc2D(data);
        NurseMenue(NurseName);
        return;
    }
    freeMalloc2D(data);
    NurseBack();
    clearTerminal();
    NurseMenue(NurseName);
    return;

}

void NurseMenue(char* name){
    clearTerminal();
    char* header = NurseWelcomeMessage(name);
    char* options[] = {"Current Doctor Schedules","Available Doctor","View Station Inventory","Update Station Inventory","View Patient Report","View Unit Report","Log Out"};

    int output = displayMenu(header, options, 7);  
    clearTerminal();
    
    if (output == 1){
        NCurrentDoctorSchedules();
        return;
    }else if (output == 2){
        NAvailableDoctor();
    }else if(output == 3){
        NViewStationInventory();
    }
    else if(output == 4){
        NUpdateStationInventory();
    }
    else if (output == 5){
        NViewPatientReport();
    }
    else if (output == 6){
       NViewUnitReport();
    }
    else{
        StaffPortal();
        return;
    }
}

////////////////////admin.c/////////////////////////

void AdminMenu();
void AdminRegistermenu();
///////// Utility //////////
int compareDates(const char *date1, const char *date2) {
    // Variables to hold the parsed date components
    int year1, month1, day1;
    int year2, month2, day2;
    
    // Parse the date1 string
    sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
    
    // Parse the date2 string
    sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);
    
    // Compare the years
    if (year1 != year2) {
        return year1 - year2;
    }
    
    // Compare the months if years are equal
    if (month1 != month2) {
        return month1 - month2;
    }
    
    // Compare the days if years and months are equal
    return day1 - day2;
}

int* Appointment_trend(struct dataContainer2D data)
{   
    char* date;
    char* date_int[3];
    int count=0;
    int d, m, y;
    int* amount_per_day = malloc(7*sizeof(int));

    for (int i=0; i<7;i++)
    {
        amount_per_day[i]=0;
    }

    int i = 0;
    while (i < data.y)
    {   
        count=0;
        date = strdup(data.data[i][5]);

        char *token = strtok(date, "-");
        while (token != NULL)
        {   
            date_int[count++] = token;
            token = strtok(NULL, "-");
        }

        d = atoi(date_int[2]);
        m = atoi(date_int[1]);
        y = atoi(date_int[0]);

        int weekday  = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;

        amount_per_day[weekday] = amount_per_day[weekday] + 1;
    
        i=i+1;
    }   
    return amount_per_day;
}

struct dataContainer2D filteringData(struct dataContainer2D data) 
{
    char* currentDate = "2024-06-03";
    int count = 0;

    struct dataContainer2D filteredData;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) >= 0) 
        {
            count++;
        }
    }

    char*** relaying_array = malloc(count*sizeof(char**));
    int count2  = 0;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) >= 0) 
        {
            relaying_array[count2] = data.data[i];
            count2++;
        }
    }

    filteredData.data = relaying_array;
    filteredData.y = count2;
    filteredData.x = data.x;
    filteredData.error = 0;
    filteredData.fields = data.fields;

    return filteredData;
} 

struct dataContainer2D filteringData2(struct dataContainer2D data) 
{
    char* currentDate = "2024-06-03";
    int count = 0;

    struct dataContainer2D filteredData;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) < 0) 
        {
            count++;
        }
    }

    char*** relaying_array = malloc(count*sizeof(char**));
    int count2  = 0;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) < 0) 
        {
            relaying_array[count2] = data.data[i];
            count2++;
        }
    }

    filteredData.data = relaying_array;
    filteredData.y = count2;
    filteredData.x = data.x;
    filteredData.error = 0;
    filteredData.fields = data.fields;
    
    return filteredData;
}

char* itoa(int num, char* buffer, int base)   
{  
    int current = 0;  
    if (num == 0) {  
    buffer[current++] = '0';  
    buffer[current] = '\0';  
    return buffer;  
    }  
    int num_digits = 0;  
    if (num < 0) {  
    if (base == 10) {  
    num_digits ++;  
    buffer[current] = '-';  
    current ++;  
    num *= -1;  
    }  
    else  
    return NULL;  
    }  
    num_digits += (int)floor(log(num) / log(base)) + 1;  
    while (current < num_digits)   
    {  
    int base_val = (int) pow(base, num_digits-1-current);  
    int num_val = num / base_val;  
    char value = num_val + '0';  
    buffer[current] = value;  
    current ++;  
    num -= base_val * num_val;  
    }  
    buffer[current] = '\0';  
    return buffer;  
}   

char* int_to_string(int number)
{   
    char buffer[256]; 
    char* string_return; 

    itoa(number,buffer,10);
    string_return = buffer;

    return string_return;
}
///////// Admin Login //////////
int AdminLogin(){
    
    displaySystemMessage("Opening Admin Log in Portal",2);
    
    //get userName and concrent data
    char* userName = getString("Enter Your User ID: ");
    struct dataContainer1D userNameCheck = queryKey("Staff_IDs", userName);
    
    //Establishing end poin of string
    if (userNameCheck.error == 0){
        userNameCheck.data[3][5] = '\0';
    }

    //validation of user name
    while (userNameCheck.error == 1 || strcmp(userNameCheck.data[3],"Admin")){
        if (userNameCheck.error == 1)
        {
            displaySystemMessage("Invalid User ID",3);
        }else{
            displaySystemMessage("User is not an Admin",3);
        }
        userName = getString("Enter Your UserName: ");
        userNameCheck = queryKey("Staff_IDs", userName);
        userNameCheck.data[3][5] = '\0';
    }

    //get password
    char* password = getString("Enter Your Password: ");

    //validation of password
    while(strcmp(userNameCheck.data[1],password)){
        displaySystemMessage("Wrong Password, Please try agian",3);
        password = getString("Enter Your Password: ");    
    }       

    return 1;
}
///////// Patient Menu //////////
void PatientRegistration();

void displaycurrentpatient() 
{
    struct dataContainer2D d_patient = getData("Patient_IDs");
        
    displayTabulatedData(d_patient);
    freeMalloc2D(d_patient);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if( strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }    
}

void RegisterNewPatient()
{   
    clearTerminal();
    char* PatientUserID = getString("Enter Patient User ID: ");

    struct dataContainer2D d_patient = queryFieldStrict("Patient_IDs","PatientUserID",PatientUserID);
    if(d_patient.error==0)
    {   
        clearTerminal();
        displaySystemMessage("Patient User ID is already existed!",2);
        displaySystemMessage("Please enter a different one",2);
        freeMalloc2D(d_patient);
        RegisterNewPatient();
    }
    
    freeMalloc2D(d_patient);

    char* UserPW = getString("Enter Patient User Password: ");
    char* Name = getString("Enter Patient Name: ");
    char* Age = getString("Enter Patient Age: ");
    char* Ward = getString("Enter Patient Ward: ");
    char* InsuranceCompany = getString("Enter Patient Insurance Company: ");
    char* InsuranceID = getString("Enter Patient Insurance ID: ");

    char** PatientRinputs = malloc (7 * sizeof(char*));

    PatientRinputs[0]=PatientUserID;
    PatientRinputs[1]=UserPW;
    PatientRinputs[2]=Name;
    PatientRinputs[3]=Age;
    PatientRinputs[4]=Ward;
    PatientRinputs[5]=InsuranceCompany;
    PatientRinputs[6]=InsuranceID;

    char* ConfirmationMessage = getString("Are you sure you want to Register this Patient? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        write_new_data("Patient_IDs", 7, PatientRinputs);
        clearTerminal();
        displaySystemMessage("Registration Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
}

void DeletePatient()
{   
    clearTerminal();

    struct dataContainer2D d_patient = getData("Patient_IDs");  
    displayTabulatedData(d_patient);
    freeMalloc2D(d_patient);

    char* PatientUserID = getString("Enter Patient User ID: ");

    struct dataContainer2D d_Patient = queryFieldStrict("Patient_IDs", "PatientUserID",PatientUserID);

    if(d_Patient.error == 1)
    {   
        clearTerminal();
        displaySystemMessage("Patient does not exist!",2);
        freeMalloc2D(d_Patient);
        DeletePatient();
    }
    else
    {   
        clearTerminal();
        displayTabulatedData(d_Patient);
        freeMalloc2D(d_Patient);
    }

    char* ConfirmationMessage = getString("Are you sure you want to delete this Patient? (Y/N): ");
    
    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        deleteKey("Patient_IDs", PatientUserID);
        clearTerminal();
        displaySystemMessage("Deletion Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to the menu...", 2);
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
}

void PatientRegistration()
{
    char* patientheader = "Patient Registration Menu";
    char* patientoptions[] = {"View Current Patient","Register New Patient","Delete Patient","Return To Menu"};

    clearTerminal();
    int patientoutput = displayMenu(patientheader, patientoptions, 4);

    if(patientoutput==1)
    {
        clearTerminal();
        displaycurrentpatient();
    }
    else if(patientoutput==2)
    {
        clearTerminal();
        RegisterNewPatient();
    }
    else if(patientoutput==3)
    {
        clearTerminal();
        DeletePatient();
    }
    else if(patientoutput==4)
    {
        clearTerminal();
        AdminRegistermenu();
        return;
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return;
    }
}
///////// Doctor Menu //////////
void DoctorRegistration();

void displaycurrentdoctor() 
{
    struct dataContainer2D d_doctor = queryFieldStrict("Staff_IDs","Tags","doctor");
        
    displayTabulatedData(d_doctor);
    freeMalloc2D(d_doctor);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        DoctorRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
}

void RegisterNewDoctor()
{   
    clearTerminal();
    char* StaffUserID = getString("Enter Doctor User ID: ");

    struct dataContainer2D d_Doctor = queryFieldStrict("Staff_IDs","StaffUserID",StaffUserID);
    if(d_Doctor.error==0)
    {   
        clearTerminal();
        displaySystemMessage("Doctor User ID is already existed!",2);
        displaySystemMessage("Please enter a different one",2);
        freeMalloc2D(d_Doctor);
        RegisterNewDoctor();
    }

    freeMalloc2D(d_Doctor);

    char* UserPW = getString("Enter Doctor User Password: ");
    char* Name = getString("Enter Doctor Name: ");

    char** DoctorRinputs = malloc (4 * sizeof(char*));

    DoctorRinputs[0]=StaffUserID;
    DoctorRinputs[1]=UserPW;
    DoctorRinputs[2]=Name;
    DoctorRinputs[3]="Doctor";

    char* ConfirmationMessage = getString("Are you sure you want to Register this Doctor? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        write_new_data("Staff_IDs", 4, DoctorRinputs);
        clearTerminal();
        displaySystemMessage("Registration Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
}

void DeleteDoctor()
{
    clearTerminal();

    struct dataContainer2D d_doctor = queryFieldStrict("Staff_IDs","Tags","doctor");
        
    displayTabulatedData(d_doctor);
    freeMalloc2D(d_doctor);

    char* StaffUserID = getString("Enter Doctor User ID: ");

    struct dataContainer2D d_Doctor = queryFieldStrict("Staff_IDs","StaffUserID",StaffUserID);
    if(d_Doctor.error==1)
    {   
        clearTerminal();
        displaySystemMessage("Doctor User ID does not exist!",2);
        freeMalloc2D(d_Doctor);
        DeleteDoctor();
    }
    else
    {   
        clearTerminal();
        displayTabulatedData(d_Doctor);
        freeMalloc2D(d_Doctor);
    }

    char* ConfirmationMessage = getString("Are you sure you want to delete this Doctor? (Y/N): ");
    
    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        deleteKey("Staff_IDs", StaffUserID);
        clearTerminal();
        displaySystemMessage("Deletion Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to the menu...", 2);
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
}

void DoctorRegistration()
{
    char* Doctorheader = "Doctor Registration Menu";
    char* DoctorOptions[] = {"View Current Doctor","Register New Doctor","Delete Doctor","Return To Menu"};

    clearTerminal();
    int Doctoroutput = displayMenu(Doctorheader, DoctorOptions, 4);

    if(Doctoroutput==1)
    {
        clearTerminal();
        displaycurrentdoctor();
    }
    else if(Doctoroutput==2)
    {
        clearTerminal();
        RegisterNewDoctor();
    }
    else if(Doctoroutput==3)
    {
        clearTerminal();
        DeleteDoctor();
    }
    else if(Doctoroutput==4)
    {
        clearTerminal();
        AdminRegistermenu();
        return;
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return; 
    }
}
///////// Nurse Menu //////////
void NurseRegistration();

void displaycurrentnurse() 
{
    struct dataContainer2D d_nurse = queryFieldStrict("Staff_IDs","Tags","Nurse");
        
    displayTabulatedData(d_nurse);
    freeMalloc2D(d_nurse);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaycurrentnurse();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
}

void RegisterNewNurse()
{
    clearTerminal();
    char* StaffUserID = getString("Enter Nurse User ID: ");

    struct dataContainer2D d_Nurse = queryFieldStrict("Staff_IDs","StaffUserID",StaffUserID);
    if(d_Nurse.error==0)
    {   
        clearTerminal();
        displaySystemMessage("Nurse User ID is already existed!",2);
        displaySystemMessage("Please enter a different one",2);
        freeMalloc2D(d_Nurse);
        RegisterNewDoctor();
    }
    
    freeMalloc2D(d_Nurse);

    char* UserPW = getString("Enter Nurse User Password: ");
    char* Name = getString("Enter Nurse Name: ");

    char** NurseRinputs = malloc (4 * sizeof(char*));

    NurseRinputs[0]=StaffUserID;
    NurseRinputs[1]=UserPW;
    NurseRinputs[2]=Name;
    NurseRinputs[3]="Nurse";;

    char* ConfirmationMessage = getString("Are you sure you want to Register this Nurse? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        write_new_data("Staff_IDs", 7, NurseRinputs);
        clearTerminal();
        displaySystemMessage("Registration Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
}

void DeleteNurse()
{
    clearTerminal();

    struct dataContainer2D d_Nurse = queryFieldStrict("Staff_IDs","Tags","Nurse");
        
    displayTabulatedData(d_Nurse);
    freeMalloc2D(d_Nurse);

    char* StaffUserID = getString("Enter Nurse User ID: ");

    struct dataContainer2D d_nurse = queryFieldStrict("Patient_IDs", "StaffUserID",StaffUserID);

    if(d_nurse.error==1)
    {   
        clearTerminal();
        displaySystemMessage("Nurse User ID does not exist!",2);
        freeMalloc2D(d_nurse);
        DeleteNurse();
    }
    else
    {   
        clearTerminal();
        displayTabulatedData(d_nurse);
        freeMalloc2D(d_nurse);
    }    

    displayTabulatedData(d_nurse);
    freeMalloc2D(d_nurse);

    char* ConfirmationMessage = getString("Are you sure you want to delete this Nurse? (Y/N): ");
    
    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        deleteKey("Staff_IDs", StaffUserID);
        clearTerminal();
        displaySystemMessage("Deletion Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to the menu...", 2);
        NurseRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
}

void NurseRegistration()
{
    char* Nurseheader = "Nurse Registration Menu";
    char* NurseOptions[] = {"View Current Nurse","Register New Nurse","Delete Nurse","Return to Menu"};

    clearTerminal();
    int Nurseoutput = displayMenu(Nurseheader, NurseOptions, 4);

    if(Nurseoutput==1)
    {
        clearTerminal();
        displaycurrentnurse();
    }
    else if(Nurseoutput==2)
    {
        clearTerminal();
        RegisterNewNurse();
    }
    else if(Nurseoutput==3)
    {
        clearTerminal();
        DeleteNurse();
    }
    else if(Nurseoutput==4)
    {
        clearTerminal();
        AdminRegistermenu();
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return; 
    }
}
///////// Appointment Menu //////////
void AdminAppointmentMenu();

void ActiveAppointment()
{   
    struct dataContainer2D d_Appointment = getData("Appointments");
    struct dataContainer2D filteredData = filteringData(d_Appointment);


    displayTabulatedData(filteredData);
    freeMalloc2D(filteredData);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
}

void PastAppointment()
{   
    struct dataContainer2D d_Appointment = getData("Appointments");
    struct dataContainer2D filteredData = filteringData2(d_Appointment);

        
    displayTabulatedData(filteredData);
    freeMalloc2D(filteredData);

        char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
}

void AppointmentTrend()
{   
    struct dataContainer2D d_Appointment = getData("Appointments");

    int* amount_per_day = Appointment_trend(d_Appointment);

    char* Trend_header = "Appointment Trend";
        
    struct dataContainer2D data;

    data.error = 0;
    data.y = 7;
    data.x = 2;

    char* days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    data.data = malloc (data.y * sizeof(char**));

    for (int i=0; i<7; i++) {
    data.data[i] = malloc(data.x * sizeof(char*));
    data.data[i][0] = days[i];
    data.data[i][1] = strdup(int_to_string(amount_per_day[i]));
    }

    data.fields = malloc(data.x * sizeof(char*));
    data.fields[0] = "Day";
    data.fields[1] = "Data";
    
    clearTerminal();
    displayTabulatedData(data);

    freeMalloc2D(data);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    
}

void AdminAppointmentMenu() 
{
    char* Appointmentheader = "Appointment Menu";
    char* Appointmentoptions[] = {"View Active Appointment List", "View Past Appointment List", "View Appointment Trend","Return to Menu"};
        
        clearTerminal();
        int Appointmentoutput = displayMenu(Appointmentheader, Appointmentoptions, 4);

    if(Appointmentoutput==1)
    {
        clearTerminal();
        ActiveAppointment();
    }
    else if(Appointmentoutput==2)
    {
        clearTerminal();
        PastAppointment();
    }
    else if(Appointmentoutput==3)
    {
        clearTerminal();
        AppointmentTrend();
    }
    else if(Appointmentoutput==4)
    {
        clearTerminal();
        AdminMenu();
        return;
    }
     else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return; 
    }      
}
///////// Inventory Menu //////////
void AdminInventoryMenu();

void displayInventory()
{
    struct dataContainer2D d_inventory = getData("Inventory");

    clearTerminal();    
    displayTabulatedData(d_inventory);
    freeMalloc2D(d_inventory);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminInventoryMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
}

void SearchInventory()
{   
    clearTerminal();
    char* MedID = getString("Enter Medicine ID: ");

    struct dataContainer2D d_Inventory = queryFieldStrict("Inventory", "MedicineID", MedID);

    clearTerminal();
    displayTabulatedData(d_Inventory);
    freeMalloc2D(d_Inventory);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminInventoryMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    } 
}

void RestockInventory()
{   
    clearTerminal();
    struct dataContainer2D d_inventory = getData("Inventory");

    clearTerminal();    
    displayTabulatedData(d_inventory);
    freeMalloc2D(d_inventory);

    char* MedID = getString("Enter Medicine ID to be restock: ");

    struct dataContainer2D d_Inventory = queryFieldStrict("Inventory", "MedicineID", MedID);
    
    clearTerminal();
    displayTabulatedData(d_Inventory);

    char* ConfirmationMessage = getString("Are you sure you want to restock this Medicine? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {   
        clearTerminal();
        int RestockValue = getInt("Enter number of restock: ");
        d_Inventory.data[0][4] = int_to_string(atoi(d_Inventory.data[0][4]) + RestockValue);
        updateData("Inventory", d_Inventory.data[0]);
        freeMalloc2D(d_Inventory);
        displaySystemMessage("Restock Complete !", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        freeMalloc2D(d_Inventory);
        RestockInventory();
        return;
    }
    else
    {   
        clearTerminal();
        freeMalloc2D(d_Inventory);
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
}

void AdminInventoryMenu()
{
    char* Inventoryheader = "Inventory Menu";
    char* Inventoryoptions[] = {"View Inventory", "Search Inventory", "Restock Inventory","Return to Menu"};
        
    clearTerminal();
    int inventoryoutput = displayMenu(Inventoryheader, Inventoryoptions, 4);

    if(inventoryoutput==1)
    {
        clearTerminal();
        displayInventory();
        return;
    }
    else if(inventoryoutput==2)
    {
        clearTerminal();
        SearchInventory();
        return;
    }
    else if(inventoryoutput==3)
    {
        clearTerminal();
        RestockInventory();
        return;
    }
    else if(inventoryoutput==4)
    {
        clearTerminal();
        AdminMenu();
        return;
    }
     else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return; 
    }      
}
///////// Registration Menu //////////
void AdminRegistermenu()
{
    char* registrationheader = "Registration Menu";
        char* registrationoptions[] = {"Patient Registration","Doctor Registration","Nurse Registration","Return to Menu"};

        clearTerminal();
        int RegistrationOutput = displayMenu(registrationheader, registrationoptions, 4);

    if(RegistrationOutput==1)
    {
        clearTerminal();
        PatientRegistration();
    }
    else if(RegistrationOutput==2)
    {
        clearTerminal();
        DoctorRegistration();
    }
    else if(RegistrationOutput==3)
    {
        clearTerminal();
        NurseRegistration();
    }
    else if(RegistrationOutput==4)
    {
        clearTerminal();
        AdminMenu();
        return;
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return; 
    }
}
///////// Admin Menu //////////
void AdminMenu()
{
    char* header = "Welcome to the Hospital Admin Management";
    char* option[] = {"Registration", "Appointment", "Inventory","Log out"};
    int output;
    
    clearTerminal();
    output = displayMenu(header, option, 4);

    if (output==1)
    {
        clearTerminal();
        AdminRegistermenu();
        return;
    }

    else if (output==2)
    {
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }

    else if (output==3)
    {
        clearTerminal();
        AdminInventoryMenu();
        return;
    }
    else if (output==4)
    {
        clearTerminal();
        displaySystemMessage("Logging out", 1);
        displaySystemMessage("Logging out.", 1);
        displaySystemMessage("Logging out..", 1);
        displaySystemMessage("Logging out...", 1);
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminMenu();
        return; 
    }
}

/////////////////////main.c////////////////////////

int adminMain() 
{
    int check = AdminLogin();

    if(check==1)
    {
        AdminMenu();
    }
    else
    {
        displaySystemMessage("No access for you", 2);
    }
}

int doctor() 
{   
    char* doctor_username = getValidUsername();

    //cooking infomation for menu
    struct dataContainer1D userData = queryKey("Staff_IDs", doctor_username);

    char d_menu[strlen(userData.data[2])+10];
    sprintf(d_menu, "Hi, %s", userData.data[2]);
    char* d_choices[] = {"My Schedule", "EHR access", "My Reports", "Logout"};
    int noOptions = 4;

    freeMalloc1D(userData);
        
/////////////////////////MENU/////////////////////////////////
    while (1) 
    {  
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if(d_output ==1 )
        {
            my_schedule(doctor_username);
        }
        else if (d_output == 2)
        {
            EHR_access(doctor_username);
        }
        else if (d_output == 3)
        {
            My_reports_menu(doctor_username);
        }
        else if (d_output == 4)
        {
            clearTerminal();
            printf("\n\n");
            displaySystemMessage("Logging you out..... Have a nice day  !", 3);
            return 0;
        }
    }
}

int NurseMain(){
    NurseName = NurseLogin();
    clearTerminal();
    
    NurseMenue(NurseName);
    return 0;
}

void StaffPortal(){
    char* header = "Welcome Staff";
    char* options[] = {"Admin", "Doctor", "Nurse", "Back"};
    
    while (1) {
        clearTerminal();
        int output = displayMenu(header,options,4);
        clearTerminal();

        if (output == 1)
        {
            adminMain();
        }
        else if (output == 2)
        {
            doctor();
        }
        else if (output == 3)
        {
            NurseMain();
        }
        else 
        {
            return;
        }
    }
    
}

int patientMainMenu() {
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
            return 0;
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

int main() 
{
    clearTerminal();
    char* header = "WELCOME TO THE HOSPITAL";
    char* options[] = {"Patient Login", "Staff Login", "Close System"};
    int output; 

    while (1) {
        clearTerminal();
        output = displayMenu(header, options, 3);

        if (output == 1)
        {
            patientMainMenu();
        }
        else if (output == 2)
        {
            StaffPortal();
        }
        else{
            displaySystemMessage("Thank You, Hope to see you soon!.....",5);
            return 0;
        }
    }
}