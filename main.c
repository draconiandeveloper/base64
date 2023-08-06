#include <stdio.h>
#include "base64.h"

int main(int argc, char **argv) {
	if (argc != 2) return -1;
	**argv++;
	
    char *encoded = base64_encode(*argv);
    char *decoded = base64_decode(encoded);
    
    printf("Encoded: %s\nDecoded: %s\n", encoded, decoded);
    return 0;
}