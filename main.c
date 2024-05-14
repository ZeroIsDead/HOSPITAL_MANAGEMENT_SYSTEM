#include "File_exe.h"

int main() 
{
    char* header = "WELCOME TO THE HOSPITAL";
    char* options[] = {"Patient Login", "Staff Login"};
    int output; 

    clearTerminal();
    output = displayMenu(header, options, 2);

    if (output == 1)
    {
        char* header2 = "HOW CAN I HELP TODAY";
        char* options2[] = {"Option1", "Option2", "Option3", "Option4"};
        int output2;

        clearTerminal();
        output2 = displayMenu(header, options2, 4);
        printf("%d", output2);

    }
    else if (output == 2)
    {
        char* header3 = "SELEKT YOUR ROLE";
        char* options4[] = {"Doctor", "Nurse", "Admin"};
        int output3;
        
        clearTerminal();
        output3 = displayMenu(header3, options4, 3);
        if (output3 == 1)
        {
            printf("Doctor");
            //doctor() starts here
        }
        else if (output3 == 2)
        {
            printf("Nurse");
            //nurse() starts here
        }
        else if (output3 == 3)
        {
            printf("Admin");
            //admin() starts here
        }
        
        
    }
}