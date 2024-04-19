#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dataContainer2D {
    int error;
    char** fields;
    char*** data;
    int y; //y
    int x; //x
};

struct dataContainer1D {
    int error;
    char** fields;
    char** data;
    int x;
};

struct dataContainer1D queryField(char* file, char* field);

void freeMalloc(struct dataContainer2D pointer) {
    for (int i = 0; i < pointer.y; i++) {
        free(pointer.data[i]);
    }
    free(pointer.data);
    free(pointer.fields);
}

int getFileNumOfLines(char* file) {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength];

    int count = 0;

    filePointer = fopen(file, "r");

    while(fgets(buffer, bufferLength, filePointer)) {
        count++;
        }
        
    fclose(filePointer);

    return count;
}

int getFileNumberOfColumns(char* file) {
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

// DATA READ FUNCTIONS

// returns a struct that holds the field and data of the corresponding file
struct dataContainer2D getData(const char* file) {
    FILE* filePointer;
    int bufferLength = 255;
    char buffer[bufferLength]; 

    const char* separator = ";";

    // Concatenate FilePath
    const char* folder = "./data/";
    const char* extension = ".txt";

    char* filePath;
    filePath = malloc(strlen(extension) + strlen(folder) + strlen(file) + 1);
    strcpy(filePath, folder); 
    strcat(filePath, file); 
    strcat(filePath, extension); 

    filePointer = fopen(filePath, "r");

    if (filePointer == NULL) {
        printf("FAIL FUCK");
        EXIT_FAILURE;
    }

    const int fieldColumn = 1;

    // Get Dimensions of the Array
    int fileLength = getFileNumOfLines(filePath) - fieldColumn; // y
    int columnLength = getFileNumberOfColumns(filePath); // x

    // Create Array of Pointers
    char*** data = (char***) malloc (fileLength * sizeof(char**)); 

    // Place Pointers of Arrays into the Array to Make a 2D array
    for (int i=0; i < fileLength; i++) {
        data[i] = (char**) malloc (columnLength * sizeof(char*));
    }

    free(filePath); // free memory allocation

    // get Fields
    char** fields = (char**) malloc (columnLength * sizeof(char*));

    fgets(buffer, bufferLength, filePointer);
    buffer[strcspn(buffer, "\n")] = 0;
    char* token = strtok(buffer, separator);
    int j=0;

    while (token != NULL) {
        fields[j++] = strdup(token);
        token = strtok(NULL, separator);
    }

    // Get Data
    int i=0;

    while(fgets(buffer, bufferLength, filePointer)) { // Reads line by line until end of file
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

    struct dataContainer2D container;

    container.error = 0;
    container.fields = fields;
    container.data = data;
    container.x = columnLength;
    container.y = fileLength;

    return container;
}

// returns the record with the corresponding key/ID
struct dataContainer1D queryKey(char* file, char* key) {
    struct dataContainer2D data = getData(file);

    char* IDField = data.fields[0];

    struct dataContainer1D IDs = queryField(file, IDField);


    struct dataContainer1D returnedValue;
    for (int i=0; i<data.y; i++) {
        if (!strncmp(IDs.data[i], key, 255)) {

            returnedValue.data = data.data[i];
            returnedValue.fields = data.fields;
            returnedValue.x = data.x;

            return returnedValue;
        }
    }

    return returnedValue;
}

// Returns an array of values in the field column
struct dataContainer1D queryField(char* file, char* field) {
    struct dataContainer2D data = getData(file);

    int fieldColumn = -1;

    for (int i=0; i<data.x; i++) {
        if (!strncmp(data.fields[i], field, 255)) {
            fieldColumn = i;
            break;
        }
    }

    struct dataContainer1D returnedValue;
    returnedValue.error = 0;

    // Fail to find
    if (fieldColumn == -1) {
        freeMalloc(data);
        returnedValue.error = 1;
        return returnedValue;
    }

    // Get all Data In the Specified Field Column
    char** fieldData = (char**) malloc (data.y * sizeof(char*));

    for (int i=0; i<data.y; i++) {
        fieldData[i] = strdup(data.data[i][fieldColumn]);
    }

    returnedValue.data = fieldData;
    returnedValue.x = data.y;

    freeMalloc(data);

    return returnedValue;
}

// Return the record with  the specified key in the field column
struct dataContainer1D queryFieldStrict(char* file, char* field, char* key) { 
    struct dataContainer1D fieldData = queryField(file, field);

    struct dataContainer1D returnedValue;
    returnedValue.error = 0;

    if (fieldData.error) { // Fail to Find
        returnedValue.error = 1;
        return returnedValue;
    }

    struct dataContainer2D data = getData(file);


    // Get the Data Record With the 
    for (int i=0; i<data.y; i++) {
        if (!strncmp(fieldData.data[i], key, 255)) { // Compare Strings
            returnedValue.fields = data.fields;
            returnedValue.data = data.data[i];
            returnedValue.x = data.x;

            return returnedValue;
        }
    }

    freeMalloc(data);
    free(fieldData.data);

    return returnedValue;
}

// DATA WRITE FUNCTIONS

int writeData(char* file, char** data) {

}

int appendData(char* file, char** data) {

}

int updateData(char* file, char* key, char** data) {

}

int deleteKey(char* file, char* key) {

}

