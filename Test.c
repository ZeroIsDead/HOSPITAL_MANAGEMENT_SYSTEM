#include "File_exe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main()
{
    char UserID[MAX_LINE_LENGTH];
    char UserPW[MAX_LINE_LENGTH];
    char Name[MAX_LINE_LENGTH];
    char Tags[MAX_LINE_LENGTH];

    printf("Enter UserID: ");
    fgets(UserID, MAX_LINE_LENGTH, stdin);
    UserID[strcspn(UserID, "\n")] = '\0';

    printf("Enter UserPW: ");
    fgets(UserPW, MAX_LINE_LENGTH, stdin);
    UserPW[strcspn(UserPW, "\n")] = '\0';

    printf("Enter Name: ");
    fgets(Name, MAX_LINE_LENGTH, stdin);
    Name[strcspn(Name, "\n")] = '\0';

    printf("Enter Tags: "); 
    fgets(Tags, MAX_LINE_LENGTH, stdin);
    Tags[strcspn(Tags, "\n")] = '\0';

    const char* input[] = {UserID, UserPW, Name, Tags};

    append_file("Users.txt", 4, input);

}