#include "strutil.h"
#include <ctype.h> // For tolower()
#include <stdlib.h>
#include <string.h>

// Case-insensitive string comparison
int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        if (diff != 0) {
            return diff;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

char *strdup(const char *s) {
    if (s == NULL) return NULL;
    
    size_t len = strlen(s) + 1; // +1 for null terminator
    char *new_s = (char*)malloc(len);

    if (new_s == NULL) return NULL;
    
    memcpy(new_s, s, len); // Copy string content + null terminator
    return new_s;
}

#include <stddef.h>

char *strtok_r(char *s, const char *delim, char **saveptr)
{
    char *token;

    if (s == NULL)
        s = *saveptr;

    /* Scan leading delimiters.  */
    s += strspn(s, delim);
    if (*s == '\0')
    {
        *saveptr = s;
        return NULL;
    }

    /* Find end of token.  */
    token = s;
    s = strpbrk(token, delim);
    if (s == NULL)
    {
        /* This token finishes the string.  */
        *saveptr = strchr(token, '\0');
    }
    else
    {
        /* Terminate the token and make *SAVE_PTR point past it.  */
        *s = '\0';
        *saveptr = s + 1;
    }

    return token;
}

char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) { 
        *str = '\0'; 
        return str;
    }

    char* ptr = str;
    int tmp_value;
    
    // Handle 0 separately to simplify the loop logic
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    // Handle negative numbers for base 10
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    // Calculate the length of the number in the given base
    tmp_value = value;
    while (tmp_value) {
        tmp_value /= base;
        ptr++;
    }

    *ptr = '\0';

    // Now convert the number to string from back to front
    char* ptr1 = ptr - 1;
    do {
        *ptr1-- = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (value % base)];
        value /= base;
    } while (value);

    // Move the string to the beginning of the buffer if we wrote from the end
    strcpy(str, ptr1 + 1);

    return str;
}
