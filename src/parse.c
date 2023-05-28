#include "./parse.h"

void append_code(string_t *dst, const char *lines[], unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        string_concat(dst, lines[i]);
        string_push(dst, '\n');
    }
    string_push(dst, '\n');
}

void write_block_name(string_t *dst, unsigned block_id) {
    char name[sizeof(unsigned) * 8 + 10];
    snprintf(name, sizeof(name), "_block%d", block_id);
    string_concat(dst, name);
}

void block_create(array_t *blocks) {
    string_t block = string_create();
    write_block_name(&block, blocks->size);
    string_concat(&block, ":\n");
    array_push(blocks, &block);
}

void block_open(array_t *blocks, array_t *stack, unsigned block_id) {
    block_create(blocks);
    string_t *block = array_get(blocks, block_id);
    unsigned next_block = blocks->size - 1;

    // If current value is zero, jump to next block
    // cmp byte ptr [rbx], 0
    string_concat(block, asm_cmp);
    string_push(block, '\n');

    // je _skip_block<N>
    string_concat(block, "    je _skip");
    write_block_name(block, next_block);
    string_concat(block, "\n");

    // call _block<N>
    string_concat(block, "    call ");
    write_block_name(block, next_block);
    string_concat(block, "\n\n");

    // _skip_block<N>:
    string_concat(block, "_skip");
    write_block_name(block, next_block);
    string_concat(block, ":\n");

    // Push the stack
    array_push(stack, &next_block);
}

void block_close(array_t *blocks, array_t *stack, unsigned block_id) {
    string_t *block = array_get(blocks, block_id);

    // If current value is non-zero, jump to start of block
    string_concat(block, asm_cmp);
    string_push(block, '\n');
    string_concat(block, "    jne ");
    write_block_name(block, block_id);
    string_push(block, '\n');

    // Otherwise, return
    string_concat(block, "    ret\n\n");

    // Pop the stack
    array_pop(stack);
}

void block_append(array_t *blocks, unsigned block_id, const char *line) {
    string_t *block = array_get(blocks, block_id);

    // Append code snippet
    string_concat(block, line);
    string_concat(block, "\n\n");
}

array_t generate_blocks(char *source) {
    array_t blocks = array_create(sizeof(string_t));
    block_create(&blocks);

    array_t stack = array_create(sizeof(unsigned));
    unsigned curr_block = 0;
    array_push(&stack, &curr_block);

    unsigned n = strlen(source);
    for (unsigned i = 0; i < n; i++) {
        // printf("%d\n", curr_block);
        switch (source[i]) {
        case TOKEN_PTR_INC:
            block_append(&blocks, curr_block, asm_ptr_inc);
            break;
        case TOKEN_PTR_DEC:
            block_append(&blocks, curr_block, asm_ptr_dec);
            break;
        case TOKEN_VAL_INC:
            block_append(&blocks, curr_block, asm_val_inc);
            break;
        case TOKEN_VAL_DEC:
            block_append(&blocks, curr_block, asm_val_dec);
            break;
        case TOKEN_WRITE:
            block_append(&blocks, curr_block, asm_write);
            break;
        case TOKEN_READ:
            block_append(&blocks, curr_block, asm_read);
            break;
        case TOKEN_BLOCK_OPEN:
            block_open(&blocks, &stack, curr_block);
            curr_block = blocks.size - 1;
            break;
        case TOKEN_BLOCK_CLOSE:
            block_close(&blocks, &stack, curr_block);
            curr_block = *(unsigned *)array_back(&stack);
            break;
        default:
            break;
        }
    }

    // Close root block and destroy stack
    block_append(&blocks, 0, "    ret");
    array_destroy(&stack);
    return blocks;
}

string_t parse(char *source) {
    string_t assembly = string_create();
    append_code(&assembly,
                main_header,
                sizeof(main_header) / sizeof(main_header[0]));

    // Process tokens and create blocks
    array_t blocks = generate_blocks(source);

    // Write the footer of main subroutine
    append_code(&assembly,
                main_footer,
                sizeof(main_footer) / sizeof(main_footer[0]));

    // Append blocks to end-of-file and cleanup
    for (unsigned i = 0; i < blocks.size; i++) {
        string_t *block = array_get(&blocks, i);
        string_concat(&assembly, block->buffer);
        string_push(&assembly, '\n');
        string_destroy(block);
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
        if (balance < 0) {
            return false;
        }
    }
    return balance == 0;
}