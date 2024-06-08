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
        return;
    }

    freeMalloc2D(chosen_day);
    freeMalloc2D(d_appointment);
    freeMalloc2D(particular_day);

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
