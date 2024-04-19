
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256


FILE* filecheck(const char* filename, const char* mode)
{
    FILE* fptr = fopen(filename, mode);
    if(fptr == NULL)
    {
        printf("Error opening file %s!\n", filename);
    }
    return fptr;
}

void append_file(const char* filename, int numInputs, const char* inputs[])
{
     /*Things to add in this Function:
    - semicolon check
    - Validate numInputs with field numbers
     */

    FILE* fptr = filecheck(filename, "a");
   
    // Calculate the total length needed for the input string
    int total_len = 0;
    for (int i = 0; i < numInputs; ++i) 
    {
        total_len += snprintf(NULL, 0, "%s;", inputs[i]);
    }
    total_len += 1; // Add 1 for the newline character

    char* line = malloc(total_len); // Allocate memory for the line
    
    // Check for null pointer
    if (line == NULL) 
    { 
        printf("Error allocating memory!\n");
        fclose(fptr);
        return;
    }

    // Construct the input string
    line[0] = '\0';
    for (int i = 0; i < numInputs; ++i) 
    {
        strcat(line, inputs[i]); 
        strcat(line, ";");
    }
    strcat(line, "\n"); // Add a newline character at the end

    // Write the input string to the file
    fputs(line, fptr);

    // Cleanup
    free(line);
    fclose(fptr);
}
 
// int main()
// {
//     char UserID[MAX_LINE_LENGTH];
//     char UserPW[MAX_LINE_LENGTH];
//     char Name[MAX_LINE_LENGTH];
//     char Tags[MAX_LINE_LENGTH];

//     printf("Enter UserID: ");
//     fgets(UserID, MAX_LINE_LENGTH, stdin);
//     UserID[strcspn(UserID, "\n")] = '\0';

//     printf("Enter UserPW: ");
//     fgets(UserPW, MAX_LINE_LENGTH, stdin);
//     UserPW[strcspn(UserPW, "\n")] = '\0';

//     printf("Enter Name: ");
//     fgets(Name, MAX_LINE_LENGTH, stdin);
//     Name[strcspn(Name, "\n")] = '\0';

//     printf("Enter Tags: "); 
//     fgets(Tags, MAX_LINE_LENGTH, stdin);
//     Tags[strcspn(Tags, "\n")] = '\0';

//     const char* input[] = {UserID, UserPW, Name, Tags};

//     append_file("test.txt", 4, input);

// }
