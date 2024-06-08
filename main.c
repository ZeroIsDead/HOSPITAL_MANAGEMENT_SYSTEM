#include "File_exe.h"
char* NurseName;

///////////////////patient.c///////////////////////

//////////////////doctor.c/////////////////////////



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

        displaySystemMessage("Loading Main Nurse Menue....", 2);
        NurseMenue(NurseName);
        return;
    }
}

void NAvailableDoctor(){
    char* doctorName;
    struct dataContainer2D doctorSchedule;
    
    do{
        
        doctorName = getString("Enter the Doctor name: ");

        doctorSchedule = queryFieldStrict("doctorSchedule", "DoctorID", doctorName);
        if (doctorSchedule.error == 1){
            displaySystemMessage("Doctor does not exist in the database, please enter a valid name....",2);
        }
        else{
            break;
        }
    }
    while(1);
    
    struct dataContainer1D doctorWorkingDate = getFieldValues(doctorSchedule, "Date");
    
    char* date;
    struct dataContainer2D doctorTimeDay;
    
    do{
        displayOptions("Doctor Dates",doctorWorkingDate.data,doctorWorkingDate.x);
        date = getString("Please enter the date you are looking for: ");
        doctorTimeDay = filterDataContainer(doctorSchedule, "Date", date);

        if(doctorTimeDay.error == 1){
            displaySystemMessage("Either Doctor is not working on this day or Entered date is invalid...", 2);
        }
        else{
            break;
        }
    }
    while(1);

    struct  dataContainer2D bookedAppointements = queryFieldStrict("Appointments", "StaffUserID", doctorName);
    bookedAppointements = filterDataContainer(bookedAppointements, "Date", date);

    if(bookedAppointements.error == 1){

        displayTabulatedData(doctorTimeDay);
        freeMalloc2D(bookedAppointements);
        freeMalloc2D(doctorSchedule);
        freeMalloc1D(doctorWorkingDate);
        freeMalloc2D(bookedAppointements);
        NurseBack();
        NurseMenue(NurseName);
    
    }
    else{
        int count = 0;
        
        for (int i = 0; i < bookedAppointements.y; i++){
            for (int j = 1; j < 5; j++){
                if ( strcmp(bookedAppointements.data[i][4],doctorTimeDay.data[0][j]) == 0){
                    doctorTimeDay.data[0][j] = "0";
                    count++;
                }
            }
        }
        freeMalloc2D(bookedAppointements);
        freeMalloc2D(doctorSchedule);
        freeMalloc1D(doctorWorkingDate);
        
        printf("\n\n");
        
        if (count == 4){
            displaySystemMessage("All time slots books, Please pick another date....",5);
            freeMalloc2D(bookedAppointements);
            main();
            
        }
        else{
            clearTerminal();
            printf("Available Time on %s for Doctor %s\n", date, doctorName);
            displayTabulatedData(doctorTimeDay);
            NurseBack();
            NurseMenue(NurseName);
        }
    }
    
}

//View Current inventory
void NViewStationInventory(){
    clearTerminal();
    struct dataContainer2D currentInventory = getData("Inventory");
    displayTabulatedData(currentInventory);
    NurseBack();
    clearTerminal();
    freeMalloc2D(currentInventory);
    NurseMenue(NurseName);
}

void NurseAddNewIteam(){
    
    //Get New Medicine Name 
    char* newMed = getString("Enter the medicine name: ");
    
    //Check if newMed already exists in Inventory.txt
    struct dataContainer2D checkMed = queryFieldStrict("Inventory","Medicine Name", newMed);
    if (checkMed.error == 0){
        displaySystemMessage("Medicine already Exists, Please enter new Medicine", 2);
        freeMalloc2D(checkMed);
        NurseAddNewIteam();
        return;
    }

    //If newMed is new
    else{

        //Create new Unique ID
        char* newID = NurseInventoryId();
        
        //Get Price and check if entered value is a float
        float price;
        do {
            price = getFloat("Enter Price: ");
            if (price == -1.0){
                displaySystemMessage("Entered Value is not a Float",2);
            }
            else{
                break;
            }
        }
        while(1);

        //Convert float to string
        char strPrice[10];
        snprintf(strPrice, sizeof(strPrice), "%.6g", price);

        //Get Specification of the medicine
        char* specification = getString("Enter Specification: ");
        
        //Get quanity and check if entered value is an integer
        int quantity;
        do{
            quantity = getInt("Enter Quantity: ");
            if(quantity == -1){
                displaySystemMessage("Enterned value is not an integer please try again",2);
            }
            else{
                break;
            }
        }
        while(1);

        //Convert int to string
        char strQuantity[5];
        itoa(quantity, strQuantity, 10);
        
        //Create array of all entered elements in string form
        char* newIteam [] = {newID, newMed, strPrice, specification, strQuantity};

        //update inventory.txt
        int write = write_new_data("Inventory", 5, newIteam);
        
        //Check if update successful or not 
        if (write == 0){
            displaySystemMessage("New Iteams Updated Successfully",2);
        }
        else{
            displaySystemMessage("ERROR!!!",2);
        }
    }

}

//Function to Update Existing
void NUpdateExistingInventory(){
    
    //Getting medId from user
    char* medId = getString("Enter the medicineID you want to update: ");
    struct dataContainer1D iteam = queryKey("Inventory",medId);
    
    //Error handling is iteam medId is not found;
    if (iteam.error == 1){
        displaySystemMessage("Iteam not found",2);
        freeMalloc1D(iteam);
        NUpdateExistingInventory();
        return;
    }
    else{
        
        //Getting new quantity from user
        int newQuantity = getInt("Enter the new quantity of iteams: ");
        
        //Converting string to int
        char strNewQutity [5];
        itoa(newQuantity, strNewQutity, 10);
        
        //Updateing iteam with new Quantity
        iteam.data[4] = strNewQutity;
        updateData("Inventory", iteam.data);
        
        freeMalloc1D(iteam);
        
        //Returning to Nurse Main Menue
        displaySystemMessage("Returning you to main menue",2);
        NurseMenue(NurseName);
        return;
    }
}

void NUpdateStationInventory(){
    char* InventoryBanner = "Nurse Inventory Management";
    char* options[] = {"Enter New Iteam to Inventory", "Update a Current Inventory","Back"};
    int output = displayMenu(InventoryBanner,options,3);

    //Enter New iteam
    if (output == 1){
        NurseAddNewIteam();
        clearTerminal();
        NurseMenue(NurseName);
        return;
    }

    // Update existing iteam
    else if (output == 2){
        NUpdateExistingInventory();
    }

    //Back to Nurse Menue 
    else{
        clearTerminal();
        NurseMenue(NurseName);
        return;
    }
}

void NViewPatientReport(){
    
    char* appointmentNumber = getString("Enter Appointment Number: ");
    struct dataContainer1D appointmentData = queryKey("Appointments",appointmentNumber);
    
    
    while (appointmentData.error == 1){
        displaySystemMessage("Appointment Does not exist, Please try again.....",3);
        appointmentNumber = getString ("Enter Appointment Number: ");
        appointmentData = queryKey("Appointments",appointmentNumber);
    }
   
    struct dataContainer1D patientName = queryKey("Patient_IDs", appointmentData.data[2]);
    struct dataContainer2D patientReport = queryFieldStrict("Reports","ReportID",appointmentData.data[7]);
    clearTerminal();

    printf("Name of Patient:    %s\n",patientName.data[2]);
    printf("Age:                %s\n", patientName.data[3]);
    printf("Insurance Company:  %s\n\n",patientName.data[6]);

    displayTabulatedData(patientReport);
    
    freeMalloc1D(appointmentData);
    freeMalloc1D(patientName);
    freeMalloc2D(patientReport);
    
    NurseBack();
    clearTerminal();
    NurseMenue(NurseName);
    return;
}

int NPatientNumber(struct dataContainer2D array, int num1, int num2){
    
    int sum = 0; 
    for (int i = 0; i < array.y; i++){
        if (atoi(array.data[i][3]) >= num1 && atoi(array.data[i][3]) < num2){
            sum++;
        }
    }
    return sum;

}

void NViewUnitReport(){
    
    char* unitHeader = "Unit Report";
    char* unitOptions[] = {"Number of Patient in Ward","Number of Patient in ICU","Number of Patient in Emergency Room", "Back"};

    int output2 = displayMenu(unitHeader, unitOptions, 4);
    clearTerminal();

    struct dataContainer2D data = getData("Appointments");
    int patientNumber;
    if(output2 == 1){
        patientNumber = NPatientNumber(data, 100, 200);
        printf("There are %d Patient Admitted in Ward\n", patientNumber);
    }
    else if(output2 == 2){
        patientNumber = NPatientNumber(data, 200, 300);
        printf("There are %d Patient Admitted in ICU\n", patientNumber);
    }
    else if (output2 == 3){
        patientNumber = NPatientNumber(data, 300, 400);
        printf("There are %d Patient Admitted in Emergency Room\n", patientNumber);
    }
    else{
        freeMalloc2D(data);
        NurseMenue(NurseName);
        return;
    }
    freeMalloc2D(data);
    NurseBack();
    clearTerminal();
    NurseMenue(NurseName);
    return;

}

void NurseMenue(char* name){
    clearTerminal();
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
        StaffPortal();
        return;
    }
}

int NurseMain(){
    NurseName = NurseLogin();
    clearTerminal();
    
    NurseMenue(NurseName);
    return 0;
}


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
        return;
    }
    else if (output == 2){
        // Call DoctorMain
        printf("DOCTOR PORTAL\n");
        return;
    }
    else if (output == 3){
        NurseMain();
        clearTerminal;
        return;
    }
    else {
        clearTerminal();
        main();
        return;
    }
}

int main() 
{
    clearTerminal();
    char* header = "WELCOME TO THE HOSPITAL";
    char* options[] = {"Patient Login", "Staff Login", "Close System"};
    int output; 

    clearTerminal();
    output = displayMenu(header, options, 3);

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
        clearTerminal();
        StaffPortal();
        return 0;
    }
    else{
        displaySystemMessage("Thank You, Hope to see you soon!.....",5);
    }
}