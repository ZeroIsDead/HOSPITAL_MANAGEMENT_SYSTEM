#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* filecheck(const char* filename, const char* mode)
{
    FILE* fptr = fopen(filename, mode);
    if(fptr == NULL)
    {
        printf("Error opening file %s!\n", filename);
    }
    return fptr;
}

int AskForInt(char prompt[]) {
    int number;
    printf(prompt);
    scanf("%d", &number);

    return number;
}

// fopen() with attributes as “a” or “a+” or “w” or “w+”
// Opening an existing file – fopen()
// Reading from file – fscanf() or fgets()
// Writing to a file – fprintf() or fputs()
// Moving to a specific location in a file – fseek(), rewind()
// Closing a file – fclose()

struct dataContainer {
    // char** fields;
    char*** data;
    int y; //y
    int x; //x
};

void freeMalloc(struct dataContainer pointer) {
    for (int i = 0; i < pointer.y; i++)
        free(pointer.data[i]);
 
    free(pointer.data);
}

int getFileNumOfLines(char file[]) {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength]; /* not ISO 90 compatible */

    int count = 0;

    filePointer = fopen(file, "r");

    for (char c = getc(filePointer); c != EOF; c = getc(filePointer)){
        if (c == '\n') {// Increment count if this character is newline
            count = count + 1;
        }
    }
        
    fclose(filePointer);

    return count;
}

int getFileNumberOfColumns(char file[]) {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength]; /* not ISO 90 compatible */

    const char* separator = ";";

    int count = 0;

    filePointer = fopen(file, "r");

    fgets(buffer, bufferLength, filePointer);
    char* token = strtok(buffer, separator);

    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) {
        token = strtok(NULL, separator);
        count++;
    }

    fclose(filePointer);

    return count;
}

struct dataContainer getData(const char* file) {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength]; 

    const char* separator = ";";


    // Concatenate FilePath
    const char* folder = "./data/";
    const char* extension = ".txt";

    char* filePath;
    filePath = malloc(strlen(extension)+strlen(folder)+strlen(file)+1);
    strcpy(filePath, folder); 
    strcat(filePath, file); 
    strcat(filePath, extension); 

    // Get Dimensions of the Array
    int fileLength = getFileNumOfLines(filePath); // y
    int columnLength = getFileNumberOfColumns(filePath); // x


    char*** data = (char***) malloc (fileLength * sizeof(char**)); 

    for (int i; i < fileLength; i++) {
        data[i] = (char**) malloc (columnLength * sizeof(char*));
    }

    filePointer = fopen(filePath, "r");

    free(filePath); // free memory allocation

    int i=0;



    while(fgets(buffer, bufferLength, filePointer)) {
        buffer[strcspn(buffer, "\n")] = 0;
        char* token = strtok(buffer, separator);
        int j=0;

        while (token != NULL) {
            data[i][j++] = strdup(token);
            token = strtok(NULL, separator);
        }
        i++;
    }
    fclose(filePointer);

    struct dataContainer container;

    container.data = data;
    container.x = columnLength;
    container.y = fileLength;

    return container;
}

void writeData(char file[], char *data[]) {

}


void append_file(const char* filename, int numInputs, const char* inputs[]) 
{
     /*Things to add in this Function:
    - semicolon check
    - Validate numInputs with field numbers
     */
    

    char filePath[100];
    sprintf(filePath, "../data/%s", filename);

    printf("%s\n", filePath);

    FILE* fptr = filecheck(filePath, "a");
   
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

char **queryKey(char file[], char key[]) {

}

void updateData(char file[], char key[], char *data[]) {

}

void deleteKey(char file[], char key[]) {

}

char **queryField(char file[], char field[]) {

}

char **queryFieldStrict(char file[], char field[], char data[]) {

}

char **getFieldName(char file[]) {

}