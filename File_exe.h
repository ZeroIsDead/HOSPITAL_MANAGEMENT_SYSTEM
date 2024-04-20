#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

FILE* filecheck(const char* filename, const char* mode)
{
    //get Folder Path
    char folderPath[1024];
    if(getcwd(folderPath, sizeof(folderPath)) == NULL)
    {
        perror("getcwd() error");
    }

    //strcat the assignment folder path with data folder
    char dataPath[1024];
    sprintf(dataPath, "%s\\data", folderPath);
    
    //strcat the data folder path with filename
    char filePath[1024];
    sprintf(filePath, "%s\\%s", dataPath, filename);

    //check the existance of file in the data folder, if file doesnt exist, raise a wrong filename error and prevent creating a new file
    if(access(filePath, F_OK) == -1)
    {
        fprintf(stderr, "Error: Check Your File Name!!\nFilename %s in folder: %s does not exist!\n", filename, dataPath);
        return NULL;
    }
    
    //Open the File
    FILE* fptr = fopen(filePath, mode);

    //Open file error check
    if(fptr == NULL)
    {
        printf("Error opening file %s!\n", filename);
    }
    return fptr;
}

struct dataContainer2D {
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char*** data; // a 2D array containing each line of data
    int y; //y - number of lines / arrays in the 2D array
    int x; //x - number of columns / elements in each array
};

struct dataContainer1D {
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char** data; // an array containing one line or one column of data
    int x; // x - number of columns / elements in the array
};

struct dataContainer1D queryField(char* file, char* field);

// Garbage collection
void freeMalloc(struct dataContainer2D pointer) {
    for (int i = 0; i < pointer.y; i++) {
        free(pointer.data[i]);
    }
    free(pointer.data);
    free(pointer.fields);
}

// Get The Number of Lines in a file (Number of Users + (1 which is The Fields))
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

// Get the Number of Columns in a file (Number of fields - UserID,UserPW,Name,Tag)
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

// Return the record with the specified key in the field column
struct dataContainer2D queryFieldStrict(char* file, char* field, char* key) { 
    struct dataContainer1D fieldData = queryField(file, field);

    struct dataContainer2D returnedValue;
    returnedValue.error = 0;


    if (fieldData.error) { // Fail to Find
        returnedValue.error = 1;
        return returnedValue;
    }

    struct dataContainer2D data = getData(file);
    returnedValue.fields = data.fields;
    returnedValue.x = data.x;

    
    char** buffer[255];
    int count = 0;

    // Get the Data Records With the Key
    for (int i=0; i<data.y; i++) {
        if (!strncmp(fieldData.data[i], key, 255)) { // Compare Strings
            buffer[count] = data.data[i];
            count++;
        }
    }

    char*** returnedData = (char***) malloc (count * sizeof(char**));

    for (int i=0; i<count; i++) {
        returnedData[i] = (char**) malloc (data.x * sizeof(char*));
        for (int j=0; j<data.x; j++) {
            returnedData[i][j] = strdup(buffer[i][j]);
        }
    }

    returnedValue.data = returnedData;
    returnedValue.y = count;

    freeMalloc(data);
    free(fieldData.data);

    return returnedValue;
}

// DATA WRITE FUNCTIONS

int writeData(char* file, char** data) {

}

int append_file(const char* filename, int numInputs, const char* inputs[]) {
    
    /* ### This function write a new record(values) to a file ###
    Parameter: 
    - filename: name of the file [example : "Users1.txt", "Patient_IDs.txt"]
    - numInputs: number of inputs, count your number of values stored in your array
    - inputs: array of inputs
    
    Sample of Implementation:

    1. Get your users input as UserID, UserPW, Name, Tags

    2. Put your values into the array using:  
        
        const char* input[] = {UserID, UserPW, Name, Tags};

    3. Call the function append_file("filename.txt", number_of_values, name_of_array);
        
        append_file("Users.txt", 4, inputs);
    */

    /*Things to add in this Function:
    - Validate numInputs with field numbers
    */

    // Check for semicolon in inputs
    for (int i = 0; i < numInputs; i++) 
    {
        if (strchr(inputs[i], ';') != NULL) 
        {
            fprintf(stderr, "Error: semicolon found in inputs[%d]!\n", i);
            return 1;
        }
    }

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
        return 1;
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
    return 0;
}

int updateData(char* file, char* key, char** data) {

}

int deleteKey(char* file, char* key) {

}
