#include "File_exe.h"

int main() 
{   
    float x;
    x  = getFloat("Enter Float: ");

    if (x == '\0'){
        printf("x does not have any value");
    }
}
