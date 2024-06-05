#include "File_exe.h"


#define MAX_LINE_LENGTH 256

int main() 
{   
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
        freeMalloc2D(bookedAppointements);
    }

    
}