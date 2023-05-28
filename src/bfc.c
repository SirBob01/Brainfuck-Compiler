#include "./bfc.h"

void print_usage() {
    fprintf(
        stdout,
        "Usage:\n"
        "\tbfc %s <filename> or bfc %s <source code>\n"
        "\tOptionally, include %s <output filename> to specify the output.\n",
        COMMAND_FLAG_FILE,
        COMMAND_FLAG_CODE,
        COMMAND_FLAG_OUTPUT);
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
    if (source) {
        source[length] = 0;
        fread(source, 1, length, file);
    } else {
        print_error("Could not allocate memory for input.");
    }
    fclose(file);
    return source;
}

input_t read_source(int argc, char **argv) {
    input_t input;
    input.source = NULL;
    input.target = NULL;

    // Check for correct number of arguments
    if (argc != 3 && argc != 5) {
        print_usage();
        return input;
    }

    // Get input source string
    if (strcmp(argv[1], COMMAND_FLAG_FILE) == 0) {
        input.source = open_file(argv[2]);
    } else if (strcmp(argv[1], COMMAND_FLAG_CODE) == 0) {
        // Move arg string to heap
        unsigned length = strlen(argv[2]);
        input.source = (char *)malloc(length + 1);
        if (input.source) {
            input.source[length] = 0;
            memcpy(input.source, argv[2], length);
        } else {
            print_error("Could not allocate memory for input.");
        }
    } else {
        print_usage();
        return input;
    }

    // Get target file
    if (argc == 5) {
        if (strcmp(argv[3], COMMAND_FLAG_OUTPUT) == 0) {
            input.target = fopen(argv[4], "w");
            if (input.target == NULL) {
                input.source = NULL;
                print_error("Could not open target file.");
            }
        } else {
            print_usage();
            return input;
        }
    }
    return input;
}

int main(int argc, char **argv) {
    input_t input = read_source(argc, argv);
    if (input.source == NULL) {
        return 1;
    } else if (!validate(input.source)) {
        print_error("Invalid syntax.");
    } else {
        string_t assembly = parse(input.source);
        if (input.target != NULL) {
            fwrite(assembly.buffer, 1, assembly.length, input.target);
            fclose(input.target);
        } else {
            fprintf(stdout, "%s", assembly.buffer);
        }
        string_destroy(&assembly);
    }
    free(input.source);
    return 0;
}