#include "File_exe.h"

int main(){

    struct dataContainer2D d_Inventory = queryFieldStrict("Inventory", "MedicineID", "med001");

    clearTerminal();
    displayTabulatedData(d_Inventory);
    freeMalloc2D(d_Inventory);


}
