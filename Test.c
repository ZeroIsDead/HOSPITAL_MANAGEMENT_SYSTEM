#include "File_exe.h"

void My_Reports(char* appointmentID) 
{
    char* CaseName;
    char* DiagnosticComments;
    
    printf("Report for appointment %s\n", appointmentID);
    CaseName = getString("Enter case name: ");
    DiagnosticComments = getString("Enter diagnostic comments: ");

    //cooking for UI
    char CaseHeader[256];
    char display_CaseName[256];
    char display_DiagnosticComments[256];


    sprintf(CaseHeader, "Report for appointment %s", appointmentID);
    sprintf(display_CaseName, "Case Name: %s", CaseName);
    sprintf(display_DiagnosticComments, "Diagnostic Comments: %s", DiagnosticComments);
    char* options[] = {display_CaseName, display_DiagnosticComments};

    display_data_horizontal(CaseHeader, options, 2);
    
}

void Write_New_Report()
{
    char* appointmentID;
    
    appointmentID = getString("Enter appointment ID for your report: ");

    struct dataContainer1D appointments = queryKey("Appointments", appointmentID);   

    if (appointments.data[7] == NULL)
    {
        My_Reports(appointmentID);
    }
    else
    {
        printf("Appointment %s already has a report", appointmentID);
    }
    
}

void My_reports_menu()
{
    clearTerminal();
    char* header = "My Reports";
    char* options[] = {"View My Reports", "Write New Report", "Back"};
    int noOptions = 3;

    int result = displayMenu(header, options, noOptions);

    if (result == 1) 
    {
        // View_My_Reports();
    } 
    else if (result == 2)
    {
        Write_New_Report();
    } 
    else if (result == 3) 
    {
        return;
    }
}

int main() 
{   
    My_reports_menu();
    return 0;
}