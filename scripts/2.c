#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../osms_API/osms_API.h"

void crear_archivo(const char* path, const char* contenido) {
    FILE* f = fopen(path, "w");
    if (f) {
        fputs(contenido, f);
        fclose(f);
    }
}

int main() {
    os_mount("memoria.bin");
    printf("=== [TEST FRAGMENTACIÓN] ===\n");

    printf("[1] Fragmentando memoria con archivos pequeños...\n");

    for (int i = 0; i < 10; i++) {
        char nombre_archivo[64];
        char nombre_logico[64];
        sprintf(nombre_archivo, "tiny_%d.txt", i);
        sprintf(nombre_logico, "file_%d.txt", i);
        crear_archivo(nombre_archivo, "soy un filler\n");

        osrmsFile* f = os_open(0, nombre_logico, 'w');
        int escritos = os_write_file(f, nombre_archivo);
        printf("    ↳ %s: %d bytes escritos\n", nombre_logico, escritos);
        os_close(f);
    }

    printf("[2] Generando archivo grande para forzar fragmentación...\n");

    FILE* g = fopen("grande.txt", "w");
    for (int i = 0; i < 100; i++) {
        fprintf(g, "Línea larga #%d: este contenido llenará muchas páginas.\n", i);
    }
    fclose(g);

    osrmsFile* big = os_open(0, "fragmentado.txt", 'w');
    int bytes_escritos = os_write_file(big, "grande.txt");
    printf("    ↳ Se escribieron %d bytes en 'fragmentado.txt'\n", bytes_escritos);


    printf("[3] Leyendo archivo fragmentado desde memoria...\n");
    os_read_file(big, "copia_fragmentada.txt");
    os_close(big);


    printf("[4] Comparando archivo copiado con original...\n");
    int cmp = system("diff grande.txt copia_fragmentada.txt");
    if (cmp == 0)
        printf("Archivos son idénticos.\n");
    else
        printf("Archivos son distintos.\n");

    printf("[5] Bitmap de frames (se esperaría que haya huecos):\n");
   

    printf("=== [FIN TEST FRAGMENTACIÓN] ===\n");
    return 0;
}
