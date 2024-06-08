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
    struct dataContainer2D all_schedule, chosen_day;
    
    all_schedule = queryFieldStrict("doctorSchedule", "DoctorID", doctor_username);

    if (all_schedule.error) 
    {
        displaySystemMessage("Error , no records found", 2);
    }

/////////////////////////////////////////////////////////////////////////////////////////////

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

    printf("we good2");
    d_appointment = queryFieldStrict("Appointments", "DoctorID", doctor_username);
    if (d_appointment.error == 1) 
    {
        printf("Error occurred while querying the database.\n");
        return;
    }
    particular_day = filterDataContainer(d_appointment, "Date", search_date);
    printf("we good3");
    printf("we good3");

    //cooking for header
    char display_new_appoinmentID[256];
    sprintf(display_new_appoinmentID,"New Appointment: %s", new_appointmentID);

    //Get Paitient ID
    //clearTerminal();
    char* patientID = getUserID();


}

// void searchCase() 
// {
//     int valid = 0;
//     struct dataContainer2D d_case;

//     do {
//         clearTerminal();
//         char* case_name = getString("Enter Case Name: ");

//         d_case = queryFieldStrict("Reports", "CaseName", case_name);

//         if (d_case.error) {
//             displaySystemMessage("Case Not Found!", 3);
//         } else {
//             valid = 1;
//         }

//     } while (!valid);

//     printf("%d\n", d_case.y);
//     displayTabulatedData(d_case);
// }

int main() 
{   
    char* doctor_username = getString("Enter your username: ");

    create_appointment(doctor_username);
    // searchCase();
    return 0;
}
