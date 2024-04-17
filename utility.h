#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void appendData(char file[], char *data[]) {

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