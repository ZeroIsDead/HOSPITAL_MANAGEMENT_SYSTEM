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

        int write = write_new_data("Inventory", 5, newIteam);
        if (write == 0){
            printf("\n\nYou deserve a break!!!");
        }
        else{
            printf("\n\nClose but not yet :)");
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