#include "File_exe.h"

int delete_non_primary_Key(const char* filename, char* first_key, char* second_key, int second_key_index) 
{
    struct dataContainer2D master = getData(filename);
    
    master.y = master.y - 1;

    for (int i=0; i<master.y; i++) 
    {   
        //IF FOUND the unique
        if(!strncmp(master.data[i][0], first_key, 255))
        {   
            if (!strncmp(master.data[i][second_key_index], second_key, 255))
            {
                //delete that record
                free(master.data[i]);
                for (int j=i; j<master.y; j++)
                {
                    master.data[j] = master.data[j+1];
                }
            }
                
        }
    }    

    return writeData(filename, master);
}


    
//     char new_time[256];
//     sprintf(new_time, "Uploading new time: %s\n", newtime);
//     displaySystemMessage(new_time, 2);

//     update_non_primary_Data("doctorSchedule", appointments.data[0], search_date, 5);

//     displaySystemMessage("Update Successful! ", 2);

// }
    //delete_non_primary_Key("doctorSchedule", appointments.data[0], search_date, 5);


// void delete_slots()
// {

// }


void delete_entire_day(struct dataContainer2D appointments, char* doctor_username, char* search_date)
{   
    clearTerminal();
    displayTabulatedData(appointments);
    printf("\nDo you sure to remove your entire schdule for this day?( y for yes / Press anykey to return)\n");
    char* input = getString("Your input: ");

    if (strcmp(input, "y") == 0)
    {
        char* username = getString("Enter your username for comfirmation: ");

        if (strcmp(username, doctor_username) == 0)
        {    
            delete_non_primary_Key("doctorSchedule", doctor_username, search_date, 5);
        }
        else
        {
            clearTerminal();
            printf("\n\n");
            getString("WRONG USERNAME, PRESS ENTER TO RETURN...");
        }
        
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
    char* search_date = getString("Enter the date you wish to remove (yyyy-mm-dd): ");

    struct dataContainer2D d_appointments = queryFieldStrict("doctorSchedule", "Date", search_date);

    struct dataContainer2D appointments = filterDataContainer(d_appointments, "DoctorID", doctor_username);
    
    delete_entire_day(appointments, doctor_username, search_date);
}