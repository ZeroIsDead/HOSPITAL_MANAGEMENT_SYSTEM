#include "File_exe.h"

void View_My_Reports()
{
    
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