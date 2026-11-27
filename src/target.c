#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 0;
    }

    char input[6];
    strncpy(input, argv[1], 5);

    if (input[0] == 'M') {
        if (input[1] == 'A') {
            if (input[2] == 'G') {
                if (input[3] == 'I') {
                    if (input[4] == 'C') {
                        puts("PANIC");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
    return 0;
}