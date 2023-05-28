#include "./parse.h"
#include "array.h"

void append_code(string_t *dst, const char *lines[], unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        string_concat(dst, lines[i]);
        string_push(dst, '\n');
    }
    string_push(dst, '\n');
}

string_t parse(char *source) {
    string_t assembly = string_create();
    append_code(&assembly,
                main_header,
                sizeof(main_header) / sizeof(main_header[0]));

    // Process tokens and create blocks
    array_t blocks = array_create(sizeof(string_t));

    // Write the footer of main subroutine
    append_code(&assembly,
                main_footer,
                sizeof(main_footer) / sizeof(main_footer[0]));

    // Append blocks to end-of-file and cleanup
    for (unsigned i = 1; i < blocks.size; i++) {
        string_t block;
        array_get(&blocks, i, &block);
        string_concat(&assembly, block.buffer);
        string_destroy(&block);
    }
    array_destroy(&blocks);

    return assembly;
}

bool validate(char *source) {
    unsigned balance = 0;
    unsigned n = strlen(source);
    for (unsigned i = 0; i < n; i++) {
        switch (source[i]) {
        case '[':
            balance++;
            break;
        case ']':
            balance--;
            break;
        default:
            break;
        }
    }
    return balance == 0;
}