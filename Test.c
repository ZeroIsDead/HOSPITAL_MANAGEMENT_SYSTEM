#include "File_exe.h"


#define MAX_LINE_LENGTH 256

struct dataBuffer2D 
{
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char*** data; // a 2D array containing each line of data
    int y; //y - number of rows / lines / arrays in the 2D array
    int x; //x - number of columns / elements in each array
};

int main()
{      
    char* d_menu = "Doctor";
    char* d_choices[] = {"My Schedule", "Staff Login"};
    int d_output; 

    char* doctor_username = getString("Enter your username: ");

    struct dataContainer2D d_appointments = queryFieldStrict("Appointments", "StaffUserID",doctor_username);

    struct dataBuffer2D container;

    for (int i=0; i<d_appointments.y + 1; i++) 
    {
        for (int j=0; j<d_appointments.x + 1; j++) 
        {
            container.data[i][j+1]= d_appointments.data[i][j];
        }
    }

    printf("No of rows: %d\n", d_appointments.y);
    printf("No of columns: %d", d_appointments.x);

    d_appointments.data[0][0] = "Hello";
    
    printf("%s\n",d_appointments.data[0][0]);

    for (int i=0; i<d_appointments.y + 1; i++) 
    {
        for (int j=0; j<d_appointments.x + 1; j++) 
        {
            printf("%s\n", d_appointments.data[i][j]);
        }
    }  
 
}