#include "File_exe.h"


#define MAX_LINE_LENGTH 256
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

void NurseAddNewIteam(){
    
    char* newMed;

    newMed = getString("Enter the medicine name: ");
    struct dataContainer2D checkMed = queryFieldStrict("Inventory","Medicine Name", newMed);
    if (checkMed.error == 0){
        displaySystemMessage("Medicine already Exists, Please enter new Medicine", 2);
        freeMalloc2D(checkMed);
        NurseAddNewIteam();
        return;
    }
    else{

        char* newID = NurseInventoryId();
        char* price = getString("Enter Price: ");
        char* specification = getString("Enter Specification: ");
        int quantity;
        do{
            quantity = getInt("Enter Quantity: ");
            if(quantity == -1){
                displaySystemMessage("Enterned value is not an integer please try again",2);
            }else{
                break;
            }
        }
        while(1);
        
        char strQuantity[5];
        itoa(quantity, strQuantity, 10);
        
        char* newIteam [] = {newID, newMed, price, specification, strQuantity};

        for(int i = 0; i < 5; i++){
            printf("%s \n", newIteam[i]);
        }
    }

}
void NUpdateStationInventory(){
    char* InventoryBanner = "Nurse Inventory Management";
    char* options[] = {"Enter New Iteam to Inventory", "Update a Current Inventory"};
    int output = displayMenu(InventoryBanner,options,2);

    //Enter New iteam
    if (output == 1){
        NurseAddNewIteam();

    }

    // Update existing iteam
    else{
        printf("Update\n");
    }
}

int main() 
{   
   NUpdateStationInventory();
}