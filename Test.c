#include "File_exe.h"


#define MAX_LINE_LENGTH 256

char* getValidUsername()
{
    char* doctor_username;
    struct dataContainer1D userData;
    int valid = 0;

    do 
    {
        clearTerminal();
        doctor_username = getString("Enter your username: ");
        userData = queryKey("Staff_IDs", doctor_username);

        if (userData.error == 1)  // userData.error will be 1 if the username is not found
        {
            displaySystemMessage("Username not found!", 2);
        }
        else
        {
            valid = 1;
        }

    }while(!valid);

    int valid2 = 0;

    do
    {
        char* doctor_pw = getString("Enter your password: ");

        if (!strcmp(doctor_pw, userData.data[1]))
        {
            valid2 = 1;
        }
        else
        {
            displaySystemMessage("Wrong password, please try again!", 2);
        }
    }
    while (!valid2);
    
    displaySystemMessage("Login successful!", 2);

    freeMalloc1D(userData);

    return doctor_username;
}


int main() 
{   

    char* doctor_username = getValidUsername();
    printf("Hi, %s", doctor_username);

}