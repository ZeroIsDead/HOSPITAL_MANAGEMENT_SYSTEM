#include "File_exe.h"

//char date[11] = 2024-06-01;

//////////////////////////////UTILITY///////////////////////////////////

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

    return userData.data[0][0];
}

char* getValidUsername()
{
    char* username;
    struct dataContainer1D userData;
    int valid = 0;

    do 
    {
        //clearTerminal();
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
//////////////////////////////EHR///////////////////////////////////////////////////

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

    if (choice == noOptions) {
        freeMalloc1D(appointmentIDs);
        freeMalloc2D(appointments);
        return;
    }

    char* chosenID = appointments.data[choice-1][6]; // PresciptionID 

    displayPrescriptions(chosenID);

    freeMalloc1D(appointmentIDs);
    freeMalloc2D(appointments);
}

void EHRMenu() 
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

    int index;
    char* newtime;
    if (d_output == 1)
    {   
        index = 0;
        newtime = gettime(d_choices,index);
        appointments.data[0][1] = newtime;
    }
    else if (d_output == 2)
    {
        index = 1;
        newtime = gettime(d_choices,index);
        appointments.data[0][2] = newtime;
    }
    else if (d_output == 3)
    {
        index = 2;
        newtime = gettime(d_choices,index);
        appointments.data[0][3] = newtime;
    }
    else if (d_output == 4)
    {
        index = 3;
        newtime = gettime(d_choices,index);
        appointments.data[0][4] = newtime;
    }
    else if (d_output == 5)
    {
        return;
    }
    
    char new_time[256];
    sprintf(new_time, "Uploading new time: %s\nPlease wait....", newtime);
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
    while (i < d_appointments.y)
    {
        d_reports[i] = strdup(d_appointments.data[i][7]);
        i++;
    }
    d_reports[d_appointments.y] = strdup("Back");
    
    //cooking for menu
    char* header = "My Reports";
    int noOptions = i+1;

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
        else if (appointments.data[7] != NULL)
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
            EHRMenu();
        }
        else if (d_output == 2)
        {   
            /*Search Case*/
            clearTerminal();
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

char* Availability(char* doctor_username)
{
    struct dataContainer2D d_appointments;
    struct dataContainer2D buffer_appointments;
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
            //create_appointment(doctor_username);
            printf("Create Appointment\n");
        }
        else if (d_output == 4)
        {
            //create_appointment(doctor_username);
            availability_menu(doctor_username); 
        }
        else if (d_output == 5)
        {
            return;
        }
    }
}

int doctor() 
{   
    char* doctor_username = getValidUsername();

    //cooking infomation for menu
    struct dataContainer1D userData = queryKey("Staff_IDs", doctor_username);

    char d_menu[sizeof(userData.data[2])];
    sprintf(d_menu, "Hi, %s", userData.data[2]);
    char* d_choices[] = {"My Schedule", "EHR access", "My Reports", "Logout"};
    int noOptions = 4;
        
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

int main() 
{  
    doctor();
}