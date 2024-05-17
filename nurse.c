#include "File_exe.h"

char* NurseName;

char* NurseWelcomeMessage(char name []){ 
    
    int welcomeSize = 8 + strlen(name) + 1;
    char welcome[welcomeSize];
    
    strcpy(welcome, "Welcome ");
    strcat(welcome, name);
    
    welcome[welcomeSize-1] = '\0';

    return strdup(welcome);
    
}

void NurseDoctorSchedule(){
    char* header = "Doctor Schedule";
    char* option[] = {"Option 1", "Option 2", "Back"};
    int output = displayMenu(header, option, 3);
    
    clearTerminal();

    if (output == 3){
        NurseMenue(NurseName);
    }
}

void NurseInventory(){
    printf("This is the Inventory Menue");
}

void NurseReport(){
    printf("This is the Report Menue");
}

/* Copy Paste this Function, Change "Nurse" to tag you want to search "Admin" "Doctor", also change index of '\0'
 userNameCheck.data[3][5] = '\0' (for Admin)
 userNameCheck.data[3][6] = '\0' (for Doctor)
 function will return a string pointer
 
 char* name = Login(); */ 

char* NurseLogin(){
    
    displaySystemMessage("Opening Nurse Log in Portal",5);
    
    char* userName = getString("Enter Your UserName: ");
    struct dataContainer1D userNameCheck = queryKey("Staff_IDs", userName);
    
    if (userNameCheck.error == 0){
        userNameCheck.data[3][5] = '\0';
    }

    while (userNameCheck.error == 1 || strcmp(userNameCheck.data[3],"Nurse")){
        if (userNameCheck.error == 1)
        {
            displaySystemMessage("Invalid UserName",3);
        }else{
            displaySystemMessage("User is not a Nurse",3);
        }
        userName = getString("Enter Your UserName: ");
        userNameCheck = queryKey("Staff_IDs", userName);
        userNameCheck.data[3][5] = '\0';
    }

    char* password = getString("Enter Your Password: ");

    while(strcmp(userNameCheck.data[1],password)){
        displaySystemMessage("Wrong Password, Please try agian",3);
        password = getString("Entrer Your Password: ");    
    }       

    return strdup(userNameCheck.data[2]);
    
}

int NurseMenue(char* name){

    char* header = NurseWelcomeMessage(name);
    char* options[] = {"Doctor Schedules","Inventory","Report","Log Out"};

    int output = displayMenu(header, options, 4);
    clearTerminal();
    
    if (output == 1){
        NurseDoctorSchedule();
    }else if (output == 2){
        NurseInventory();
    }else if(output == 3){
        NurseReport();
    }else{
        main();
        return 0;
    }
}

int main(){
    NurseName= NurseLogin();
    clearTerminal();
    
    NurseMenue(NurseName);
}
