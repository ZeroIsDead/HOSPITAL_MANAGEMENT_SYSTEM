#include "File_exe.h"

void search_Appointments(char* doctor_username)
{   
    struct dataContainer2D d_appointments;
    struct dataContainer2D appointments;
    char* search_date;
    int valid = 0;

    do
    {   
        clearTerminal();
        search_date = getString("Please Enter the Appointment date (yyyy-mm-dd): ");
        d_appointments = queryFieldStrict("Appointments", "Date", search_date);

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
    appointments = filterDataContainer(d_appointments, "StaffUserID", doctor_username);

    displayTabulatedData(appointments);

}

int main() 
{   
    char* doctor_username = getString("Enter your username: ");
    search_Appointments(doctor_username);
}