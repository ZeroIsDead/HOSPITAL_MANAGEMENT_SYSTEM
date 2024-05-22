#include "File_exe.h"

char* NurseName;

void NurseBack(){

    int back = 1;
    back = getInt("\nPlease Enter 0 to go back");

    if (back != 0 ){

        displaySystemMessage("Please enter the corrent input!: ",2);
        
        NurseBack();
        return;
    }
    else{
     
        return;
    
    }
}

char* NurseWelcomeMessage(char name []){ 
    
    int welcomeSize = 8 + strlen(name) + 1;
    char welcome[welcomeSize];
    
    strcpy(welcome, "Welcome ");
    strcat(welcome, name);
    
    welcome[welcomeSize-1] = '\0';

    return strdup(welcome);
    
}


/* Copy Paste this Function, Change "Nurse" to tag you want to search "Admin" "Doctor", also change index of '\0'
 userNameCheck.data[3][5] = '\0' (for Admin)
 userNameCheck.data[3][6] = '\0' (for Doctor)
 function will return a string pointer
 
 char* name = Login(); */ 

char* NurseLogin(){
    
    displaySystemMessage("Opening Nurse Log in Portal",2);
    
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

void NCurrentDoctorSchedules(){
    
    // Display Doctor Schedule Menu
    char* option[] = {"All Doctor Schedule", "Specific Doctro Schedule", "Back"};
    int output = displayMenu("Doctor Schedule", option ,3);
    //Display All Doctor Schedule
    if (output == 1){
        clearTerminal();
        
        struct dataContainer2D allDocSchedule = queryFieldStrict("Staff_IDs","Tags","Doctor");
        displayTabulatedData(allDocSchedule);
        
        printf("\n");
        
        NurseBack();
        NCurrentDoctorSchedules();
        return;
    }

    // Searching and Displaying Specific Doctor Schedule
    else if (output == 2){
        char* docName = getString("Please enter the Name of the Doctor: ");
        struct dataContainer1D docSchedule = queryKey("Staff_IDs", docName);
            
        while (docSchedule.error == 1){
            displaySystemMessage("Error!!",2);
            docName = getString("Please enter the Name of the Doctor: ");
            docSchedule = queryKey("Staff_IDs", docName);
        }
        docSchedule.data[3][6] = '\0';
        
        for (int i = 0; i < docSchedule.x; i++){
            printf("%s, ", docSchedule.data[i]);
        }
        
        NurseBack();
        NCurrentDoctorSchedules();
        return;
    }

    // Returning to the Nurse Menu
    else{

        NurseMenue(NurseName);
        return;
    }
}
void NAvailableDoctor(){
    printf("Available Doctor\n");
}
void NViewStationInventory(){
    printf("View Station Inventory\n");
}

void NUpdateStationInventory(){
    printf("Update Station Inventory\n");
}

void NViewPatientReport(){
    printf("View Patient Report\n");
}

void NViewUnitReport(){
    printf("View Unit Report\n");
}

void NurseMenue(char* name){

    char* header = NurseWelcomeMessage(name);
    char* options[] = {"Current Doctor Schedules","Available Doctor","View Station Inventory","Update Station Inventory","View Patient Report","View Unit Report","Log Out"};

    int output = displayMenu(header, options, 7);
    clearTerminal();
    
    if (output == 1){
        NCurrentDoctorSchedules();
        return;
    }else if (output == 2){
        NAvailableDoctor();
    }else if(output == 3){
        NViewStationInventory();
    }
    else if(output == 4){
        NUpdateStationInventory();
    }
    else if (output == 5){
        NViewPatientReport();
    }
    else if (output == 6){
       NViewUnitReport();
    }
    else{
        main();
        return;
    }
}

int main(){
    NurseName= NurseLogin();
    clearTerminal();
    
    NurseMenue(NurseName);
}
