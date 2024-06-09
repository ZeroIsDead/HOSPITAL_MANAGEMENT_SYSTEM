#include "File_exe.h"
#include <time.h>
#include <assert.h>
#include <string.h>

void AdminMenu();
void AdminRegistermenu();
///////// Utility //////////
int compareDates(const char *date1, const char *date2) {
    // Variables to hold the parsed date components
    int year1, month1, day1;
    int year2, month2, day2;
    
    // Parse the date1 string
    sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
    
    // Parse the date2 string
    sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);
    
    // Compare the years
    if (year1 != year2) {
        return year1 - year2;
    }
    
    // Compare the months if years are equal
    if (month1 != month2) {
        return month1 - month2;
    }
    
    // Compare the days if years and months are equal
    return day1 - day2;
}

int* Appointment_trend(struct dataContainer2D data)
{   
    char* date;
    char* date_int[3];
    int count=0;
    int d, m, y;
    int* amount_per_day = malloc(7*sizeof(int));

    for (int i=0; i<7;i++)
    {
        amount_per_day[i]=0;
    }

    int i = 0;
    while (i < data.y)
    {   
        count=0;
        date = strdup(data.data[i][5]);

        char *token = strtok(date, "-");
        while (token != NULL)
        {   
            date_int[count++] = token;
            token = strtok(NULL, "-");
        }

        d = atoi(date_int[2]);
        m = atoi(date_int[1]);
        y = atoi(date_int[0]);

        int weekday  = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;

        amount_per_day[weekday] = amount_per_day[weekday] + 1;
    
        i=i+1;
    }   
    return amount_per_day;
}

struct dataContainer2D filteringData(struct dataContainer2D data) 
{
    char* currentDate = "2024-06-03";
    int count = 0;

    struct dataContainer2D filteredData;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) >= 0) 
        {
            count++;
        }
    }

    char*** relaying_array = malloc(count*sizeof(char**));
    int count2  = 0;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) >= 0) 
        {
            relaying_array[count2] = data.data[i];
            count2++;
        }
    }

    filteredData.data = relaying_array;
    filteredData.y = count2;
    filteredData.x = data.x;
    filteredData.error = 0;
    filteredData.fields = data.fields;

    return filteredData;
} 

struct dataContainer2D filteringData2(struct dataContainer2D data) 
{
    char* currentDate = "2024-06-03";
    int count = 0;

    struct dataContainer2D filteredData;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) < 0) 
        {
            count++;
        }
    }

    char*** relaying_array = malloc(count*sizeof(char**));
    int count2  = 0;
    for (int i = 0; i < data.y; i++) {
        if (compareDates(data.data[i][5], currentDate) < 0) 
        {
            relaying_array[count2] = data.data[i];
            count2++;
        }
    }

    filteredData.data = relaying_array;
    filteredData.y = count2;
    filteredData.x = data.x;
    filteredData.error = 0;
    filteredData.fields = data.fields;
    
    return filteredData;
}

char* itoa(int num, char* buffer, int base)   
{  
int current = 0;  
if (num == 0) {  
buffer[current++] = '0';  
buffer[current] = '\0';  
return buffer;  
}  
int num_digits = 0;  
if (num < 0) {  
if (base == 10) {  
num_digits ++;  
buffer[current] = '-';  
current ++;  
num *= -1;  
}  
else  
return NULL;  
}  
num_digits += (int)floor(log(num) / log(base)) + 1;  
while (current < num_digits)   
{  
int base_val = (int) pow(base, num_digits-1-current);  
int num_val = num / base_val;  
 char value = num_val + '0';  
buffer[current] = value;  
current ++;  
num -= base_val * num_val;  
}  
buffer[current] = '\0';  
return buffer;  
}   

char* int_to_string(int number)
{   
    char buffer[256]; 
    char* string_return; 

    itoa(number,buffer,10);
    string_return = buffer;

    return string_return;
}
///////// Admin Login //////////
int AdminLogin(){
    
    displaySystemMessage("Opening Admin Log in Portal",2);
    
    //get userName and concrent data
    char* userName = getString("Enter Your User ID: ");
    struct dataContainer1D userNameCheck = queryKey("Staff_IDs", userName);
    
    //Establishing end poin of string
    if (userNameCheck.error == 0){
        userNameCheck.data[3][5] = '\0';
    }

    //validation of user name
    while (userNameCheck.error == 1 || strcmp(userNameCheck.data[3],"Admin")){
        if (userNameCheck.error == 1)
        {
            displaySystemMessage("Invalid User ID",3);
        }else{
            displaySystemMessage("User is not an Admin",3);
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
        password = getString("Enter Your Password: ");    
    }       

    return 1;
}
///////// Patient Menu //////////
void PatientRegistration();

void displaycurrentpatient() 
{
    struct dataContainer2D d_patient = getData("Patient_IDs");
        
    displayTabulatedData(d_patient);
    freeMalloc2D(d_patient);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if( strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }    
}

void RegisterNewPatient()
{   
    clearTerminal();
    char* PatientUserID = getString("Enter Patient User ID: ");

    struct dataContainer2D d_patient = queryFieldStrict("Patient_IDs","PatientUserID",PatientUserID);
    if(d_patient.error==0)
    {   
        clearTerminal();
        displaySystemMessage("Patient User ID is already existed!",2);
        displaySystemMessage("Please enter a different one",2);
        freeMalloc2D(d_patient);
        RegisterNewPatient();
    }
    
    freeMalloc2D(d_patient);

    char* UserPW = getString("Enter Patient User Password: ");
    char* Name = getString("Enter Patient Name: ");
    char* Age = getString("Enter Patient Age: ");
    char* Ward = getString("Enter Patient Ward: ");
    char* InsuranceCompany = getString("Enter Patient Insurance Company: ");
    char* InsuranceID = getString("Enter Patient Insurance ID: ");

    char** PatientRinputs = malloc (7 * sizeof(char*));

    PatientRinputs[0]=PatientUserID;
    PatientRinputs[1]=UserPW;
    PatientRinputs[2]=Name;
    PatientRinputs[3]=Age;
    PatientRinputs[4]=Ward;
    PatientRinputs[5]=InsuranceCompany;
    PatientRinputs[6]=InsuranceID;

    char* ConfirmationMessage = getString("Are you sure you want to Register this Patient? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        write_new_data("Patient_IDs", 7, PatientRinputs);
        clearTerminal();
        displaySystemMessage("Registration Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
}

void DeletePatient()
{   
    clearTerminal();

    struct dataContainer2D d_patient = getData("Patient_IDs");  
    displayTabulatedData(d_patient);
    freeMalloc2D(d_patient);

    char* PatientUserID = getString("Enter Patient User ID: ");

    struct dataContainer2D d_Patient = queryFieldStrict("Patient_IDs", "PatientUserID",PatientUserID);

    if(d_Patient.error == 1)
    {   
        clearTerminal();
        displaySystemMessage("Patient does not exist!",2);
        freeMalloc2D(d_Patient);
        DeletePatient();
    }
    else
    {   
        clearTerminal();
        displayTabulatedData(d_Patient);
        freeMalloc2D(d_Patient);
    }

    char* ConfirmationMessage = getString("Are you sure you want to delete this Patient? (Y/N): ");
    
    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        deleteKey("Patient_IDs", PatientUserID);
        clearTerminal();
        displaySystemMessage("Deletion Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to the menu...", 2);
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
}

void PatientRegistration()
{
    char* patientheader = "Patient Registration Menu";
    char* patientoptions[] = {"View Current Patient","Register New Patient","Delete Patient","Return To Menu"};

    clearTerminal();
    int patientoutput = displayMenu(patientheader, patientoptions, 4);

    if(patientoutput==1)
    {
        clearTerminal();
        displaycurrentpatient();
    }
    else if(patientoutput==2)
    {
        clearTerminal();
        RegisterNewPatient();
    }
    else if(patientoutput==3)
    {
        clearTerminal();
        DeletePatient();
    }
    else if(patientoutput==4)
    {
        clearTerminal();
        AdminRegistermenu();
        return;
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return;
    }
}
///////// Doctor Menu //////////
void DoctorRegistration();

void displaycurrentdoctor() 
{
    struct dataContainer2D d_doctor = queryFieldStrict("Staff_IDs","Tags","doctor");
        
    displayTabulatedData(d_doctor);
    freeMalloc2D(d_doctor);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        DoctorRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
}

void RegisterNewDoctor()
{   
    clearTerminal();
    char* StaffUserID = getString("Enter Doctor User ID: ");

    struct dataContainer2D d_Doctor = queryFieldStrict("Staff_IDs","StaffUserID",StaffUserID);
    if(d_Doctor.error==0)
    {   
        clearTerminal();
        displaySystemMessage("Doctor User ID is already existed!",2);
        displaySystemMessage("Please enter a different one",2);
        freeMalloc2D(d_Doctor);
        RegisterNewDoctor();
    }

    freeMalloc2D(d_Doctor);

    char* UserPW = getString("Enter Doctor User Password: ");
    char* Name = getString("Enter Doctor Name: ");

    char** DoctorRinputs = malloc (4 * sizeof(char*));

    DoctorRinputs[0]=StaffUserID;
    DoctorRinputs[1]=UserPW;
    DoctorRinputs[2]=Name;
    DoctorRinputs[3]="Doctor";

    char* ConfirmationMessage = getString("Are you sure you want to Register this Doctor? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        write_new_data("Staff_IDs", 4, DoctorRinputs);
        clearTerminal();
        displaySystemMessage("Registration Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        DoctorRegistration();
        return;
    }
}

void DeleteDoctor()
{
    clearTerminal();

    struct dataContainer2D d_doctor = queryFieldStrict("Staff_IDs","Tags","doctor");
        
    displayTabulatedData(d_doctor);
    freeMalloc2D(d_doctor);

    char* StaffUserID = getString("Enter Doctor User ID: ");

    struct dataContainer2D d_Doctor = queryFieldStrict("Staff_IDs","StaffUserID",StaffUserID);
    if(d_Doctor.error==1)
    {   
        clearTerminal();
        displaySystemMessage("Doctor User ID does not exist!",2);
        freeMalloc2D(d_Doctor);
        DeleteDoctor();
    }
    else
    {   
        clearTerminal();
        displayTabulatedData(d_Doctor);
        freeMalloc2D(d_Doctor);
    }

    char* ConfirmationMessage = getString("Are you sure you want to delete this Doctor? (Y/N): ");
    
    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        deleteKey("Staff_IDs", StaffUserID);
        clearTerminal();
        displaySystemMessage("Deletion Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to the menu...", 2);
        PatientRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        PatientRegistration();
        return;
    }
}

void DoctorRegistration()
{
    char* Doctorheader = "Doctor Registration Menu";
    char* DoctorOptions[] = {"View Current Doctor","Register New Doctor","Delete Doctor","Return To Menu"};

    clearTerminal();
    int Doctoroutput = displayMenu(Doctorheader, DoctorOptions, 4);

    if(Doctoroutput==1)
    {
        clearTerminal();
        displaycurrentdoctor();
    }
    else if(Doctoroutput==2)
    {
        clearTerminal();
        RegisterNewDoctor();
    }
    else if(Doctoroutput==3)
    {
        clearTerminal();
        DeleteDoctor();
    }
    else if(Doctoroutput==4)
    {
        clearTerminal();
        AdminRegistermenu();
        return;
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return; 
    }
}
///////// Nurse Menu //////////
void NurseRegistration();

void displaycurrentnurse() 
{
    struct dataContainer2D d_nurse = queryFieldStrict("Staff_IDs","Tags","Nurse");
        
    displayTabulatedData(d_nurse);
    freeMalloc2D(d_nurse);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaycurrentnurse();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
}

void RegisterNewNurse()
{
    clearTerminal();
    char* StaffUserID = getString("Enter Nurse User ID: ");

    struct dataContainer2D d_Nurse = queryFieldStrict("Staff_IDs","StaffUserID",StaffUserID);
    if(d_Nurse.error==0)
    {   
        clearTerminal();
        displaySystemMessage("Nurse User ID is already existed!",2);
        displaySystemMessage("Please enter a different one",2);
        freeMalloc2D(d_Nurse);
        RegisterNewDoctor();
    }
    
    freeMalloc2D(d_Nurse);

    char* UserPW = getString("Enter Nurse User Password: ");
    char* Name = getString("Enter Nurse Name: ");

    char** NurseRinputs = malloc (4 * sizeof(char*));

    NurseRinputs[0]=StaffUserID;
    NurseRinputs[1]=UserPW;
    NurseRinputs[2]=Name;
    NurseRinputs[3]="Nurse";;

    char* ConfirmationMessage = getString("Are you sure you want to Register this Nurse? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        write_new_data("Staff_IDs", 7, NurseRinputs);
        clearTerminal();
        displaySystemMessage("Registration Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
}

void DeleteNurse()
{
    clearTerminal();

    struct dataContainer2D d_Nurse = queryFieldStrict("Staff_IDs","Tags","Nurse");
        
    displayTabulatedData(d_Nurse);
    freeMalloc2D(d_Nurse);

    char* StaffUserID = getString("Enter Nurse User ID: ");

    struct dataContainer2D d_nurse = queryFieldStrict("Patient_IDs", "StaffUserID",StaffUserID);

    if(d_nurse.error==1)
    {   
        clearTerminal();
        displaySystemMessage("Nurse User ID does not exist!",2);
        freeMalloc2D(d_nurse);
        DeleteNurse();
    }
    else
    {   
        clearTerminal();
        displayTabulatedData(d_nurse);
        freeMalloc2D(d_nurse);
    }    

    displayTabulatedData(d_nurse);
    freeMalloc2D(d_nurse);

    char* ConfirmationMessage = getString("Are you sure you want to delete this Nurse? (Y/N): ");
    
    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        deleteKey("Staff_IDs", StaffUserID);
        clearTerminal();
        displaySystemMessage("Deletion Complete!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        displaySystemMessage("Returning back to the menu...", 2);
        NurseRegistration();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        NurseRegistration();
        return;
    }
}

void NurseRegistration()
{
    char* Nurseheader = "Nurse Registration Menu";
    char* NurseOptions[] = {"View Current Nurse","Register New Nurse","Delete Nurse","Return to Menu"};

    clearTerminal();
    int Nurseoutput = displayMenu(Nurseheader, NurseOptions, 4);

    if(Nurseoutput==1)
    {
        clearTerminal();
        displaycurrentnurse();
    }
    else if(Nurseoutput==2)
    {
        clearTerminal();
        RegisterNewNurse();
    }
    else if(Nurseoutput==3)
    {
        clearTerminal();
        DeleteNurse();
    }
    else if(Nurseoutput==4)
    {
        clearTerminal();
        AdminRegistermenu();
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return; 
    }
}
///////// Appointment Menu //////////
void AdminAppointmentMenu();

void ActiveAppointment()
{   
    struct dataContainer2D d_Appointment = getData("Appointments");
    struct dataContainer2D filteredData = filteringData(d_Appointment);


    displayTabulatedData(filteredData);
    freeMalloc2D(filteredData);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
}

void PastAppointment()
{   
    struct dataContainer2D d_Appointment = getData("Appointments");
    struct dataContainer2D filteredData = filteringData2(d_Appointment);

        
    displayTabulatedData(filteredData);
    freeMalloc2D(filteredData);

        char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
}

void AppointmentTrend()
{   
    struct dataContainer2D d_Appointment = getData("Appointments");

    int* amount_per_day = Appointment_trend(d_Appointment);

    char* Trend_header = "Appointment Trend";
        
    struct dataContainer2D data;

    data.error = 0;
    data.y = 7;
    data.x = 2;

    char* days[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    data.data = malloc (data.y * sizeof(char**));

    for (int i=0; i<7; i++) {
    data.data[i] = malloc(data.x * sizeof(char*));
    data.data[i][0] = days[i];
    data.data[i][1] = strdup(int_to_string(amount_per_day[i]));
    }

    data.fields = malloc(data.x * sizeof(char*));
    data.fields[0] = "Day";
    data.fields[1] = "Data";
    
    clearTerminal();
    displayTabulatedData(data);

    freeMalloc2D(data);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return;
    }
    
}

void AdminAppointmentMenu() 
{
    char* Appointmentheader = "Appointment Menu";
    char* Appointmentoptions[] = {"View Active Appointment List", "View Past Appointment List", "View Appointment Trend","Return to Menu"};
        
        clearTerminal();
        int Appointmentoutput = displayMenu(Appointmentheader, Appointmentoptions, 4);

    if(Appointmentoutput==1)
    {
        clearTerminal();
        ActiveAppointment();
    }
    else if(Appointmentoutput==2)
    {
        clearTerminal();
        PastAppointment();
    }
    else if(Appointmentoutput==3)
    {
        clearTerminal();
        AppointmentTrend();
    }
    else if(Appointmentoutput==4)
    {
        clearTerminal();
        AdminMenu();
        return;
    }
     else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminAppointmentMenu();
        return; 
    }      
}
///////// Inventory Menu //////////
void AdminInventoryMenu();

void displayInventory()
{
    struct dataContainer2D d_inventory = getData("Inventory");

    clearTerminal();    
    displayTabulatedData(d_inventory);
    freeMalloc2D(d_inventory);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminInventoryMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
}

void SearchInventory()
{   
    clearTerminal();
    char* MedID = getString("Enter Medicine ID: ");

    struct dataContainer2D d_Inventory = queryFieldStrict("Inventory", "MedicineID", MedID);

    clearTerminal();
    displayTabulatedData(d_Inventory);
    freeMalloc2D(d_Inventory);

    char* ConfirmationMessage = getString("Return back to the menu? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {
        clearTerminal();
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        AdminInventoryMenu();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    } 
}

void RestockInventory()
{   
    clearTerminal();
    struct dataContainer2D d_inventory = getData("Inventory");

    clearTerminal();    
    displayTabulatedData(d_inventory);
    freeMalloc2D(d_inventory);

    char* MedID = getString("Enter Medicine ID to be restock: ");

    struct dataContainer2D d_Inventory = queryFieldStrict("Inventory", "MedicineID", MedID);
    
    clearTerminal();
    displayTabulatedData(d_Inventory);

    char* ConfirmationMessage = getString("Are you sure you want to restock this Medicine? (Y/N): ");

    if(strncmp(ConfirmationMessage,"Y", 1) == 0)
    {   
        clearTerminal();
        int RestockValue = getInt("Enter number of restock: ");
        d_Inventory.data[0][4] = int_to_string(atoi(d_Inventory.data[0][4]) + RestockValue);
        updateData("Inventory", d_Inventory.data[0]);
        freeMalloc2D(d_Inventory);
        displaySystemMessage("Restock Complete !", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
    else if (strncmp(ConfirmationMessage,"N", 1) == 0)
    {   
        clearTerminal();
        freeMalloc2D(d_Inventory);
        RestockInventory();
        return;
    }
    else
    {   
        clearTerminal();
        freeMalloc2D(d_Inventory);
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return;
    }
}

void AdminInventoryMenu()
{
    char* Inventoryheader = "Inventory Menu";
    char* Inventoryoptions[] = {"View Inventory", "Search Inventory", "Restock Inventory","Return to Menu"};
        
    clearTerminal();
    int inventoryoutput = displayMenu(Inventoryheader, Inventoryoptions, 4);

    if(inventoryoutput==1)
    {
        clearTerminal();
        displayInventory();
        return;
    }
    else if(inventoryoutput==2)
    {
        clearTerminal();
        SearchInventory();
        return;
    }
    else if(inventoryoutput==3)
    {
        clearTerminal();
        RestockInventory();
        return;
    }
    else if(inventoryoutput==4)
    {
        clearTerminal();
        AdminMenu();
        return;
    }
     else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminInventoryMenu();
        return; 
    }      
}
///////// Registration Menu //////////
void AdminRegistermenu()
{
    char* registrationheader = "Registration Menu";
        char* registrationoptions[] = {"Patient Registration","Doctor Registration","Nurse Registration","Return to Menu"};

        clearTerminal();
        int RegistrationOutput = displayMenu(registrationheader, registrationoptions, 4);

    if(RegistrationOutput==1)
    {
        clearTerminal();
        PatientRegistration();
    }
    else if(RegistrationOutput==2)
    {
        clearTerminal();
        DoctorRegistration();
    }
    else if(RegistrationOutput==3)
    {
        clearTerminal();
        NurseRegistration();
    }
    else if(RegistrationOutput==4)
    {
        clearTerminal();
        AdminMenu();
        return;
    }
    else
    {
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminRegistermenu();
        return; 
    }
}
///////// Admin Menu //////////
void AdminMenu()
{
    char* header = "Welcome to the Hospital Admin Management";
    char* option[] = {"Registration", "Appointment", "Inventory","Log out"};
    int output;
    
    clearTerminal();
    output = displayMenu(header, option, 4);

    if (output==1)
    {
        clearTerminal();
        AdminRegistermenu();
        return;
    }

    else if (output==2)
    {
        clearTerminal();
        AdminAppointmentMenu();
        return;
    }

    else if (output==3)
    {
        clearTerminal();
        AdminInventoryMenu();
        return;
    }
    else if (output==4)
    {
        clearTerminal();
        displaySystemMessage("Logging out", 1);
        displaySystemMessage("Logging out.", 1);
        displaySystemMessage("Logging out..", 1);
        displaySystemMessage("Logging out...", 1);
        main();
        return;
    }
    else
    {   
        clearTerminal();
        displaySystemMessage("Please type in the correct Input!", 2);
        displaySystemMessage("Returning back to menu...", 2);
        AdminMenu();
        return; 
    }
}
///////// MAIN //////////
int adminMain() 
{
    int check = AdminLogin();

    if(check==1)
    {
        AdminMenu();
    }
    else
    {
        displaySystemMessage("No access for you", 2);
    }
}