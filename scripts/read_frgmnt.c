#include <stdio.h>	
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// ejecutar con memformat.bin luego de ejecutar pob_fragmnt.c

int main(int argc, char const *argv[]) {
    os_mount((char *)argv[1]);

    int pid = 1;

    const char* archivos[] = {
        "file_0.txt",
        "file_2.txt",
        "file_4.txt",
        "archivo_grande.txt"
    };
    int num_archivos = sizeof(archivos) / sizeof(archivos[0]);

    for (int i = 0; i < num_archivos; i++) {
        printf("Leyendo archivo lógico '%s' del proceso %d...\n", archivos[i], pid);
        osrmsFile* f = os_open(pid, (char*)archivos[i], 'r');
        if (!f) {
            printf("  No se pudo abrir '%s'\n", archivos[i]);
            continue;
        }
        char nombre_copia[64];
        snprintf(nombre_copia, sizeof(nombre_copia), "copia_%s", archivos[i]);
        int leidos = os_read_file(f, nombre_copia);
        printf("  Se leyeron %d bytes y se guardó como '%s'\n", leidos, nombre_copia);
        os_close(f);
    }

    return 0;
}
