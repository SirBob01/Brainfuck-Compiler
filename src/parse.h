#ifndef PARSE_H
#define PARSE_H

/**
 * @brief Token definitions
 *
 */
#define TOKEN_PTR_INC     '>'
#define TOKEN_PTR_DEC     '<'
#define TOKEN_VAL_INC     '+'
#define TOKEN_VAL_DEC     '-'
#define TOKEN_WRITE       '.'
#define TOKEN_READ        ','
#define TOKEN_BLOCK_OPEN  '['
#define TOKEN_BLOCK_CLOSE ']'

#include <stdlib.h>
#include <string.h>

#include "./array.h"
#include "./string.h"

/**
 * @brief Code snippets to assemble.
 *
 * Note: r12 - r15 are callee-saved registers (persistent)
 * http://6.s081.scripts.mit.edu/sp18/x86-64-architecture-guide.html
 *
 * r12 - Buffer offset
 * r13 - Buffer capacity
 * r14 - Buffer pointer
 */
static const char *main_header[] = {
    "extern _malloc, _free, _putchar, _getchar",
    "section .text",
    "global _main",
    "",
    "_write:",
    "    mov dil, byte [r14 + r12]", // Load value
    "    call _putchar",             // Write value
    "    ret",
    "",
    "_read:",
    "    call _getchar",            // Read value
    "    mov byte [r14 + r12], al", // Store value
    "",
    "_main:",
    "    mov r12, 0",     // Buffer offset
    "    mov r13, 32768", // Buffer capacity
    "",
    "    mov rdi, r13", // Allocate buffer
    "    call _malloc",
    "    mov r14, rax",
    "",
    "    jmp _block0", // Call the first block
};
static const char *main_footer[] = {
    "    mov rdi, r14", // Free the buffer
    "    call _free",
    "    ret",
};
static const char *asm_ptr_inc = "    inc r12";
static const char *asm_ptr_dec = "    dec r12";
static const char *asm_val_inc = "    inc byte [r14 + r12]";
static const char *asm_val_dec = "    dec byte [r14 + r12]";
static const char *asm_write = "    call _write";
static const char *asm_read = "    call _read";
static const char *asm_cmp = "    cmp byte [r14 + r12], 0";

/**
 * @brief Translate to assembly.
 *
 * @param source
 * @return string_t
 */
string_t parse(char *source);

/**
 * @brief Validate the source code to ensure that all brackets are balanced.
 *
 * @param source
 * @return true
 * @return false
 */
bool validate(char *source);

#endif