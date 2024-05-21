#include "File_exe.h"

int update_non_primary_Data(const char* filename, char** relaying_array, char* third_key, int index) 
{   
    //getData of the file
    struct dataContainer2D master = getData(filename);
    const int uniqueIDIndex = 0;

    //iterate vertically through data search for the unique key
    for (int i=0; i<master.y; i++) 
    {   
        //IF FOUND the unique
        if(!strncmp(master.data[i][uniqueIDIndex], relaying_array[uniqueIDIndex], 255))
        {
            //store the relaying array into master
            
            if (!strncmp(master.data[i][index], third_key, 255))
            {
                for (int j=0; j<master.x; j++) 
                {
                    master.data[i][j] = strdup(relaying_array[j]);
                }
            }
        
        }
    }    

    return writeData(filename, master);
}

char* gettime(char* d_choices[], int index)
{
    char* token;
    int start_hour, start_minutes, end_hour, end_minutes;

    char time_slot[256];

    sprintf(time_slot, "%s", d_choices[index]);

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
    
    //printf("Start hour: %02d, Start minutes: %02d, End hour: %02d, End minutes: %02d\n", start_hour, start_minutes, end_hour, end_minutes);
    
    int new_start_hour;
    int new_start_minutes;
    int new_end_hour;
    int new_end_minutes;
    
    ///START HOUR///
    int valid = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Current slots start: %02d:%02d\n", start_hour, start_minutes);
        sprintf(new_time_format, " [ _ _ : %02d ]\nNew Hour ( 0-23 ): ", start_minutes);
        new_start_hour = getInt(new_time_format);

        if (new_start_hour > 0 && new_start_hour < 24)
        {
            valid = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (24h format only)!", 2);
        }

    }while(!valid);

    ///START MINUTES///
    int valid2 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Current slots start : %02d:%02d\n", new_start_hour, start_minutes);
        sprintf(new_time_format, " [ %02d: _ _ ]\nNew Minutes: ", new_start_hour);
        new_start_minutes = getInt(new_time_format);

        if (new_start_minutes >= 0 && new_start_minutes < 60)
        {
            valid2 = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (0-59) only)!", 2);
        }

    }while(!valid2);

    ///END HOUR///
    int valid3 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Slots start : %02d:%02d\n", new_start_hour, new_start_minutes);
        printf("Current slots ends : %02d:%02d\n", end_hour, end_minutes);
        sprintf(new_time_format, " [ _ _ : %02d ]\nNew Hour ( %02d-23 ): ", end_minutes, new_start_hour);
        new_end_hour = getInt(new_time_format);

        if (new_end_hour > 0 && new_end_hour < 24 && new_end_hour > new_start_hour) 
        {
            valid3 = 1;
        }
        else
        {   
            char new_error_message[256];
            sprintf(new_error_message, " Invalid Time input! Your new slots can ends at ( %02d-23 ) only! : ",new_start_hour);
            displaySystemMessage(new_error_message, 2);
        }

    }while(!valid3);

    ///END MINUTES///
    int valid4 = 0;
    do 
    {   
        clearTerminal();
        char new_time_format[256];
        printf("Slots start at: %02d:%02d\n", new_start_hour, new_start_minutes);
        printf("Current slots ends : %02d:%02d\n", end_hour, end_minutes);
        sprintf(new_time_format, " [ %02d: _ _ ]\nNew Minutes: ", new_end_hour);
        new_end_minutes = getInt(new_time_format);

        if (new_end_minutes >= 0 && new_end_minutes < 60)
        {
            valid4 = 1;
        }
        else
        {
            displaySystemMessage("Invalid Time input (0-59) only)!", 2);
        }

    }while(!valid4);

    printf("New slots start at: %02d:%02d\n", new_start_hour, new_start_minutes);
    printf("New slots ends at: %02d:%02d\n", new_end_hour, new_end_minutes);

    char new_time[256];
    sprintf(new_time, "%02d:%02d-%02d:%02d", new_start_hour, new_start_minutes, new_end_hour, new_end_minutes);

    char* new_time_ptr = malloc(strlen(new_time) + 1);
    if(new_time_ptr == NULL) 
    {
        displaySystemMessage("Memory allocation failed", 2);
        return NULL;
    }
    strcpy(new_time_ptr, new_time);
    return new_time_ptr;
    
}

void ammend_slot(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{
    printf("rows: %d\n", appointments.y);
    printf("column:%d\n", appointments.x);

    /*Index note
    appointments.data[0][1] = Slots1
    appointments.data[0][2] = Slots2
    appointments.data[0][3] = Slots3
    appointments.data[0][4] = Slots4
    */

    
    char* d_menu = "Which time slots to amend?";
    char* d_choices[appointments.x];
    int choice_index = 0;
    
    for (int j = 1 ; j < appointments.x - 1; j++)
    {
        d_choices[choice_index] = appointments.data[0][j];
        choice_index++;
    }

    //print menu
    int d_output = displayMenu(d_menu, d_choices, choice_index);

    int index;
    char* newtime;
    if (d_output == 1)
    {   
        index = 0;
        newtime = gettime(d_choices,index);
        appointments.data[0][1] = newtime;
    }
    else if (d_output == 2)
    {
        index = 1;
        newtime = gettime(d_choices,index);
        appointments.data[0][2] = newtime;
    }
    else if (d_output == 3)
    {
        index = 2;
        newtime = gettime(d_choices,index);
        appointments.data[0][3] = newtime;
    }
    else if (d_output == 4)
    {
        index = 3;
        newtime = gettime(d_choices,index);
        appointments.data[0][4] = newtime;
    }
    
    char new_time[256];
    sprintf(new_time, "Uploading new time: %s\n", newtime);
    displaySystemMessage(new_time, 2);

    update_non_primary_Data("doctorSchedule", appointments.data[0], search_date, 5);

    displaySystemMessage("Update Successful! ", 2);

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
    
    
    ammend_slot(appointments, doctor_username, search_date);
}