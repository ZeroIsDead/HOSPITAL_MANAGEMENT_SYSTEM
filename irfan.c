#include "File_exe.h"

int main() 
{
    char* header = "WELCOME TO THE HOSPITAL";
    char* options[] = {"Patient Login", "Staff Login"};
    int output; 

    output = displayMenu(header, options, 2);

    if (output == 1)
    {
        char* header2 = "HOW CAN I HELP TODAY";
        char* options2[] = {"Option1", "Option2", "Option3", "Option4"};
        int output2;

        output2 = displayMenu(header, options2, 4);
    }
    else if (output == 2)
    {
        char* header3 = "SELEKT YOUR ROLE";
        char* options4[] = {"Doctor", "Nurse", "Admin"};
        int output3;

        output3 = displayMenu(header3, options4, 3);
    }
    else 
    {
        printf("Invalid input\n");
    }
}   