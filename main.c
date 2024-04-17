#include "./code/utility.h"
// #include "./code/admin.h"
// #include "./code/doctor.h"
// #include "./code/nurse.h"
// #include "./code/patient.h"

int main() {
    struct dataContainer data = getData("users");
    for (int i=0; i<data.y; i++) {
        for (int j=0; j<data.x; j++) {
            printf("%s ",data.data[i][j]);

        }
        printf("\n");
    }
    freeMalloc(data);

}