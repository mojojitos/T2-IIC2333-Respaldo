#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{

    // montar la memoria
    os_mount((char *)argv[1]);
    os_ls_processes();

    os_exists(6, "cat.jpg");

/*     //Testeando forma de obtener el binario
    unsigned char byte = 49;
    printf("binary rep. of char byte 49: %#b \n", byte);


    // Storing binary in an integer variable
    int binary_int = 0b101101; // Binary representation of 45
    printf("Integer value: %d\n", binary_int);

    // Storing binary in an unsigned integer
    unsigned int binary_uint = 0b11110000;
     printf("Unsigned integer value: %u\n", binary_uint);

    // Storing binary in a character variable
    char binary_char = 0b01000001; // Binary representation of 'A'
    printf("Character value: %c\n", binary_char);

    // Storing binary in an unsigned character
    unsigned char binary_uchar = 0b11001100;
    printf("Unsigned character value: %d\n", binary_uchar); */

}