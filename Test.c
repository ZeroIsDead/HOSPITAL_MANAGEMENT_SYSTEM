#include "File_exe.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main()
{
    // char* input[] = {UserID, UserPW, Name, Tags};
    struct dataContainer2D master = getData("users");

    char* unique_key = "POTATO5";
    struct dataContainer1D data = queryKey("users", "POTATO6");


    data.data[3] = "WORKdEUWUWbabaUWW??";

    deleteKey("users", unique_key);


}