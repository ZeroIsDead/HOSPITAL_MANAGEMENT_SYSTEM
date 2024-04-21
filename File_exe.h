#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*This function open the file for you, check the existance of file in the data folder, if file doesnt exist, raise a wrong filename error and prevent creating a new file, lastly return a file pointer of the file you opened.
* 
*Parameter:
* - filename: The name of the file you want to open. (without .txt)
* - mode : file mode you want to open the file with.
*
* Returns: A pointer to the file.
*/
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
    sprintf(filePath, "%s\\%s.txt", dataPath, filename);

    //check the existance of file in the data folder, if file doesnt exist, raise a wrong filename error and prevent creating a new file
    if(access(filePath, F_OK) == -1)
    {
        fprintf(stderr, "Error: Check Your File Name!!\nFilename %s in folder: %s does not exist!\n", filename, dataPath);
        return NULL;
    }
    
    //Open the File
    FILE* filePointer = fopen(filePath, mode);

    //Open file error check
    if(filePointer == NULL)
    {
        printf("Error opening file %s!\n", filename);
    }
    return filePointer;
}

struct dataContainer2D 
{
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char*** data; // a 2D array containing each line of data
    int y; //y - number of lines / arrays in the 2D array
    int x; //x - number of columns / elements in each array
};

struct dataContainer1D 
{
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char** data; // an array containing one line or one column of data
    int x; // x - number of columns / elements in the array
};

struct dataContainer1D queryField(char* file, char* field);

/* This function frees the memory allocated for the dataContainer2D struct.
 * It iterates over each line of data, frees the memory allocated for each element in that line,
 * and then frees the memory allocated for the line itself. Finally, it frees the memory
 * allocated for the fields and the data array.
 */
void freeMalloc(struct dataContainer2D pointer) 
{
    for (int i = 0; i < pointer.y; i++) {
        free(pointer.data[i]);
    }
    free(pointer.data);
    free(pointer.fields);
}

// Get The Number of Lines in a file (Number of Users + (1 which is The Fields))
/*
 * This function takes a file path as input and returns the number of lines in that file.
 * The function opens the file in read mode, reads each line until the end of the file is reached,
 * and increments a counter for each line. Finally, it closes the file and returns the counter.
 */
int getFileNumOfLines(const char* filename) 
{
    FILE* filePointer = filecheck(filename, "r");

    int bufferLength = 255;
    char buffer[bufferLength];

    int count = 0;

    while(fgets(buffer, bufferLength, filePointer)) 
    {
        count++;
    }
        
    fclose(filePointer);

    return count;
}

// Get the Number of Columns in a file (Number of fields - UserID,UserPW,Name,Tag)
/*
 * This function takes a file path as input and returns the number of columns in that file.
 * The function opens the file in read mode, reads the first line, and counts the number of
 * separators in that line. Finally, it closes the file and returns the number of columns.
 */
int getFileNumberOfColumns(const char* filename) 
{   
    FILE* filePointer = filecheck(filename, "r");

    int bufferLength = 255;
    char buffer[bufferLength]; /* not ISO 90 compatible */

    const char* separator = ";";

    int count = 0;

    fgets(buffer, bufferLength, filePointer);

    // Tokenize the line based on the separators
    char* token = strtok(buffer, separator);

    // Keep counting the number of separators until there are no more
    while (token != NULL) 
    {
        // Increment the column counter
        count++;

        // Get the next token
        token = strtok(NULL, separator);
    }

    fclose(filePointer);

    const int expectedSeparator = 1;

    return count - expectedSeparator;
}

// DATA READ FUNCTIONS

// returns a struct that holds the field and data of the corresponding file
struct dataContainer2D getData(const char* filename) 
{
    int bufferLength = 255;
    char buffer[bufferLength]; 

    const char* separator = ";";

    FILE* filePointer = filecheck(filename, "r");

    const int fieldColumn = 1;

    // Get Dimensions of the Array
    int fileLength = getFileNumOfLines(filename) - fieldColumn; // y
    int columnLength = getFileNumberOfColumns(filename); // x

    // Create Array of Pointers
    char*** data = (char***) malloc (fileLength * sizeof(char**)); 

    // Place Pointers of Arrays into the Array to Make a 2D array
    for (int i=0; i < fileLength; i++) 
    {
        data[i] = (char**) malloc (columnLength * sizeof(char*));
    }

    // get Fields
    char** fields = (char**) malloc (columnLength * sizeof(char*));

    fgets(buffer, bufferLength, filePointer);
    buffer[strcspn(buffer, "\n")] = 0;
    char* token = strtok(buffer, separator);
    int j=0;

    while (token != NULL) 
    {
        fields[j++] = strdup(token);
        token = strtok(NULL, separator);
    }

    // Get Data
    int i=0;

    while(fgets(buffer, bufferLength, filePointer)) 
    { // Reads line by line until end of file
        buffer[strcspn(buffer, "\n")] = 0;
        char* token = strtok(buffer, separator);
        int j=0;

        while (token != NULL) 
        {
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

/*This function queries a file for a record for only unique key/ID.
 * It takes a filename and a key as input, and returns a struct
 * that contains the corresponding data and fields of the record.
 */
struct dataContainer1D queryKey(char* filename, char* key) 
{
    struct dataContainer2D data = getData(filename);

    struct dataContainer1D returnedValue;
    returnedValue.error = 0;

    if (data.error) {
        returnedValue.error = 1;
        return returnedValue;
    }

    char* IDField = data.fields[0];

    struct dataContainer1D IDs = queryField(filename, IDField);

    if (IDs.error) {
        returnedValue.error = 1;
        freeMalloc(data);
        return returnedValue;
    }


    for (int i=0; i<data.y; i++) 
    {
        if (!strncmp(IDs.data[i], key, 255)) 
        {

            returnedValue.data = data.data[i];
            returnedValue.fields = data.fields;
            returnedValue.x = data.x;

            return returnedValue;
        }
    }

    return returnedValue;
}

// Returns an array of values in the field column
struct dataContainer1D queryField(char* filename, char* field) 
{
    struct dataContainer2D data = getData(filename);

    struct dataContainer1D returnedValue;
    returnedValue.error = 0;

    if (data.error) {
        returnedValue.error = 1;
        return returnedValue;
    }

    int fieldColumn = -1;

    for (int i=0; i<data.x; i++) 
    {
        if (!strncmp(data.fields[i], field, 255))
        {
            fieldColumn = i;
            break;
        }
    }

    printf("%d\n", fieldColumn);

    // Fail to find
    if (fieldColumn == -1) 
    {
        freeMalloc(data);
        returnedValue.error = 1;
        return returnedValue;
    }

    // Get all Data In the Specified Field Column
    char** fieldData = (char**) malloc (data.y * sizeof(char*));

    for (int i=0; i<data.y; i++) 
    {
        fieldData[i] = strdup(data.data[i][fieldColumn]);
    }

    returnedValue.data = fieldData;
    returnedValue.x = data.y;

    freeMalloc(data);

    return returnedValue;
}

// Return the record with the specified key in the field column
struct dataContainer2D queryFieldStrict(char* filename, char* field, char* key) 
{ 
    struct dataContainer1D fieldData = queryField(filename, field);

    struct dataContainer2D returnedValue;
    returnedValue.error = 0;


    if (fieldData.error) 
    { // Fail to Find
        returnedValue.error = 1;
        return returnedValue;
    }

    struct dataContainer2D data = getData(filename);
    returnedValue.fields = data.fields;
    returnedValue.x = data.x;

    
    char** buffer[255];
    int count = 0;

    // Get the Data Records With the Key
    for (int i=0; i<data.y; i++) 
    {
        if (!strncmp(fieldData.data[i], key, 255)) 
        { // Compare Strings
            buffer[count] = data.data[i];
            count++;
        }
    }

    char*** returnedData = (char***) malloc (count * sizeof(char**));

    for (int i=0; i<count; i++) 
    {
        returnedData[i] = (char**) malloc (data.x * sizeof(char*));
        for (int j=0; j<data.x; j++) 
        {
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

int writeData(char* filename, char** data) 
{

}

/* This function write a new record(values) to a file.
    *Parameter: 
    * - filename: name of the file without .txt [example : "Users1", "Patient_IDs"]
    * - numInputs: number of inputs, count your number of values stored in your array
    * - inputs: array of inputs
    
    * Sample of Implementation:

    *1. Get your users input as UserID, UserPW, Name, Tags

    *2. Put your values into the array using:  
        
    * - const char* input[] = {UserID, UserPW, Name, Tags};

    *3. Call the function append_file("filename", number_of_values, name_of_array);
        
    * - append_file("Users", 4, inputs);
*/
int append_file(const char* filename, int numInputs, const char* inputs[]) {
    
    /*Things to add in this Function:
    - Validate numInputs with field numbers
    */
    
    // Make a copy of the input array to avoid modifying the original data
    char* inputs_copy[numInputs];
    for (int i = 0; i < numInputs; ++i) 
    {
        inputs_copy[i] = strdup(inputs[i]);
    }

    // Remove newline characters from each input
    for (int i = 0; i < numInputs; ++i) 
    {
        inputs_copy[i][strcspn(inputs_copy[i], "\n")] = '\0';
    }

    // Check for semicolon in inputs
    for (int i = 0; i < numInputs; i++) 
    {
        if (strchr(inputs_copy[i], ';') != NULL) 
        {
            fprintf(stderr, "Error: semicolon found in inputs[%d]!\n", i);
            return 1;
        }
    }

    FILE* filePointer = filecheck(filename, "a");
   
    // Calculate the total length needed for the input string
    int total_len = 0;
    for (int i = 0; i < numInputs; ++i) 
    {
        total_len += snprintf(NULL, 0, "%s;", inputs_copy[i]);
    }
    total_len += 1; // Add 1 for the newline character

    char* line = malloc(total_len); // Allocate memory for the line
    
    // Check for null pointer
    if (line == NULL) 
    { 
        printf("Error allocating memory!\n");
        fclose(filePointer);
        return 1;
    }

    // Construct the input string
    line[0] = '\0';
    for (int i = 0; i < numInputs; ++i) 
    {
        strcat(line, inputs_copy[i]); 
        strcat(line, ";");
    }
    strcat(line, "\n"); // Add a newline character at the end

    // Write the input string to the file
    fputs(line, filePointer);

    // Cleanup
    free(line);
    fclose(filePointer);
    return 0;
}

int updateData(char* filename, char* key, char** data) {
// Sandaris will handle
}

int deleteKey(char* file, char* key) {
// working on it
}
