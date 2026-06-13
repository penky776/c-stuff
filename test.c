#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE *fileptr;
    fileptr = fopen(argv[1], "r");

    if(fileptr == NULL) {
        printf("File does not exist or cannot be opened\n");
        return 0;
    }

    return 0;
}