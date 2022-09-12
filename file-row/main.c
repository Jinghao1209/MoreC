#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) (void)(x)
#define CONTENT_LENGTH_MAX 1024

typedef struct file_content_t file_content_t;

struct file_content_t {
    char text[CONTENT_LENGTH_MAX];
    size_t sz;
};

typedef struct {
    size_t sz;
    file_content_t pools[CONTENT_LENGTH_MAX];
} file_content_pool_t;

file_content_pool_t file_content_pool = {0};

void print(FILE *out, const char *cstring) {
    fputs(cstring, out);
    return;
}

void set_file_content(file_content_t *file_content, const char *cstring) {
    size_t n = strlen(cstring);

    if (n > CONTENT_LENGTH_MAX - 1) {
        n = CONTENT_LENGTH_MAX - 1;
    }

    memset(file_content->text, 0, CONTENT_LENGTH_MAX);
    memcpy(file_content->text, cstring, n);
}

file_content_t *file_content_alloc(file_content_pool_t *file_content_pool) {
    if (file_content_pool->sz > CONTENT_LENGTH_MAX) {
        print(stderr, "file_content_pool full");
    }

    file_content_t *file_content = &file_content_pool->pools[file_content_pool->sz];
    memset(file_content, 0, sizeof(file_content_t));
    file_content_pool->sz++;
    return file_content;
}

void print_pool(file_content_pool_t *file_content_pool) {
    for (size_t i = 0; i < file_content_pool->sz; i++) {
        print(stdout, file_content_pool->pools[i].text);
    }
}

int main(int argc, char const *argv[]) {
    UNUSED(argc);

    if (!(argc > 1)) {
        print(stderr, "ERROR: NOT A VALID FILE\n\n");
        fprintf(stdout, "Usage: %s <filename>\n", argv[0]);

        exit(1);
    }

    const char *filename = argv[1];

    FILE *file = fopen(filename, "rw");

    if (file == NULL) {
        fprintf(stderr, "ERROR: could not open file %s: %s\n", filename, strerror(errno));
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long original_file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("ORIGINAL_FILE_SIZE: %ld\n\n", original_file_size);

    char *line_buf = NULL;
    size_t line_size_buf = 0;
    ssize_t line_size = 0;
    while ((line_size = getline(&line_buf, &line_size_buf, file)) > 0) {
        file_content_t *file_content = file_content_alloc(&file_content_pool);

        if (line_buf[line_size - 1] != 10) {
            line_buf[line_size] = 10;          // add "\n" into last character
            line_buf[line_size + 1] = 0;
            line_size++;
        }

        set_file_content(file_content, line_buf);
        file_content->sz = (size_t) line_size; // let pool or content know len

        memset(line_buf, 0, line_size);          // clean all str cache
    }

    print(stdout, "CONTENT: \n");
    print_pool(&file_content_pool);

    fclose(file);
    return 0;
}
