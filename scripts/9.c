#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);

    osrmsFile* f = os_open(0, "escritura.txt", 'r');
    if (!f) {
        printf("[ERROR] Archivo no encontrado para lectura.\n");
        return 1;
    }

    int leidos = os_read_file(f, "lectura_individual.txt");
    printf("[4] Bytes leídos: %d\n", leidos);
    os_close(f);

    return 0;
}
