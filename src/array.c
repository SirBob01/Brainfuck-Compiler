#include "./array.h"

array_t array_create(unsigned type_size) {
    array_t array;
    array.size = 0;
    array.capacity = 2;
    array.type_size = type_size;

    // Allocate the buffer
    array.buffer = malloc(array.capacity * array.type_size);
    assert(array.buffer != NULL);
    return array;
}

void array_destroy(array_t *array) { free(array->buffer); }

void array_push(array_t *array, void *src) {
    // Resize the buffer if necessary
    if (array->size == array->capacity) {
        array->capacity *= 2;
        array->buffer =
            realloc(array->buffer, array->capacity * array->type_size);
        assert(array->buffer != NULL);
    }

    // Copy the data into the buffer
    unsigned offset = array->size * array->type_size;
    memcpy(array->buffer + offset, src, array->type_size);
    array->size++;
}

void array_get(array_t *array, unsigned index, void *dst) {
    assert(index < array->size);
    unsigned offset = index * array->type_size;
    memcpy(dst, array->buffer + offset, array->type_size);
}
