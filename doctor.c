#include "File_exe.h"

//char date[11] = 2024-06-01;


/////////////////////NOT MINE////////////////////////////////////////////
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

//////////////////////////UTILITY/////////////////////////////////////////

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

/////////////////////////PART OF MENU////////////////////////////

void append_slots_menu(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{
    char* d_menu = "My Availability";
    char* d_choices[] = {"Add New Slots", "Delete Current Slots", "Return to My Schedule"};
    int noOptions = 3;

    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if (d_output == 1)
        {
            displaySystemMessage("Add New Slots", 2);
            //add_new_slots(appointments, doctor_username, search_date);
        }
        else if (d_output == 2)
        {   
            displaySystemMessage("Delete Current Slots", 2);
            //delete_slots(appointments, doctor_username, search_date);
        }
        else if (d_output == 3)
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

void my_schedule(char* doctor_username) 
{
    char* d_menu = "My Schedule";
    char* d_choices[] = {"All Appointments History", "Search Appointments", "Availability", "Back"};
    int noOptions = 4;

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
            Availability(doctor_username);   
        }
        else if (d_output == 4)
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
            //My_reports();
            printf("\nMy Reports");
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