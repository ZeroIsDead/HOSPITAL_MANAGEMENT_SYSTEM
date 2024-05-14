#include "File_exe.h"

char* message(char name []){ 
    
    int welcomeSize = 8 + strlen(name) + 1;
    char welcome[welcomeSize];
    
    strcpy(welcome, "Welcome ");
    strcat(welcome, name);
    
    welcome[welcomeSize-1] = '\0';

    return strdup(welcome);
    
}

int main(){
    char name []= "mugdho";
    char* header = message(name);
    char* options[] = {"PEPE","POPO","PEPEPOPO"};

    int output = displayMenu(header, options, 3);

    clearTerminal();
}
