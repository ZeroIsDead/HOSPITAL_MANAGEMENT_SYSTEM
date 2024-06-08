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
    struct dataContainer2D d_appointments, d_appointment;
    
    char* previous_appointmentID;
    char new_appointmentID[256];
    int prev_count;
    int error;


    d_appointments = queryFieldStrict("doctorSchedule", "DoctorID", doctor_username);

    if (d_appointments.error) 
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
        d_appointment = filterDataContainer(d_appointments, "Date", search_date);

        if (d_appointment.error)
        {
            displaySystemMessage("You have appointment for that day.", 2);
        }
        else
        {
            printf("You have no appointment for that day.\n");
            valid = 1;
        }

    }while(!valid);


    //Generate new appointmentID
    struct dataContainer2D all_appointments;
    all_appointments = getData("Appointments");
    if (all_appointments.error == 1)
    {
         displaySystemMessage("Error, no records found");
    }
    previous_appointmentID = strdup(all_appointments.data[all_appointments.y-1][0]);
    prev_count = atoi(previous_appointmentID+3);
    sprintf(new_appointmentID, "apt%03d", prev_count+1);
    freeMalloc2D(all_appointments);

    

    //cooking for header
    char display_new_appoinmentID[256];
    sprintf(display_new_appoinmentID,"New Appointment: %s", new_appointmentID);

    //Get Paitient ID
    //clearTerminal();
    char* patientID = getUserID();


//     valid = 0;
//     char* room;
//     char* confirm;
//     char* input1[] = {doctor_username, patientID, search_date};
//     do {
//         //Get Room
//         clearTerminal();
//         char* input1[] = {doctor_username, patientID, search_date};
//         displayUnorderedOptions(display_new_appoinmentID, input1, 3);
//         room = getString("Enter Room: ");

//         //printing
//         clearTerminal();
//         char* input2[] = {doctor_username, patientID, room, search_date};
//         displayUnorderedOptions(display_new_appoinmentID, input2, 4);
        
//         confirm = getString("Confirm (Y|N): ");
        
//         if (tolower(confirm[0]) == 'y') {
//             valid = 1;
//         }

//     } while (!valid);

    

//     char* fields[] = {"TimeSlots1", "TimeSlots2", "TimeSlots3", "TimeSlots4"};
//     struct dataContainer2D d_timeslots = shortenDataContainer(d_appointment, fields, 4);

//     int count = 0;
    
//     char** data = d_timeslots.data[0];
//     char* timeslots[d_timeslots.x];

//     for (int i=0; i<d_timeslots.x; i++) {
//         if (strcmp(data[i], "NONE") != 0) {
//             timeslots[count] = strdup(data[i]);
//             count++;
//         }
//     }

//     valid = 0;
//     char* input3[5] = {doctor_username, patientID, room, "", search_date};
//     char* chosen_time;
//     int chosen_time_index;
//     do {
//         clearTerminal();
//         chosen_time_index = displayMenu(display_new_appoinmentID, timeslots, count);

//         clearTerminal();
//         input3[3] = strdup(timeslots[chosen_time_index-1]);
//         displayUnorderedOptions(display_new_appoinmentID, input3, 5);
//         confirm = getString("Confirm (Y|N): ");

//         if (tolower(confirm[0]) == 'y') {
//             valid = 1;

//             d_appointment.data[0][chosen_time_index] = strdup("NONE");
//             error = updateData("doctorSchedule", d_appointment.data[0]);

//             if (error) {
//                 displaySystemMessage("ERROR", 5);
//                 return;
//             }
//         }
 
//     } while (!valid);

//     int noFields = all_appointments.x;
//     char** created_appointment = malloc(noFields * sizeof(char*));
//     created_appointment[0] = strdup(new_appointmentID);

//     for (int i=1; i<6; i++) {
//         created_appointment[i] = strdup(input3[i-1]);
//     }

//     created_appointment[6] = strdup("");
//     created_appointment[7] = strdup("");

//     error = write_new_data("Appointments", noFields, created_appointment);

//     if (error) {
//         displaySystemMessage("ERROR", 5);
//         return;
//     }
//     free(created_appointment);

//     freeMalloc2D(d_appointment);
//     freeMalloc2D(d_appointments);
//     freeMalloc2D(d_timeslots);
}

void searchCase() 
{
    int valid = 0;
    struct dataContainer2D d_case;

    do {
        clearTerminal();
        char* case_name = getString("Enter Case Name: ");

        d_case = queryFieldStrict("Reports", "CaseName", case_name);

        if (d_case.error) {
            displaySystemMessage("Case Not Found!", 3);
        } else {
            valid = 1;
        }

    } while (!valid);

    printf("%d\n", d_case.y);
    displayTabulatedData(d_case);
}

int main() 
{   
    char* doctor_username = getString("Enter your username: ");

    create_appointment(doctor_username);
    // searchCase();
    return 0;
}
