#include "File_exe.h"

//char date[11] = 2024-06-01;

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

void EHR_access(char* doctor_username)
{
    struct dataContainer2D records = queryFieldStrict("EHR", "DoctorID", doctor_username);

    clearTerminal();
    if (records.error) 
    {
        displaySystemMessage("Failed to Access Electronic Health Records...Ask for Technician`s Help", 3);
        return;
    }

    if (!records.y) 
    {
        displaySystemMessage("No records found!!", 3);
        return;
    }
    displayTabulatedData(records);

    printf("\n\n");
    getString("PRESS ENTER TO RETURN...");
    
    freeMalloc2D(records);
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

int my_schedule(char* doctor_username) 
{
    char* d_menu = "My Schedule";
    char* d_choices[] = {"All Appointments History", "Search Appointments", "Back"};
    int noOptions = 3;

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
            /*Search Appointments*/
            clearTerminal();
            char* search_date = getString("Please Enter the Appointment date (yyyy-mm-dd): ");
        }
        else if (d_output == 3)
        {
            return;
        }
    }
}

int doctor(char* doctor_username) 
{
    
    
    char* d_menu = "Doctor";
    char* d_choices[] = {"My Schedule", "EHR access", "My Reports", "Logout"};
    int noOptions = 4;

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
            //EHR_access();
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
    clearTerminal();
    char* d_username = getString("Enter your username: ");
    doctor(d_username);
}