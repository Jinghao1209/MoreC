#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <string.h>

#define FILENAME "memmem.txt"

const char *filter_word[] = {
    "sad",
    "sadness"};

int main() {
    FILE *file = fopen(FILENAME, "r");

    if (file == NULL) {
        fprintf(stderr,"Error opening file\n");
        exit(1);
    }

    char *buffer = malloc(BUFSIZ);
    char *found = NULL;
    int found_count = 0;
    unsigned long long line = 1;

    while (fgets(buffer, BUFSIZ, file) != NULL) {
        for (size_t i = 0; i < sizeof(filter_word) / sizeof(filter_word[0]); i++) {
            found = memmem(buffer, strlen(buffer), filter_word[i], strlen(filter_word[i]));
            if (found != NULL) {
                printf("Found in string: %s\n", found);
                printf("Found address  : %p\n", found);
                printf("At line        : %llu\n", line);
                found_count++;
            }
        }

        line++;
    }

    fputs("Enter founded address: ", stdout);
    void *xl;
    scanf("%p", &xl);
    printf("Address %p have `%s` value.\n", xl, (char *)xl);
    printf("Found %d times.\n", found_count);

    // free after malloc
    free(buffer);

    if (fclose(file) != 0) {
        printf("Error closing file\n");
        exit(1);
    }

    return 0;
}
