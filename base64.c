#include <stdlib.h>
#include <string.h>
#include "base64.h"

/*
 * Estimate the length of an output based on the length of the input.
 *
 * For example we're wanting to go from ASCII to Base64 so that means that we're essentially
 *  wanting to go from 8-bit wide chunks to 6-bit wide chunks.
 * 
 * This is important for when we're breaking down a stream of data into their respective
 *  chunks and when we allocate memory to a pointer we need to know how much data we're
 *  needing to allocate beforehand.
 */
#define ESTIMATE(in, fr, to) (((in * fr) % to) != 0) + ((in * fr) / to)

/*
 * When we're decoding Base64 inputs we need to be able to get the index of each character
 *  in the Base64 character set.
 * 
 * This function should return the index of each character in the Base64 character set.
 */
int base64_ascii(const char input) {
    if ('A' <= input && input <= 'Z')                                                          // Is the current character between 'A' and 'Z'?
        return input - 'A';                                                                    // Grab the relative index of the Base64 uppercase character.
    
    if ('a' <= input && input <= 'z')                                                          // Is the current character between 'a' and 'z'?
        return (input - 'a') + 26;                                                             // Grab the relative index of the Base64 lowercase character.
    
    if ('0' <= input && input <= '9')                                                          // Is the current character between '0' and '9'?
        return (input - '0') + 52;                                                             // Grab the relative index of the Base64 numerical character.
    
    if (input == '+') return 62;                                                               // Simply return the Base64 charset index of the plus sign.
    if (input == '/') return 63;                                                               // Simply return the Base64 charset index of the backslash sign.
    return input;                                                                              // This should not be accessible under normal conditions.
}

char *base64_encode(const char *input) {
    size_t length = ESTIMATE(strlen(input), 8, 6);                                             // Calculate the estimated length of the output string.
    char *output = (char *) calloc(length, 1);                                                 // Allocate the output string with zeroes or NULL bytes.
    size_t idx = 0;                                                                            // Store the current location of the output string.

    while (idx < length) {
        char b1 = *input++;                                                                    // Grab the 1st character from the input string.
        char b2 = *input++;                                                                    // Grab the 2nd character from the input string.
        char b3 = *input++;                                                                    // Grab the 3rd character from the input string.

        unsigned buffer = b1 << 16 | b2 << 8 | b3;                                             // Combine our bytes into a larger buffer to break down.

                                                                                               // Undefined behavior warning: idx is accessed after being modified.
        *(output + idx++) = BASE64_CHARSET[(buffer & 0xFC0000) >> 18];                         // strlen(BASE64_CHARSET) << 18 = 0xFC0000
        *(output + idx++) = (idx >= length) ? '=' : BASE64_CHARSET[(buffer & 0x3F000) >> 12];  // strlen(BASE64_CHARSET) << 12 = 0x3F000
        *(output + idx++) = (idx >= length) ? '=' : BASE64_CHARSET[(buffer & 0xFC0) >> 6];     // strlen(BASE64_CHARSET) <<  6 = 0xFC0
        *(output + idx++) = (idx >= length) ? '=' : BASE64_CHARSET[(buffer & 0x3F)];           // strlen(BASE64_CHARSET)       = 0x3F
    };

    return output;
}

char *base64_decode(const char *input) {
    size_t length = ESTIMATE(strlen(input), 6, 8);                                             // Calculate the estimated length of the output string.
    char *output = (char *) calloc(length, 1);                                                 // Allocate the output string with zeroes or NULL bytes.
    size_t idx = 0;                                                                            // Store the current location of the output string.

    while (idx < length) {
        int i1 = base64_ascii(*input++);                                                       // Get the Base64 character set index for the 1st character.
        int i2 = base64_ascii(*input++);                                                       // Get the Base64 character set index for the 2nd character.
        char c3 = *input++;                                                                    // Grab the 3rd character from the input string.
        char c4 = *input++;                                                                    // Grab the 4th character from the input string.

        *(output + idx++) = (i1 << 2) | (i2 >> 4);                                             // Grab the last 6 bits from the 1st character and
                                                                                               //  append the first 2 bits from the 2nd character.

        if (c3 != '=') {                                                                       // Is the 3rd character NOT a Base64 padding character?
            int i3 = base64_ascii(c3);                                                         // Get the Base64 character set index for the 3rd character.
            *(output + idx++) = ((i2 & 0xF) << 4) + (i3 >> 2);                                 // Grab the last 4 bits from the 2nd character and append
                                                                                               //  the first 4 bits from the 3rd character.

            if (c4 != '=') {                                                                   // Is the 4th character NOT a Base64 padding character?
                int i4 = base64_ascii(c4);                                                     // Get the Base64 character set index for the 4th character.
                *(output + idx++) = ((i3 & 0x3) << 6) | i4;                                    // Grab the last 2 bits from the 3rd character and 
            }                                                                                  //  append the entire 4th character.
        }
    }

    return output;
}
