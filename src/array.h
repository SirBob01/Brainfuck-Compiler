#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Dynamically sized array.
 *
 */
typedef struct {
    unsigned size;
    unsigned capacity;
    unsigned type_size;
    void *buffer;
} array_t;

/**
 * @brief Create a new array.
 *
 * @param type_size
 * @return array_t
 */
array_t array_create(unsigned type_size);

/**
 * @brief Destroy an array.
 *
 * @param array
 */
void array_destroy(array_t *array);

/**
 * @brief Push an item to the back of the array.
 *
 * @param array
 * @param src
 */
void array_push(array_t *array, void *src);

/**
 * @brief Get an item from the array.
 *
 * @param array
 * @param index
 */
void *array_get(array_t *array, unsigned index);

/**
 * @brief Pop an item from the back of the array.
 *
 * @param array
 */
void array_pop(array_t *array);

/**
 * @brief Get the last item in the array.
 *
 * @param array
 * @return void*
 */
void *array_back(array_t *array);

#endif