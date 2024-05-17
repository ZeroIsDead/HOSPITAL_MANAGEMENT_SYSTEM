#include "File_exe.h"


#define MAX_LINE_LENGTH 256
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
   




int main(){
    char* docName = getString("Please enter the Name of the Doctor: ");
    struct dataContainer1D docSchedule = queryKey("Staff_IDs", docName);
        
    while (docSchedule.error == 1){
        displaySystemMessage("Error!!",2);
        docName = getString("Please enter the Name of the Doctor: ");
        docSchedule = queryKey("Staff_IDs", docName);
    }
    docSchedule.data[3][6] = '\0';
    for (int i = 0; i < docSchedule.x; i++){
        printf("%s ", docSchedule.data[i]);
    }
    NurseBack();
}