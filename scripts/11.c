#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);
    osrmsFile* f = os_open(0, "archivo_temp.txt", 'w');
    if (f) {
        printf("[6] Cerrando descriptor...\n");
        os_close(f);
        printf("[6] Descriptor liberado correctamente.\n");
    } else {
        printf("[ERROR] No se pudo abrir archivo temporal.\n");
    }

    return 0;
}
