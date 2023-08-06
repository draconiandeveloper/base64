/**
 * Project: Shin Megami Tensei III - Nocturne HD Save Editor
 * Author: Dracovian
 * 
 * Implement Base64 encoding and decoding.
 */

#ifndef __SAVE_EDITOR_BASE64_H
#define __SAVE_EDITOR_BASE64_H

/*
 *                  : The Base64 Alphabet (RFC-4648) Table :
 * +-------+---------+-------+---------+-------+---------+-------+---------+
 * | Char  | Binary  | Char  | Binary  | Char  | Binary  | Char  | Binary  |
 * +-------+---------+-------+---------+-------+---------+-------+---------+
 * |   A   | 0000000 |   Q   | 0010000 |   g   | 0100000 |   w   | 0110000 |
 * |   B   | 0000001 |   R   | 0010001 |   h   | 0100001 |   x   | 0110001 |
 * |   C   | 0000010 |   S   | 0010010 |   i   | 0100010 |   y   | 0110010 |
 * |   D   | 0000011 |   T   | 0010011 |   j   | 0100011 |   z   | 0110011 |
 * |   E   | 0000100 |   U   | 0010100 |   k   | 0100100 |   0   | 0110100 |
 * |   F   | 0000101 |   V   | 0010101 |   l   | 0100101 |   1   | 0110101 |
 * |   G   | 0000110 |   W   | 0010110 |   m   | 0100110 |   2   | 0110110 |
 * |   H   | 0000111 |   X   | 0010111 |   n   | 0100111 |   3   | 0110111 |
 * |   I   | 0001000 |   Y   | 0011000 |   o   | 0101000 |   4   | 0111000 |
 * |   J   | 0001001 |   Z   | 0011001 |   p   | 0101001 |   5   | 0111001 |
 * |   K   | 0001010 |   a   | 0011010 |   q   | 0101010 |   6   | 0111010 |
 * |   L   | 0001011 |   b   | 0011011 |   r   | 0101011 |   7   | 0111011 |
 * |   M   | 0001100 |   c   | 0011100 |   s   | 0101100 |   8   | 0111100 |
 * |   N   | 0001101 |   d   | 0011101 |   t   | 0101101 |   9   | 0111101 |
 * |   O   | 0001110 |   e   | 0011110 |   u   | 0101110 |   +   | 0111110 |
 * |   P   | 0001111 |   f   | 0011111 |   v   | 0101111 |   /   | 0111111 |
 * +-------+---------+-------+---------+-------+---------+-------+---------+
 * 
 *                  : The Truncated ASCII Alphabet Table :
 * +-------+---------+-----------------+-------+---------+-------+---------+
 * | Char  | Binary  | Char  | Binary  | Char  | Binary  | Char  | Binary  |
 * +-------+---------+-------+---------+-------+---------+-------+---------+
 * |  SPA  | 0100000 |   8   | 0111000 |   P   | 1010000 |   h   | 1101000 |
 * |   !   | 0100001 |   9   | 0111001 |   Q   | 1010001 |   i   | 1101001 |
 * |   "   | 0100010 |   :   | 0111010 |   R   | 1010010 |   j   | 1101010 |
 * |   #   | 0100011 |   ;   | 0111011 |   S   | 1010011 |   k   | 1101011 |
 * |   $   | 0100100 |   <   | 0111100 |   T   | 1010100 |   l   | 1101100 |
 * |   %   | 0100101 |   =   | 0111101 |   U   | 1010101 |   m   | 1101101 |
 * |   &   | 0100110 |   >   | 0111110 |   V   | 1010110 |   n   | 1101110 |
 * |   '   | 0100111 |   ?   | 0111111 |   W   | 1010111 |   o   | 1101111 |
 * |   (   | 0101000 |   @   | 1000000 |   X   | 1011000 |   p   | 1110000 |
 * |   )   | 0101001 |   A   | 1000001 |   Y   | 1011001 |   q   | 1110001 |
 * |   *   | 0101010 |   B   | 1000010 |   Z   | 1011010 |   r   | 1110010 |
 * |   +   | 0101011 |   C   | 1000011 |   [   | 1011011 |   s   | 1110011 |
 * |   ,   | 0101100 |   D   | 1000100 |   \   | 1011100 |   t   | 1110100 |
 * |   -   | 0101101 |   E   | 1000101 |   ]   | 1011101 |   u   | 1110101 |
 * |   .   | 0101110 |   F   | 1000110 |   ^   | 1011110 |   v   | 1110110 |
 * |   /   | 0101111 |   G   | 1000111 |   _   | 1011111 |   w   | 1110111 |
 * |   0   | 0110000 |   H   | 1001000 |   `   | 1100000 |   x   | 1111000 |
 * |   1   | 0110001 |   I   | 1001001 |   a   | 1100001 |   y   | 1111001 |
 * |   2   | 0110010 |   J   | 1001010 |   b   | 1100010 |   z   | 1111010 |
 * |   3   | 0110011 |   K   | 1001011 |   c   | 1100011 |   {   | 1111011 |
 * |   4   | 0110100 |   L   | 1001100 |   d   | 1100100 |   |   | 1111100 |
 * |   5   | 0110101 |   M   | 1001101 |   e   | 1100101 |   }   | 1111101 |
 * |   6   | 0110110 |   N   | 1001110 |   f   | 1100110 |   ~   | 1111110 |
 * |   7   | 0110111 |   O   | 1001111 |   g   | 1100111 |  DEL  | 1111111 |
 * +-------+---------+-------+---------+-------+---------+-------+---------+
 */
static const char BASE64_CHARSET[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

/*
 * When encoding data we're taking a stream of bits and breaking it down
 *  from byte-width (8 bits) to a width of 6 bits and then associating
 *  the 6-bit chunks with the Base64 alphabet table above.
 * 
 *                            01010100 (T)
 *                            01100101 (e)
 *                            01110011 (s)
 *                            01110100 (t)
 * 
 *                       8-bit (byte) boundaries
 *                01010100|01100101|01110011|01110100
 * 
 *          6-bit (Base64) boundaries (with 4 bits of padding)
 *              010101|000110|010101|110011|011101|00 0000
 * 
 *  Associating the bits of the 6-bit boundaries with the Base64 table
 *   we can gather the following characters for the encoded version...
 * 
 *                            010101 (V)
 *                            000110 (G)
 *                            010101 (V)
 *                            110011 (z)
 *                            011101 (d)
 *                            000000 (A)
 * 
 *  When we apply padding ('=') we do so to make the Base64 encoded
 *   string of a length that is evenly divisible by 4.
 *  
 *  So in this case we have 6 Base64 characters and as such we must
 *   add two more characters to have 8 characters so we add two pad
 *   characters at the end and that should be our encoded string.
 * 
 *                         Test -> VGVzdA==
 */
char *base64_encode(const char *);

/*
 * Decoding a Base64 string starts by removing the padding from our
 *  encoded string.
 * 
 *                       VGVzdA== -> VGVzdA
 * 
 * From there we can break down the characters into their binary
 *  representation according to the Base64 alphabet table.
 * 
 *                          010101 (V)
 *                          000110 (G)
 *                          010101 (V)
 *                          110011 (z)
 *                          011101 (d)
 *                          000000 (A)
 * 
 * From there we can string together the bits and then break them
 *  down into 1 byte (8 bits) wide segments:
 * 
 * Since the number of bits in our Base64 encoded string is not
 *  evenly divisible by 8 we must pad out the end with zeroes
 *  until we reach a length that is evenly divisible by 8.
 * 
 *         01010100|01100101|01110011|01110100|0000 0000
 * 
 * Now we can start associating each of the chunks with the ASCII
 *  table above to retrieve our decoded string.
 * 
 *                         01010100 (T)
 *                         01100101 (e)
 *                         01110011 (s)
 *                         01110100 (t)
 *                         00000000 (NUL)
 * 
 * If we ignore any control characters in our decoded output then
 *  we should end up with something that resembles our original
 *  input prior to encoding.
 */
char *base64_decode(const char *);

#endif
