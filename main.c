#include "File_exe.h"

///////////////////patient.c///////////////////////

//////////////////doctor.c/////////////////////////

///////////////////nurse.c/////////////////////////

////////////////////admin.c/////////////////////////

/////////////////////main.c////////////////////////
void StaffPortal(){
    char* header = "Welcome Staff";
    char* options[] = {"Admin", "Doctor", "Nurse", "Back"};
    int output = displayMenu(header,options,4);
    clearTerminal();

    if (output == 1){
        //Call AdminMain
        printf("ADMIN PORTAL\n");
    }
    else if (output == 2){
        // Call DoctorMain
        printf("DOCTOR PORTAL\n");
    }
    else if (output == 3){
        // Call NurseMain
        printf("NURSE PORTAL\n");
    }
    else {
        clearTerminal();
        main();
        return;
    }
}

int main() 
{
    char* header = "WELCOME TO THE HOSPITAL";
    char* options[] = {"Patient Login", "Staff Login"};
    int output; 

    clearTerminal();
    output = displayMenu(header, options, 2);

    if (output == 1)
    {
        
        int paitent = 2;// Call Paitent Main
        while (paitent == 1){
            displaySystemMessage("WRONG! PAITENT!",5);
            paitent = 2;//Call Paitent Main
        }

    }
    else if (output == 2)
    {
        StaffPortal();
        return 0;
    }
}