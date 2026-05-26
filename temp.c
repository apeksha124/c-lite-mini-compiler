#include <stdio.h>

int main() {
    // This is a test of comment removal

    /* This should be removed by Lex */

    int a = 5; // another comment
    printf("Value: %d\n", a);

    return 0;
}