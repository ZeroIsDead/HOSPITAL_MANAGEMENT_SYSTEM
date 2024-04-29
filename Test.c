#include "File_exe.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main()
{   
    char UserID[MAX_LINE_LENGTH];
    
    printf("Enter UserID: ");
    scanf("%s", UserID);

    //CONTINUE PW CHECK
    
    // fgets(UserID, sizeof(UserID), stdin);
    // UserID[strcspn(UserID, "\n")] = '\0';

    struct dataContainer1D data = queryKey("users", UserID);

    for (int i = 0; i < data.x; i++)
    {
        int j = i + 1;
        printf("%d. %s\n",j, data.data[i]);
    }   

    // data.data[3] = "WORKdEUWUWbabaUWW??";

    // updateData("users", data.data);


}