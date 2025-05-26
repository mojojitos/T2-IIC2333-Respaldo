#include <stdio.h>	
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// con memformat.bin clean

int main(int argc, char const *argv[]){
    os_mount((char *)argv[1]);
    printf("[FRAG] Memoria montada.\n");

    // Llenar algunos frames intermedios
    for (int i = 0; i < 5; i++) {
        char nombre[32];
        sprintf(nombre, "temp_%d.txt", i);
        crear_demo(nombre, "Hola!\n");

        char nombre_archivo[32];
        sprintf(nombre_archivo, "filler_%d.txt", i);
        osrmsFile* temp = os_open(0, nombre_archivo, 'w');
        os_write_file(temp, nombre);
        os_close(temp);
        printf("[FRAG] Escrito %s\n", nombre_archivo);
    }

    // Crear archivo grande que se fragmentará
    crear_demo("grande.txt", "Este es un archivo más grande que ocupará varias páginas.\n"
                             "Cada línea está diseñada para llenar múltiples páginas virtuales...\n"
                             "Repetiremos líneas...\n"
                             "Repetiremos líneas...\n"
                             "Repetiremos líneas...\n"
                             "Repetiremos líneas...\n"
                             "Repetiremos líneas...\n"
                             "Repetiremos líneas...\n");

    osrmsFile* grande = os_open(0, "fragmentado.txt", 'w');
    int escritos = os_write_file(grande, "grande.txt");
    printf("[FRAG] Archivo grande escrito: %d bytes\n", escritos);
    os_close(grande);

    // Leer el archivo fragmentado
    grande = os_open(0, "fragmentado.txt", 'r');
    os_read_file(grande, "fragmentado_copiado.txt");
    os_close(grande);

    printf("[FRAG] Lectura completa. Revisa 'fragmentado_copiado.txt'\n");
    printf("[FRAG] Puedes ahora usar 'xxd memoria.bin | less' y buscar el bitmap (offset 0x32000)\n");

    os_unmount();

    return 0;
}
