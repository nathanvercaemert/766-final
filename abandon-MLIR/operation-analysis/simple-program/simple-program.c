#include <stdio.h>

int main() {
    int a, b, c;
    int output = 0;

    printf("Enter the value for a: ");
    scanf("%d", &a);

    printf("Enter the value for b: ");
    scanf("%d", &b);

    printf("Enter the value for c: ");
    scanf("%d", &c);

    for (int i = 0; i < a; i++) {
        output += c;

        for (int j = 0; j < b; j++) {
            output++;
            output *= c;
        }
    }

    printf("Output: %d\n", output);
    return 0;
}
