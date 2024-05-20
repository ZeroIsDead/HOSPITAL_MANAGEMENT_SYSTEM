#include "File_exe.h"

void gettime(char* d_choices[])
{   
    char* token;
    int start_hour, start_minutes, end_hour, end_minutes;

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

    printf("Start hour: %02d, Start minutes: %02d, End hour: %02d, End minutes: %02d\n", start_hour, start_minutes, end_hour, end_minutes);

}


void delete_slots(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{
    printf("rows: %d\n", appointments.y);
    printf("column:%d\n", appointments.x);

    /*Index note
    appointments.data[0][1] = Slots1
    appointments.data[0][2] = Slots2
    appointments.data[0][3] = Slots3
    appointments.data[0][4] = Slots4
    */

    
    char* d_menu = "Which time slots to delete?";
    char* d_choices[appointments.x];
    int choice_index = 0;
    
    for (int j = 1 ; j < appointments.x - 1; j++)
    {
        d_choices[choice_index] = appointments.data[0][j];
        choice_index++;
    }

    //print menu
    int d_output = displayMenu(d_menu, d_choices, choice_index);

    char* newtime;
    if (d_output == 1)
    {
        //printf("gettime\n");
        gettime(d_choices);
    }
    else if (d_output == 2)
    {
        printf("gettime2\n");
    }
    
}

// void add_new_slots(struct dataContainer2D appointments, char* doctor_username, char* search_date)
// {       
//         struct dataContainer2D  d_slots;
//         char* wantedFields[] = {"TimeSlots"};
//         int no_of_fields = 1;
        
//         d_slots = shortenDataContainer(appointments, wantedFields, no_of_fields);
        
//         //cooking for menu
//         char* d_menu = "Your time table";
//         char* d_choices[d_slots.y];

//         for (int i = 0; i < d_slots.y; i++)
//         {
//             d_choices[i] = d_slots.data[i][0];
//         }
//         int noOptions = d_slots.y;

//         displayMenu(d_menu, d_choices, noOptions);
// }

int main() 
{   
    char* doctor_username = getString("Enter your username: ");
    char* search_date = getString("Enter the date you wish to read (yyyy-mm-dd): ");

    struct dataContainer2D d_appointments = queryFieldStrict("doctorSchedule", "Date", search_date);

    struct dataContainer2D appointments = filterDataContainer(d_appointments, "DoctorID", doctor_username);
    
    
    delete_slots(appointments, doctor_username, search_date);
}