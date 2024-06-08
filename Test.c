#include "File_exe.h"

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
    
    printf("Name of Patient:    %s\n",patientName.data[2]);
    printf("Age:                %s\n", patientName.data[3]);
    printf("Insurance Company:  %s\n\n",patientName.data[6]);

    displayTabulatedData(patientReport);
}

int main() 
{   
    NViewPatientReport();
}
