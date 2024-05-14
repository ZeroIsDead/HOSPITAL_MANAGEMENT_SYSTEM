#include "File_exe.h"


char* message(char name []){ 
    
    int welcomeSize = 8 + strlen(name) + 1;
    char welcome[welcomeSize];
    
    strcpy(welcome, "Welcome ");
    strcat(welcome, name);
    
    welcome[welcomeSize-1] = '\0';

    return strdup(welcome);
    
}

void DoctorSchedule(){
    printf("This is the Doctor Schedule Menue");
}

void Inventory(){
    printf("This is the Inventory Menue");
}

void Report(){
    printf("This is the Report Menue");
}

void LoginUserName(){
    char userName[50];
    char password[50];
    printf("Please Enter Your Username: ");
    gets(userName);

    struct dataContainer1D userNameCheck = queryKey("Staff_IDs", userName);
   
    if (userNameCheck.error == 1){
        displaySystemMessage("User Does not exist, Please try again", 2);
        LoginUserName();
        return;
    }else if (strcmp(userNameCheck.data[3],"Nurse")){
        displaySystemMessage("User is not a Nurse, Please try again", 2);
        LoginUserName();
        return;
    }else {
        
    }
    
}

int main(){
    Login();
    char name []= "mugdho";
    char* header = message(name);
    char* options[] = {"Doctor Schedules","Inventory","Report","Log Out"};

    int output = displayMenu(header, options, 3);
    
    if (output == 1){
        DoctorSchedule();
    }else if (output == 2){
        Inventory();
    }else if(output == 3){
        Report();
    }
}
