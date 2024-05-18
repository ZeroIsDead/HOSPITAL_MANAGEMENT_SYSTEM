#include "File_exe.h"


#define MAX_LINE_LENGTH 256

char* getUserID() 
{
    char* username;
    int valid = 0;

    do 
    {
        username = getString("Enter patient`s name: ");
        struct dataContainer2D userData = queryFieldStrict("Patient_IDs", "Name" ,username);

        if (userData.error == 1)  // userData.error will be 1 if the username is not found
        {
             displaySystemMessage("Username not found!", 2);
        }
        else
        {
            valid = 1;
        }

    } while (!valid);

    return username;
}

int main() 
{   

    char* userID = getUserID();
    printf("Hi, %s", userID);

}