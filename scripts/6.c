#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);
    printf("[1] Montando memoria...\n");
    printf("[1] Memoria montada correctamente.\n");

    // Aquí podrías usar fseek() o debug manual si quieres inspeccionar offsets
    return 0;
}
