#include "File_exe.h"

void add_new_slots(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{       
        struct dataContainer2D  d_slots;
        char* wantedFields[] = {"TimeSlots"};
        int no_of_fields = 1;
        
        d_slots = shortenDataContainer(appointments, wantedFields, no_of_fields);
        
        printf("%d", d_slots.error);

        displayTabulatedData(d_slots);

        // //cooking for menu
        // char* d_menu = "Choose the slots you wish y";
        // char* d_choices[] = {"All Appointments History", "Search Appointments", "Availability", "Back"};
        // int noOptions = 4;
    

        // displayMenu
}

void append_slots_menu(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{
    char* d_menu = "My Avaialbility";
    char* d_choices[] = {"Add New Slots", "Delete Current Slots", "Return to my Schedule"};
    int noOptions = 3;

    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if (d_output == 1)
        {
            //displaySystemMessage("Add New Slots", 2);
            add_new_slots(appointments, doctor_username, search_date);
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

char* Availability(char* doctor_username)
{
    struct dataContainer2D d_appointments;
    struct dataContainer2D appointments;
    
    char* search_date;
    int valid = 0;

    do
    {   
        clearTerminal();
        search_date = getString("Please enter your schedule date (yyyy-mm-dd): ");
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

    //print dr`s availability
    displayTabulatedData(appointments);

    //ask for appending slots
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



int main() 
{   
    char* doctor_username = getString("Enter your username: ");
    Availability(doctor_username);
}