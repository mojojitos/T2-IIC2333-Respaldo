#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);

    osrmsFile* f = os_open(0, "archivo_nuevo.txt", 'w');
    if (f) {
        printf("[2] Archivo creado exitosamente:\n");
        printf("    Nombre: %s\n", f->nombre_archivo);
        printf("    Dir virtual: VPN = %d, Offset = %d\n", (f->dir_virtual >> 15) & 0xFFF, f->dir_virtual & 0x7FFF);
        os_close(f);
    } else {
        printf("[ERROR] No se pudo crear archivo\n");
    }

    return 0;
}
