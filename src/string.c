#include "./string.h"

string_t string_create() {
    string_t string;
    string.length = 0;
    string.capacity = 4;

    // Allocate the buffer
    string.buffer = malloc(string.capacity);
    assert(string.buffer != NULL);
    return string;
}

void string_destroy(string_t *string) { free(string->buffer); }

void string_push(string_t *string, char c) {
    // Resize the buffer if necessary
    if (string->length + 1 >= string->capacity) {
        string->capacity *= 2;
        string->buffer = realloc(string->buffer, string->capacity);
        assert(string->buffer != NULL);
    }

    // Copy the char into the buffer
    string->buffer[string->length++] = c;
    string->buffer[string->length] = 0;
}

void string_concat(string_t *string, const char *str) {
    // Resize the buffer if necessary
    unsigned append_length = strlen(str);
    if (string->length + append_length + 1 >= string->capacity) {
        string->capacity = (string->length + append_length) * 2 + 1;
        string->buffer = realloc(string->buffer, string->capacity);
        assert(string->buffer != NULL);
    }

    // Copy the char into the buffer
    strncat(string->buffer, str, append_length);
    string->length += append_length;
}

void string_set(string_t *string, unsigned index, char c) {
    assert(index < string->length);
    string->buffer[index] = c;
}

char string_get(string_t *string, unsigned index) {
    assert(index < string->length);
    return string->buffer[index];
}
