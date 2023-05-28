#include "./bfc.h"

void print_usage() {
    fprintf(stdout,
            "Usage: bfc %s <filename> or bfc %s <source code>\n",
            COMMAND_FLAG_FILE,
            COMMAND_FLAG_CODE);
}

void print_error(const char *message) {
    fprintf(stderr, "Compilation error: %s\n", message);
}

char *open_file(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        print_error("Could not open input file.");
        return NULL;
    }

    // Find length of file (total bytes)
    fseek(file, 0, SEEK_END);
    unsigned length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Create heap string
    char *source = (char *)malloc(length + 1);
    assert(source != NULL);

    // Write to the string
    source[length] = 0;
    fread(source, 1, length, file);
    return source;
}

char *read_source(int argc, char **argv) {
    if (argc != 3) {
        print_usage();
        return NULL;
    }

    if (strcmp(argv[1], COMMAND_FLAG_FILE) == 0) {
        return open_file(argv[2]);
    } else if (strcmp(argv[1], COMMAND_FLAG_CODE) == 0) {
        // Move arg string to heap
        unsigned length = strlen(argv[2]);
        char *source = (char *)malloc(length + 1);
        assert(source != NULL);

        source[length] = 0;
        memcpy(source, argv[2], length);
        return source;
    } else {
        print_usage();
        return NULL;
    }
}

int main(int argc, char **argv) {
    char *source = read_source(argc, argv);
    if (source == NULL) {
        return 1;
    } else if (!validate(source)) {
        print_error("Invalid syntax.");
        return 1;
    } else {
        string_t assembly = parse(source);
        printf("%s", assembly.buffer);
        string_destroy(&assembly);
    }
    free(source);
    return 0;
}