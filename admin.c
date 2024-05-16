#include "File_exe.h"

int main() 
{
    char* header = "Welcome to the Matrix";
    char* option[] = {"Registration", "Appointment", "Inventory"};
    char* output;
    
    clearTerminal();
    output = displayMenu(header, option, 3);

    if (output==1)
    {
        char* header2 = "Registration Menu";
        char* options2[] = {"Patient","Doctor","Nurse"};

        clearTerminal();
        int output2 = displayMenu(header2, options2, 3);
    }

    else if (!strncmp(output, option[1], 255))
    {
        char* header3 = "Appointment Menu";
        char* options3[] = {"View Active Appointment List", "View Past Appointment List"};
        
        clearTerminal();
        int output3 = displayMenu(header3, options3, 2);
        printf(output3);
    }

    else if (!strncmp(output, option[2], 255))
    {
        char* header4 = "Inventory Menu";
        char* options4[] = {"View Inventory", "Search Inventory", "Restock Inventory"};
        
        clearTerminal();
        int output3 = displayMenu(header4, options4, 3);
    }
}