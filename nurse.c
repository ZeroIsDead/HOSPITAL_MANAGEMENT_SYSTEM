#include "File_exe.h"

char* NurseName;

void NurseBack(){

    int back = 1;
    back = getInt("\nPlease Enter 0 to go back: ");

    if (back != 0 ){

        displaySystemMessage("Please enter the corrent input!: ",2);
        
        NurseBack();
        return;
    }
    else{
     
        return;
    
    }
}


char* NurseInventoryId(){
    struct dataContainer2D dataN = getData("Inventory");
    char* inventoryID = dataN.data[dataN.y-1][0];
    char number[3];
    
    //Separating the string "021" from "med021"
    for (int i = 0; i < 3; i++){
        number[i] = inventoryID[i+3];
    }
    
    //string to int and adding 1 to it 
    int num = atoi(number) + 1;
    char newNum[3];
    char newId[7];

    //Creating new Inventory ID
    if (num < 99){
        strcpy(newId, "med0");
        strcat(newId, itoa(num, newNum, 10));
    }
    else{
        strcpy(newId, "med");
        strcat(newId, itoa(num, newNum, 10));    
    }

    freeMalloc2D(dataN);
    return strdup(newId);
}

// Displaying Welcome banner to the specific nurse name
char* NurseWelcomeMessage(char name []){ 
    
    int welcomeSize = 8 + strlen(name) + 1;
    char welcome[welcomeSize];
    
    strcpy(welcome, "Welcome ");
    strcat(welcome, name);
    
    welcome[welcomeSize-1] = '\0';

    return strdup(welcome);
    
}


// Nurse Login Function
char* NurseLogin(){
    
    displaySystemMessage("Opening Nurse Log in Portal",2);
    
    //get userName and concrent data
    char* userName = getString("Enter Your UserName: ");
    struct dataContainer1D userNameCheck = queryKey("Staff_IDs", userName);
    
    //Establishing end poin of string
    if (userNameCheck.error == 0){
        userNameCheck.data[3][5] = '\0';
    }

    //validation of user name
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

    //get password
    char* password = getString("Enter Your Password: ");

    //validation of password
    while(strcmp(userNameCheck.data[1],password)){
        displaySystemMessage("Wrong Password, Please try agian",3);
        password = getString("Entrer Your Password: ");    
    }       

    //
    return strdup(userNameCheck.data[2]);
    
}

void NCurrentDoctorSchedules(){
    
    // Display + input of Doctor Schedule Menu
    char* option[] = {"All Doctor Schedule", "Specific Doctro Schedule", "Back"};
    int output = displayMenu("Doctor Schedule", option ,3);
    clearTerminal();
    
    //Display All Doctor Schedule
    if (output == 1){
        clearTerminal();
        
        struct dataContainer2D allDocSchedule = queryFieldStrict("Staff_IDs","Tags","Doctor");
        displayTabulatedData(allDocSchedule);
        
        printf("\n");
        
        NurseBack();
        clearTerminal();
        NCurrentDoctorSchedules();
        freeMalloc2D(allDocSchedule);
        return;
    }

    // Searching and Displaying Specific Doctor Schedule
    else if (output == 2){
        do{
            char* docName = getString("Please enter the name of the doc: ");
            struct dataContainer2D specificDocSche = queryFieldStrict("doctorSchedule","DoctorID",docName);
            if(specificDocSche.error == 1){
                displaySystemMessage("Wrong input, Please enter a doctor name: ",2);
            }
            else{
                displayTabulatedData(specificDocSche);
                freeMalloc2D(specificDocSche);
                break;
            }
        }
        while(1);
        
        NurseBack();
        clearTerminal();
        NCurrentDoctorSchedules();
        return;
    }

    // Returning to the Nurse Menu
    else{

        displaySystemMessage("Loading Current Doctor Schedule Menue....", 2);
        NurseMenue(NurseName);
        return;
    }
}


void NAvailableDoctor(){
    printf("Available Doctor\n");
}

//View Current inventory
void NViewStationInventory(){
    struct dataContainer2D currentInventory = getData("Inventory");
    displayTabulatedData(currentInventory);
    NurseBack();
    clearTerminal();
    freeMalloc2D(currentInventory);
    NurseMenue(NurseName);
}

void NUpdateStationInventory(){
    char* nInventoryBanner = "Nurse Inventory Management";
    char* options = {"Enter New Iteam to Inventory", "Update a Current Inventory"};
    int output = displayMenu(nInventoryBanner,options,2);

    //Enter New iteam
    if (output == 1){
        char* newMed;
        do{
            newMed = getString("Enter the medicine name: ");
            struct dataContainer1D checkMed = queryKey("Inventory","Medicine Name");
            if (checkMed.error == 1){
                freeMalloc1D(checkMed);
                displaySystemMessage("Medicine already Exists, Please enter new Medicine", 2);
            }
            else{
                freeMalloc1D(checkMed);
                break;
            }
        }while(1);

        char* newId = NurseInventoryId();
    }

    // Update existing iteam
    else{
        printf("Update\n");
    }
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
