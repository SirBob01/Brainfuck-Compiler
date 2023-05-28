#ifndef STRING_H
#define STRING_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Dynamically sized string
 *
 */
typedef struct {
    unsigned length;
    unsigned capacity;
    char *buffer;
} string_t;

/**
 * @brief Create a dynamic string.
 *
 * @return string_t
 */
string_t string_create();

/**
 * @brief Destroy a dynamic string.
 *
 * @param string
 */
void string_destroy(string_t *string);

/**
 * @brief Push a character onto the back of the string.
 *
 * @param string
 * @param c
 */
void string_push(string_t *string, char c);

/**
 * @brief Concatenate a string onto the back of the string.
 *
 * @param string
 * @param str
 */
void string_concat(string_t *string, const char *str);

#endif