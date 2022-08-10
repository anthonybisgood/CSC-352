#include <math.h>
#include <stdio.h>

int main(){
    float aLength, bLength, cLength;
    printf("Enter side A length:\n");
    scanf("%e", &aLength);
    printf("Enter side B length:\n");
    scanf("%e", &bLength);
    aLength = aLength * aLength;
    bLength = bLength * bLength;
    cLength = sqrt(aLength + bLength);
    printf("Length of side C: %.2f\n", cLength);
    return 0;
}
