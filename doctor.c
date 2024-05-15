#include "File_exe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

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

int doctor() 
{
    char* d_menu = "Doctor";
    char* d_choices[] = {"My Schedule", "Staff Login"};
    int d_output; 

    char* doctor_username = getString("Enter your username: ");

    clearTerminal();
    d_output = displayMenu(d_menu,d_choices,2);

    if(d_output ==1 )
    {
        struct dataContainer2D d_appointments = queryFieldStrict("Appointments", "StaffUserID",doctor_username);
        
        displayTabulatedData(d_appointments);
        freeMalloc2D(d_appointments);

        // char line[sizeof(d_appointments.x)];
        // line[0] = '\0';
        
        // for (int i = 0; i < d_appointments.x; i++)
        // {    
        //     for (int i = 0; i < d_appointments.x; i++)
        //     {
        //         strcat(line, d_appointments.data[i][j]); 
        //         strcat(line, ";");
        //     }
        // }


        // clearTerminal();
        // for (int i = 0; i < d_appointments.x; i++)
        // {
        //     for (int j = 0; j < d_appointments.y; j++)
        //     {
        //         printf("%s ;", d_appointments.data[i][j]);
        //     }
        //     printf("\n");
        // }

    }   
}

int main() 
{
    doctor();
}