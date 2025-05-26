#include <stdio.h>	
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// con memformat.bin clean

int main(int argc, char const *argv[]){
    os_mount((char *)argv[1]);

    // Intentar abrir archivo en proceso inexistente
    osrmsFile* f = os_open(99, "noexiste.txt", 'r');
    if (!f) printf("OK: No se puede abrir archivo en proceso inexistente\n");

    // Crear proceso y abrir archivo en modo 'w'
    os_start_process(10, "proc10");
    osrmsFile* f2 = os_open(10, "a.txt", 'w');
    if (!f2) printf("ERROR: No se pudo crear archivo\n");

    // Intentar abrir el mismo archivo en modo 'w'
    osrmsFile* f3 = os_open(10, "a.txt", 'w');
    if (!f3) printf("OK: No se puede crear dos veces el mismo archivo\n");

    // Escribir archivo inexistente 
    int res = os_write_file(NULL, "entrada.txt");
    if (res == -1) printf("OK: No se puede escribir en archivo NULL\n");

    // Leer archivo inexistente 
    res = os_read_file(NULL, "salida.txt");
    if (res == -1) printf("OK: No se puede leer archivo NULL\n");

    // Cerrar archivo NULL
    os_close(NULL);

    // Eliminar archivo que no existe
    os_delete_file(10, "noexiste.txt");

    // Eliminar archivo válido
    os_delete_file(10, "a.txt");

    // Eliminar archivo dos veces
    os_delete_file(10, "a.txt");

    // Terminar proceso inexistente
    os_finish_process(99);

    // Terminar proceso válido
    os_finish_process(10);

    os_unmount();

    return 0;
}