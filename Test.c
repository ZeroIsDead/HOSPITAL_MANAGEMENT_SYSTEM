#include "File_exe.h"


#define MAX_LINE_LENGTH 256

int main() 
{
    char* doctor_username = getString("Enter your username: ");

    struct dataContainer2D d_appointments = queryFieldStrict("EHR", "DoctorID", doctor_username);

    struct dataContainer2D container;

    container.x = d_appointments.x + 1;
    container.y = d_appointments.y;

    // allocate memory for fields
    container.fields = malloc(container.x * sizeof(char*));

    container.fields[0] = "No.";

    // copy fields from d_appointments to container
    for (int i = 0; i < d_appointments.x; i++)
    {
        container.fields[i+1] = strdup(d_appointments.fields[i]);
    }

    container.data = malloc(d_appointments.y * sizeof(char**));
    for (int i = 0; i < d_appointments.y; i++) //for every rows
    {
        container.data[i] = malloc(container.x * sizeof(char*));

        char numStr[3];
        sprintf(numStr, "%d", i+1);
        container.data[i][0] = strdup(numStr);

        for (int j = 0; j < d_appointments.x; j++)
        {
            container.data[i][j+1] = strdup(d_appointments.data[i][j]);
        }
    }


    /////////////PRINTING/////////////////////

    displayTabulatedData(container);

    ///////////////////FREE MEMORY/////////////////////
    freeMalloc2D(d_appointments);
    freeMalloc2D(container);
}