#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);

    const char* nombre = "escritura.txt";

    osrmsFile* f = os_open(0, nombre, 'r');
    if (!f) {
        printf("[ERROR] No se encontró '%s' para eliminar.\n", nombre);
        return 1;
    }

    os_delete_file(0, nombre);
    os_close(f);

    return 0;
}
