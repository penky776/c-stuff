#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    float vector[5] = {1.2, 2.4, 3.5, 4.1, 5.0};
    
    int len = sizeof(vector) / sizeof(vector[0]);
    float sum = 0;
    for (int i=0; i<len; i++) {
        sum += vector[i] * vector[i];
    }

    printf("len: %d \n", len);
    printf("The length of the vector [");
    for (int i = 0; i < len; i++) {
        if (i > 0 && i != len) { // not the first or last element
            printf(", ");
        }
        printf("%2.1f", vector[i]);
    }
    printf("] is %3.2f", sqrt(sum));

    return 0;
}