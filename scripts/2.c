#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);

// Prueba de las 5 funciones de archivos
    os_mount("memoria.bin");

    // 1. Crear archivo (w)
    osrmsFile* f = os_open(1, "archivo.txt", 'w');

    // 2. Escribir archivo
    os_write_file(f, "entrada.txt"); // entrada.txt debe existir

    // 3. Cerrar archivo
    os_close(f);

    // 4. Abrir archivo (r)
    osrmsFile* f2 = os_open(1, "archivo.txt", 'r');

    // 5. Leer archivo
    os_read_file(f2, "salida.txt");

    // Cerrar archivo
    os_close(f2);

    // Eliminar archivo
    os_delete_file(1, "archivo.txt");
}