#include "File_exe.h"

int main() 
{
    char* header = "WELCOME TO THE HOSPITAL";
    char* options[] = {"Patient Login", "Staff Login"};
    char* output; 

    clearTerminal();
    output = displayMenu(header, options, 2);

    if (strcmp(output, options[0]) == 0)
    {
        char* header2 = "HOW CAN I HELP TODAY";
        char* options2[] = {"Option1", "Option2", "Option3", "Option4"};
        char* output2;

        clearTerminal();
        output2 = displayMenu(header, options2, 4);
        printf(output2);
    }
    else  (strcmp(output, options[1]));
    {
        char* header3 = "SELEKT YOUR ROLE";
        char* options4[] = {"Doctor", "Nurse", "Admin"};
        char* output3;

        clearTerminal();
        output3 = displayMenu(header3, options4, 3);
    }
    
}   