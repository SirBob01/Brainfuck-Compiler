#ifndef BFC_H
#define BFC_H

#define COMMAND_FLAG_FILE   "-i"
#define COMMAND_FLAG_CODE   "-c"
#define COMMAND_FLAG_OUTPUT "-o"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./parse.h"
#include "./string.h"

/**
 * @brief Input data
 *
 */
typedef struct input {
    char *source;
    FILE *target;
} input_t;

/**
 * @brief Print usage information.
 *
 */
void print_usage();

/**
 * @brief Print error message.
 *
 */
void print_error(const char *message);

/**
 * @brief Create a heap string from the contents of a file.
 *
 * @param path
 * @return char*
 */
char *open_file(char *path);

/**
 * @brief Parse input commands and get the source code.
 *
 * @param argc
 * @param argv
 * @return input_t
 */
input_t read_source(int argc, char **argv);

#endif