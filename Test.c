#include "File_exe.h"

char* get_new_time()
{
    //getinput
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
        sprintf(new_time_format, " [ _ _ : _ _ ]\nEnter New Hour ( 0-23 ): ");
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
        sprintf(new_time_format, " [ %02d: _ _ ]\nEnter New Minutes: ", new_start_hour);
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
        sprintf(new_time_format, " [ _ _ : _ _ ]\nEnter End Hour ( %02d-23 ): ", new_start_hour);
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
        sprintf(new_time_format, " [ %02d: _ _ ]\nEnter New Minutes: ", new_end_hour);
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

    clearTerminal();
    printf("New slots start at:[ %02d:%02d ]\n", new_start_hour, new_start_minutes);
    printf("New slots ends at: [ %02d:%02d ]\n", new_end_hour, new_end_minutes);

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



void create_new_Schedule(char* doctor_username)
{   
    int valid_date = 0;
    char* new_date;
    do
    {    
        clearTerminal();
        new_date = getString("Enter date for new schedule (yyyy-mm-dd): ");
        
        struct dataContainer2D existing_schedule = queryFieldStrict("doctorSchedule", "Date", new_date);

        if (is_valid_date(new_date)) 
        {
            if (existing_schedule.error == 1)
            {
                valid_date = 1;
            }
            else
            {
                displaySystemMessage("You have already created a schedule for this day!\nGo to <Change My Schedule>", 2);
                clearTerminal();
                printf("\n\n");
                getString("Press any key to return...");

                freeMalloc2D(existing_schedule);

                return;
            }
        }
        else 
        {
            displaySystemMessage("Invalid date!!!", 2);
        }
    }while(!valid_date);

    //do prettier UI afterwards
    char* new_time1 = get_new_time();
    getString("\n\nPress any key to continue...");
    char* new_time2 = get_new_time();
    getString("\n\nPress any key to continue...");
    char* new_time3 = get_new_time();
    getString("\n\nPress any key to continue...");
    char* new_time4 = get_new_time();
    getString("\n\nPress any key to continue...");


    char* inputs[] = {doctor_username, new_time1, new_time2, new_time3, new_time4, new_date};
    write_new_data("doctorSchedule", 6 , inputs);
}


void availability_menu(char* doctor_username)
{
    char* d_menu = "My Availability";
    char* d_choices[] = {"Change My Schedule", "New Schedule","Back"};
    int noOptions = 3;

    while (1)
    {
        clearTerminal();
        int d_output = displayMenu(d_menu, d_choices, noOptions);

        if (d_output == 1)
        {

        }
        else if (d_output == 2)
        {   
            create_new_Schedule(doctor_username);
        }
        else if (d_output == 3)
        {
            return;
        }
    }
}

int main() 
{   
    char* doctor_username = getString("Enter your username: ");

    availability_menu(doctor_username);
}
