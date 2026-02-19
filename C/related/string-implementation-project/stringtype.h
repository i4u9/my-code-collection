#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>


typedef struct String
{
    char *buffer;
    uint64_t size;
} String;


String create_string(char* content);
String create_string_s(uint64_t size);
void*  copy_string(String* dest_string, const String src_string, uint64_t n);
void*  append_string(String* dest_string, const String src_string, uint64_t n);
void*  insert_string(String* dest_string, const String src_string, uint64_t targ_index);
void   toupper_string(String* string);
void   tolower_string(String* string);
void   destroy_string(String* string);

bool   check_valid_string(uint64_t stringc, ...);
bool   reallocate_string(String* targ_string, uint64_t reallocation_size, uint64_t newsize);


// allocate a char buffer on the HEAP segment and determines it's size based on a pre-inserted string
String create_string(char* content)
{
    String result = {0};

    result.size = strlen(content);
    result.buffer = (char*)calloc(result.size + 1, sizeof(char));
    if (!result.buffer)
    {
        fprintf(stderr, "strtype error: during string buffer allocation\n");
        return (String){0};
    }
    memcpy(result.buffer, content, result.size);

    return result;
}

// allocate a char buffer on the HEAP segment and determines it's size returning a buffer with no content
String create_string_s(uint64_t size)
{
    String result = {0};

    result.size = size;
    result.buffer = (char*)calloc(result.size + 1, sizeof(char));
    if (!result.buffer)
    {
        fprintf(stderr, "strtype error: during string buffer allocation\n");
        return (String){0};
    }

    return result;
}

// completely copy n characters from src to dest, so it can shrink or expand dest
void* copy_string(String* dest_string, const String src_string, uint64_t n)
{
    uint64_t stringc = 2;
    if (!check_valid_string(stringc, *dest_string, src_string)) return NULL;

    if (n > src_string.size)
    {
        fprintf(stderr, "strtype error: trying to copy charcters outside the src string bound\n");
        return NULL;
    }

    if (dest_string->size != n)
    {
        if (!reallocate_string(dest_string, sizeof(char) * (n + 1), n)) return NULL;
        dest_string->buffer[n] = '\0';
    }    

    for (uint64_t i = 0; i < dest_string->size; i++)
    {
        dest_string->buffer[i] = src_string.buffer[i];
    }
}

// appends n characters from src to dest, so it expands dest buffer
void*  append_string(String* dest_string, const String src_string, uint64_t n)
{
    uint64_t stringc = 2;
    if (!check_valid_string(stringc, *dest_string, src_string)) return NULL;

    uint64_t last_index = dest_string->size;

    if (!reallocate_string(dest_string, sizeof(char) * ((dest_string->size + 1) + n), n + dest_string->size)) return NULL;

    for (uint64_t i = last_index; i < dest_string->size; i++)
    {
        dest_string->buffer[i] = src_string.buffer[i - last_index];
    }
}

// takes an index of dest and switch the character in there to the src string content
void* insert_string(String* dest_string, const String src_string, uint64_t targ_index)
{
    uint64_t stringc = 2;
    if (!check_valid_string(stringc, *dest_string, src_string)) return NULL;

    if (targ_index > dest_string->size - 1)
    {
        fprintf(stderr, "strtype error: trying to access an index out of dest boundary\n");
        return NULL;
    }
    
    String dest_cpy = create_string(dest_string->buffer);

    if (!reallocate_string(dest_string, dest_string->size + src_string.size, dest_string->size + src_string.size - 1)) return NULL;

    // for (uint64_t i = dest_string->size - 1; dest_cpy.size - 1 + i - dest_string->size + 1 != targ_index; i--)
    // {
    //     // dest_cpy.size - 1 + i - dest_string->size + 1
    //     dest_string->buffer[i] = dest_cpy.buffer[dest_cpy.size + i - dest_string->size];
    // }
    
    // for (uint64_t i = targ_index; i - targ_index < src_string.size; i++)
    // {
    //     dest_string->buffer[i] = src_string.buffer[i - targ_index];
    // }

    for (uint64_t i = 0; i < src_string.size; i++)
    {
        dest_string->buffer[i + targ_index] = src_string.buffer[i];
    }

    for (uint64_t i = targ_index + src_string.size; i < dest_string->size; i++)
    {
        dest_string->buffer[i] = dest_cpy.buffer[i - src_string.size + 1];
    }
    
}

void toupper_string(String* string)
{
    if (!check_valid_string(1, *string)) return;

    for (uint64_t i = 0; i < string->size; i++)
    {
        if (string->buffer[i] >= 97 && string->buffer[i] <= 122)
        {
            string->buffer[i] -= 32;
        }
    }
}

void tolower_string(String* string)
{
    if (!check_valid_string(1, *string)) return;

    for (uint64_t i = 0; i < string->size; i++)
    {
        if (string->buffer[i] >= 65 && string->buffer[i] <= 90)
        {
            string->buffer[i] += 32;
        }
    }
}

void destroy_string(String* string)
{
    string->size = 0;
    free(string->buffer);
    string->buffer = NULL;
}

bool check_valid_string(uint64_t stringc, ...)
{
    va_list strings;
    va_start(strings, stringc);

    for (uint64_t i = 0; i < stringc; i++)
    {
        String s = va_arg(strings, String);
        if (s.buffer == NULL || s.size == 0)
        {
            fprintf(stderr,"strtype error: string operation takes unintialized string as argument\n");
            va_end(strings);
            return false;
        }
    }
    
    va_end(strings);

    return true;
}

// reallocate targ_string buffer to have reallocation_size and updates the string size
// (hint: realloc_size = newsize + 1, in any case)
bool reallocate_string(String* targ_string, uint64_t reallocation_size, uint64_t newsize)
{
    void *temp = realloc(targ_string->buffer, reallocation_size * sizeof(char));
    if (!temp)
    {
        fprintf(stderr, "strtype error: error during string buffer reallocation\n");
        return false;
    }

    targ_string->size = newsize;
    targ_string->buffer = (char*)temp;
    temp = NULL;

    targ_string->buffer[targ_string->size] = '\0';

    return true;
}

#endif