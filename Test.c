#include "File_exe.h"

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

void create_appointment(char* doctor_username)
{   
    struct dataContainer2D all_appointments;
    char* previous_appointmentID;
    char new_appointmentID[256];
    int prev_count;

    //Generate new appointmentID
    all_appointments = getData("Appointments");
    if (all_appointments.error == 1)
    {
        printf("Error");
    }
    previous_appointmentID = strdup(all_appointments.data[all_appointments.y-1][0]);
    prev_count = atoi(previous_appointmentID+3);
    sprintf(new_appointmentID, "apt0%d", prev_count+1);
    freeMalloc2D(all_appointments);

    //get date
    char* search_date;
    int valid = 0;
    do
    {   
        clearTerminal();
        search_date = getString("Date of new appointment (yyyy-mm-dd) : ");
        struct dataContainer2D d_appointments = queryFieldStrict("doctorSchedule", "Date", search_date);

        if (strcmp(search_date, d_appointments.data[0][5]) != 0)
        {
            displaySystemMessage("No appointment for that day!", 2);
        }
        else
        {
            valid = 1;
        }

    }while(!valid);

    //cooking for header
    char display_new_appoinmentID[256];
    sprintf(display_new_appoinmentID,"New Appointment: %s", new_appointmentID);

    //Get Paitient ID
    //clearTerminal();
    char* patientID = getUserID();

    //Get Room
    char* input1[3] = {doctor_username, patientID};
    displayUnorderedOptions(display_new_appoinmentID, input1, 2);
    char* room = getString("Enter Room: ");

    //printing
    clearTerminal();
    char* input2[3] = {doctor_username, patientID, room};
    displayUnorderedOptions(display_new_appoinmentID, input2, 3);

    printf("Appointment ID: %s\n", new_appointmentID);
    printf ("Doctor ID: %s\n", doctor_username);
    printf ("Patient ID: %s\n", patientID);
 }

int main() 
{   
    char* doctor_username = getString("Enter your username: ");

    create_appointment(doctor_username);
    return 0;
}
