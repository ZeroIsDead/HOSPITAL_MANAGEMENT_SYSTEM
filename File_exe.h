#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <ctype.h>

/*ULTILITIES*/

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
        return NULL;
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
        return NULL;
    }
    return filePointer;
}

/*A 2D data container to store entire data of the file.

you use this 2Dcontainer to store data from **getData() most of the time.

Implementaton:

*- call  struct dataContainer2D <container_name> = getData( file_name );
    -- container_name: is the name of the struct
    -- file_name: is the name of the file you want to read without .txt
    
To use the element in the struct:
    * - container_name.field_name : return you the field_names in the file 
    * - container_name.data : a 2D array containing each line from the file
    * - container_name.y : number of rows / horizontal lines count of the file 
    * - container_name .x : number of columns / vertical lines count of the file

*/
struct dataContainer2D 
{
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char*** data; // a 2D array containing each line of data
    int y; //y - number of rows / lines / arrays in the 2D array
    int x; //x - number of columns / elements in each array
};

/*A 1D data container to store a single line of data from  the file.

you use this 1Dcontainer to store data( single line ) from : queryField(), queryKey(), etc. 

Implementaton:
*- call  struct dataContainer1D <container_name> = queryField( file_name, field_name );
    -- container_name: is the name of the struct
    -- file_name: is the name of the file you want to read without .txt
    -- field_name: is the name of the field you want to read from the file
    
To use the element in the struct:
    * - container_name.field_name : return you the field_names in the file 
    * - container_name.data : a 1D contain specific line from the file according to the field_name you put
    * - container_name . x : number of columns / vertical lines count of the file / number of elements in the line

*/
struct dataContainer1D 
{
    int error; // 1 - error | 0 - fine
    char** fields; // an array containing the fields
    char** data; // an array containing one line or one column of data
    int x; // x - number of columns / elements in the array
};

struct dataContainer1D queryField(const char* file, char* field);

void clearTerminal() 
{
    printf("\e[1;1H\e[2J");
}

/*char* options[] = {"ar", "a", "b", "C"};

    displayMenu("GOD", options, 4);*/
int displayMenu(char* header, char* options[], int noOptions) 
{
    // get max sizeof option string
    int maxLength = strlen(header);
    int stringLengths[noOptions];
    char* modifiedOptions[noOptions];

    for (int i=0; i<noOptions; i++) {
        // format the strings
        char buffer[255];

        sprintf(buffer, "%d. %s", i+1, options[i]);

        modifiedOptions[i] = strdup(buffer);

        // get length
        int currentLength = strlen(modifiedOptions[i]);
        stringLengths[i] = currentLength;
    
        if (currentLength > maxLength) {
            maxLength = currentLength;
        }
    }

    const int horizontalpadding = 15;
    int borderLength = maxLength + 2 * horizontalpadding;

    // Top  Horizontal Line
    for (int i=0; i<borderLength; i++) {
        printf("-");
    }
    printf("\n");

    // Header
    const int verticalPadding = 1;

    int headerPadding = floor((borderLength - strlen(header) - 2)/2);
    
    const int borderCount = 2;
    const char character = '|';
    const int rightPadding = borderLength - borderCount - strlen(header) - headerPadding;

    printf("%c", character);

    for (int i=0; i<headerPadding; i++) {
        printf(" ");
    }

    printf(header);

    for (int i=0; i<rightPadding; i++) {
        printf(" ");
    }

    printf("%c", character);

    printf("\n");

    // Middle Horizontal Line
    for (int i=0; i<borderLength; i++) {
        printf("-");
    }
    printf("\n");

    const int leftPadding = 5;

    // Options
    for (int i=0; i<noOptions; i++) {
        const char* text = modifiedOptions[i];
        const int rightPadding = borderLength - borderCount - strlen(text) - leftPadding;

        printf("%c", character);

        for (int i=0; i<leftPadding; i++) {
            printf(" ");
        }

        printf(text);

        for (int i=0; i<rightPadding; i++) {
            printf(" ");
        }

        printf("%c", character);

        printf("\n");
    }

    // Bottom Horizontal Line
    for (int i=0; i<borderLength; i++) {
        printf("-");
    }
    printf("\n");

            // Get Input
        int bufferLength = 256;
        char input[bufferLength];
        printf("Enter your Input: ");

        fgets(input, bufferLength, stdin);   
        input[strcspn(input, "\n")] = 0;

        int isDigit = 0;
        for (int i; input[i]; i++) {
            isDigit += isdigit(input[i]);
        }

        int intInput = atoi(input);
        if (isDigit && 0 < intInput && intInput <= noOptions) {
            return options[intInput-1];
        }

        // Lowercase the input string
        for (int j=0; input[j]; j++) {
            input[j] = tolower(input[j]);
        }

        // Repeat Menu until Valid Input
        for (int i=0; i<noOptions; i++) {
            char* option = strdup(options[i]);

            // Lowercase the option string
            for (int j=0; option[j]; j++) {
                option[j] = tolower(option[j]);
            }

            // Compare the strings
            if (!strncmp(input, option, bufferLength)) {
                return options[i];
            }
        }

        // Repeat Menu until Valid Input
        clearTerminal();
        displayMenu(header, options, noOptions);
}

/* This function frees the memory allocated for the dataContainer2D struct.
 * It iterates over each line of data, frees the memory allocated for each element in that line,
 * and then frees the memory allocated for the line itself. Finally, it frees the memory
 * allocated for the fields and the data array.
 */
void freeMalloc2D(struct dataContainer2D pointer) 
{
    for (int i = 0; i < pointer.y; i++) {
        free(pointer.data[i]);
    }
    free(pointer.data);
    free(pointer.fields);
}

// Frees the memory allocated for the dataContainer1D struct
void freeMalloc1D(struct dataContainer1D pointer) {
    free(pointer.data);
    free(pointer.fields);
}

///////////////////////////////////*DATA READ FUNCTIONS*////////////////////////////////////////////////

// returns a struct that holds the field and data of the corresponding file
struct dataContainer2D getData(const char* filename) 
{
    int bufferLength = 255;
    char buffer[bufferLength]; 

    const char* separator = ";";

    struct dataContainer2D container;
    container.error = 0;

    FILE* filePointer = filecheck(filename, "r");

    if (filePointer == NULL) {
        container.error = 1;
        return container;
    }

    const int fieldColumn = 1;

    // Get Dimensions of the Array
    int fileLength = -1; // y
    int columnLength = -1; // x

    // get File Length - Y
    while(fgets(buffer, bufferLength, filePointer)) 
    {
        fileLength++;
    }

    rewind(filePointer);

    // get Column Length - X
    fgets(buffer, bufferLength, filePointer);

    // Tokenize the line based on the separators
    char* token = strtok(buffer, separator);

    // Keep counting the number of separators until there are no more
    while (token != NULL) 
    {
        // Increment the column counter
        columnLength++;

        // Get the next token
        token = strtok(NULL, separator);
    }

    rewind(filePointer);

    // Create Array of Pointers
    char*** data = malloc (fileLength * sizeof(char**)); 

    // Place Pointers of Arrays into the Array to Make a 2D array
    for (int i=0; i < fileLength; i++) 
    {
        data[i] = malloc (columnLength * sizeof(char*));
    }

    // get Fields
    char** fields = malloc (columnLength * sizeof(char*));

    fgets(buffer, bufferLength, filePointer);
    buffer[strcspn(buffer, "\n")] = 0;
    token = strtok(buffer, separator);
    int j=0;

    while (token != NULL) 
    {
        fields[j++] = strdup(token);
        token = strtok(NULL, separator);
    }

    container.fields = fields;

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

    container.data = data;
    container.x = columnLength;
    container.y = fileLength;

    return container;
}

/*____________________________3 Functions that you can call___________________________________________________________________________________*/

/*Horizontal search using unique_key, return a line of values with that unique_key.
    * - example: 
        -- unique_key = POTATO1
        returns in .data: 
                    * - POTATO1;password123;John Smith;Doctor;
 
To call this function:
    struct dataContainer1D <container_name> = queryKey(file_name, unique_key );
    -- container_name: is the name of the struct
    -- file_name: name of the file that you want to read
    -- unique_Key: the unique key for the line you want to search for


 */
struct dataContainer1D queryKey(const char* filename, char* key) 
{
    struct dataContainer2D data = getData(filename);

    struct dataContainer1D returnedValue;
    returnedValue.error = 0;

    if (data.error) {
        freeMalloc2D(data);
        returnedValue.error = 1;
        return returnedValue;
    }


    // Get all Data In the Specified Field Column
    char** IDs = malloc (data.y * sizeof(char*));

    const int fieldColumn = 0;

    for (int i=0; i<data.y; i++) 
    {
        IDs[i] = strdup(data.data[i][fieldColumn]);
    }

    char** relayDataArray = malloc (data.x * sizeof(char*));
    char** relayFieldArray = malloc (data.x * sizeof(char*));


    for (int i=0; i<data.y; i++) 
    {
        if (!strncmp(IDs[i], key, 255)) 
        {
            // Copy Data to Relay Array
            for (int j=0; j<data.x; j++) {
                relayDataArray[j] = strdup(data.data[i][j]);
                relayFieldArray[j] = strdup(data.fields[j]);
            }

            returnedValue.data = relayDataArray;
            returnedValue.fields = relayFieldArray;
            returnedValue.x = data.x;

            freeMalloc2D(data);

            return returnedValue;
        }
        
    }

    free(IDs);
    freeMalloc2D(data);
    returnedValue.error = 1;
    return returnedValue;
}

/*Vertical search using field_name, return a line of values with that field_name
    * - example: struct dataContainer1D UserPW = queryField("users", "UserPW");
        -- file : users.txt
        -- field_name = UserPW
        -- returns : all password values

To call this function:A
    struct dataContainer1D <container_name> = queryField(file_name, field );
    -- container_name: is the name of the struct
    -- file_name: name of the file that you want to read
    -- field: the field you want to use for the search
*/
struct dataContainer1D queryField(const char* filename, char* field) 
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

    // Fail to find
    if (fieldColumn == -1) 
    {
        freeMalloc2D(data);
        returnedValue.error = 1;
        return returnedValue;
    }

    // Get all Data In the Specified Field Column
    char** fieldData = malloc (data.y * sizeof(char*));

    for (int i=0; i<data.y; i++) 
    {
        fieldData[i] = strdup(data.data[i][fieldColumn]);
    }

    returnedValue.data = fieldData;
    returnedValue.x = data.y;

    char** fieldName = malloc (1 * sizeof(char*));

    returnedValue.fields = fieldName;

    freeMalloc2D(data);

    return returnedValue;
}

/*Horizontal search using field_name, return a horizontal line of values using the field
    * - example: 
        -- fieldname = Tags
        -- fieldvalue = Doctor
        returns in .data: 
                        * - POTATO1;password123;John Smith;Doctor;
                        * - POTATO4;abc123;Sarah Brown;Doctor;
                        * - POTATO7;secure432;Brian Miller;Doctor;
To call this function:
    struct dataContainer1D <container_name> = queryFieldStrict(file_name, field, key );
    -- container_name: is the name of the struct
    -- file_name: name of the file that you want to read
    -- field: the field you want to use for the search
    -- key : the value you want to search in the field
*/
struct dataContainer2D queryFieldStrict(const char* filename, char* field, char* key) 
{ 
    struct dataContainer2D returnedValue;
    returnedValue.error = 0;

    struct dataContainer2D data = getData(filename);

    if (data.error) {
        returnedValue.error = 1;
        return returnedValue;
    }

    char** relayFieldArray = malloc (data.x * sizeof(char*));
    for (int i=0; i<data.x; i++) {
        relayFieldArray[i] = strdup(data.fields[i]);
    }

    returnedValue.fields = relayFieldArray;
    returnedValue.x = data.x;

    // Get Field Index
    int fieldColumn = -1;

    for (int i=0; i<data.x; i++) 
    {
        if (!strncmp(data.fields[i], field, 255))
        {
            fieldColumn = i;
            break;
        }
    }

    // Fail to find
    if (fieldColumn == -1) 
    {
        freeMalloc2D(data);
        returnedValue.error = 1;
        return returnedValue;
    }

    // Get all Data In the Specified Field Column
    char** fieldData = malloc (data.y * sizeof(char*));

    for (int i=0; i<data.y; i++) 
    {
        fieldData[i] = strdup(data.data[i][fieldColumn]);
    }

    char** buffer[data.y];
    int count = 0;

    // Get the Data Records With the Key
    for (int i=0; i<data.y; i++) 
    {
        if (!strncmp(fieldData[i], key, 255)) 
        { // Compare Strings
            buffer[count++] = data.data[i];
        }
    }

    char*** returnedData = malloc (count * sizeof(char**));

    for (int i=0; i<count; i++) 
    {
        returnedData[i] = malloc (data.x * sizeof(char*));
        for (int j=0; j<data.x; j++) 
        {
            returnedData[i][j] = strdup(buffer[i][j]);
        }
    }

    returnedValue.data = returnedData;
    returnedValue.y = count;

    freeMalloc2D(data);
    free(fieldData);

    return returnedValue;
}

/*__________________________Functions that you can call ends here___________________________________________________________________________________*/

///////////////////////////////////* DATA WRITE FUNCTIONS*////////////////////////////////////////////////

//take 2D array and concatenate into one line and write into file
int writeData(const char* filename, struct dataContainer2D array) 
{
    FILE* filePointer = filecheck(filename, "w");
    
    char line[array.x * 2 + 1]; 
    
    /*Field names*/
    line[0] = '\0'; // Initialize the line buffer
    
    //concatenate into one line
    for (int i = 0; i < array.x; i++)
    {
        strcat(line, array.fields[i]); 
        strcat(line, ";");
    }
    strcat(line, "\n");

    //write field_names into file
    fputs(line, filePointer);
    
    /*Values*/
    for (int i=0; i < array.y; i++)
    {   
        // Clear line buffer
        line[0] = '\0';
        
        //concatenate into one line
        for (int j = 0; j < array.x; j++)
        {
            strcat(line, array.data[i][j]); 
            strcat(line, ";");
        }
        strcat(line, "\n");

        //write line into file
        fputs(line, filePointer);

    }

    fclose(filePointer);

    freeMalloc2D(array);

    return 0; // Return if Nothing Goes Wrong
}

/*____________________________3 Functions that you can call starts here___________________________________________________________________________________*/

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
int write_new_data(const char* filename, int numInputs, const char* inputs[]) 
{
    
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

/*Update existing record in existing file

*Parameter: 
    * - filename: name of the file without .txt [example : users, Patient_IDs, Inventory]

    * - relaying_array: array that store all updated inputs
        -- if you do: struct dataContainer1D master = queryKey("users", UserID);
        data.data is your array

        >> you can read struct datacontainer1D above

updateData("file_name", master.data);

Sample of implementation:

*1. Get your unqiue_key input as Unique_key

*2. use: struct dataContainer1D master = queryKey("file_name", Unique_key);
    * - the data is stored in master.data 

*3. Modify accordingly by using different index of master.data[ your_index_here ]

*4. call: updateData("file_name_without.txt", master.data);

*/
int updateData(const char* filename, char** relaying_array) 
{   
    //getData of the file
    struct dataContainer2D master = getData(filename);
    const int uniqueIDIndex = 0;

    //iterate vertically through data search for the unique key
    for (int i=0; i<master.y; i++) 
    {   
        //IF FOUND the unique
        if(!strncmp(master.data[i][uniqueIDIndex], relaying_array[uniqueIDIndex], 255))
        {
            //store the relaying array into master

            for (int j=0; j<master.x; j++) 
            {
                master.data[i][j] = strdup(relaying_array[j]);
            }
        }
    }    

    return writeData(filename, master);
}   

/*Delete existing record in existing file:

*Parameter:
    * - filename: name of the file without .txt
    * - unique_key: unique key of the record you want to delete

deleteKey("filename", unique_key);

Sample of implementation:

*1. Get your the unqiue_key of the lines of record you want to delete

*2. call: deleteKey("filename_without.txt", unique_key);
*/
int deleteKey(const char* filename, char* unique_key) 
{
    struct dataContainer2D master = getData(filename);
    
    master.y = master.y - 1;

    for (int i=0; i<master.y; i++) 
    {   
        //IF FOUND the unique
        if(!strncmp(master.data[i][0], unique_key, 255))
        {   
            //delete that record
            free(master.data[i]);
            for (int j=i; j<master.y; j++)
            {
                master.data[j] = master.data[j+1];
            }
            
        }
    }    

    return writeData(filename, master);
}

/*___________________________Functions that you can call ends here_________________________________________________________________________*/