#include "./parse.h"

void write_block(string_t *assembly, const char **lines, unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        string_concat(assembly, lines[i]);
        string_push(assembly, '\n');
    }
    string_push(assembly, '\n');
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

string_t parse(char *source) {
    string_t assembly = string_create();
    write_block(&assembly, asm_head, sizeof(asm_head) / sizeof(asm_head[0]));

    unsigned n = strlen(source);
    for (unsigned i = 0; i < n; i++) {
        switch (source[i]) {
        case TOKEN_PTR_INC:
            write_block(&assembly,
                        asm_ptr_inc,
                        sizeof(asm_ptr_inc) / sizeof(asm_ptr_inc[0]));
            break;
        case TOKEN_PTR_DEC:
            write_block(&assembly,
                        asm_ptr_dec,
                        sizeof(asm_ptr_dec) / sizeof(asm_ptr_dec[0]));
            break;
        case TOKEN_VAL_INC:
            write_block(&assembly,
                        asm_val_inc,
                        sizeof(asm_val_inc) / sizeof(asm_val_inc[0]));
            break;
        case TOKEN_VAL_DEC:
            write_block(&assembly,
                        asm_val_dec,
                        sizeof(asm_val_dec) / sizeof(asm_val_dec[0]));
            break;
        case TOKEN_WRITE:
            write_block(&assembly,
                        asm_write,
                        sizeof(asm_write) / sizeof(asm_write[0]));
            break;
        case TOKEN_READ:
            write_block(&assembly,
                        asm_read,
                        sizeof(asm_read) / sizeof(asm_read[0]));
            break;
        case TOKEN_JMP_INC:
            // Create a new subroutine
            break;
        case TOKEN_JMP_DEC:
            // Jump to the beginning of the subroutine if looped
            break;
        default:
            break;
        }
    }

    write_block(&assembly, asm_foot, sizeof(asm_foot) / sizeof(asm_foot[0]));
    return assembly;
}