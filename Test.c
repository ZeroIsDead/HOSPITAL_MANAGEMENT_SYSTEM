#include "File_exe.h"

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

int main(){

    struct dataContainer2D d_Inventory = queryFieldStrict("Inventory", "MedicineID", "med001");

    clearTerminal();
    displayTabulatedData(d_Inventory);
    freeMalloc2D(d_Inventory);


}
