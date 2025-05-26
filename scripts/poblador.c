#include <stdio.h>	
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// con memformat.bin clean

void crear_archivo(const char* path, const char* contenido, int repeticiones) {
    FILE* f = fopen(path, "w");
    if (f) {
        for (int i = 0; i < repeticiones; i++) {
            fputs(contenido, f);
        }
        fclose(f);
    }
}

int main(int argc, char const *argv[]) {
    os_mount((char *)argv[1]);
    printf("=== [POBLAR MEMORIA] ===\n");

    for (int i = 0; i < 10; i++) {
        char nombre_archivo[64];
        char nombre_logico[64];
        sprintf(nombre_archivo, "tiny_%d.txt", i);
        sprintf(nombre_logico, "file_%d.txt", i);
        crear_archivo(nombre_archivo, "pequeño\n", 1);

        osrmsFile* f = os_open(0, nombre_logico, 'w');
        os_write_file(f, nombre_archivo);
        os_close(f);
    }

    for (int i = 0; i < 3; i++) {
        char nombre_archivo[64];
        char nombre_logico[64];
        sprintf(nombre_archivo, "grande_%d.txt", i);
        sprintf(nombre_logico, "bigfile_%d.txt", i);
        crear_archivo(nombre_archivo, "linea grande para poblar memoria...\n", 200);

        osrmsFile* f = os_open(0, nombre_logico, 'w');
        os_write_file(f, nombre_archivo);
        os_close(f);
    }

    printf("=== [FIN POBLAR MEMORIA] ===\n");
    os_unmount();
    return 0;
}