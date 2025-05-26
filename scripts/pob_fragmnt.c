#include <stdio.h>	
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// con memformat.bin clean

// Crear un archivo local de texto con contenido repetido
void crear_archivo_local(const char* path, const char* contenido, int repeticiones) {
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

    int pid = 1; // Debe ser PID libre
    if (os_start_process(pid, "poblador") != 0) {
        printf("No se pudo crear el proceso %d\n", pid);
        os_unmount();
        return 1;
    }

    printf("[1] Poblando memoria con archivos pequeños...\n");
    // Crea archivos pequeños para ocupar frames dispersos
    for (int i = 0; i < 6; i++) {
        char nombre_archivo[32], nombre_logico[32];
        sprintf(nombre_archivo, "tiny_%d.txt", i);
        sprintf(nombre_logico, "file_%d.txt", i);
        crear_archivo_local(nombre_archivo, "pequeño\n", 1);

        osrmsFile* f = os_open(pid, nombre_logico, 'w');
        if (!f) {
            printf("Error: os_open falló para %s\n", nombre_logico);
            continue;
        }
        int escritos = os_write_file(f, nombre_archivo);
        printf("    ↳ %s: %d bytes escritos\n", nombre_logico, escritos);
        os_close(f);
    }

    // Opcional: Borra algunos archivos pequeños para dejar huecos (fragmentación)
    printf("[2] Borrando algunos archivos pequeños para crear huecos...\n");
    os_delete_file(pid, "file_1.txt");
    os_delete_file(pid, "file_3.txt");
    os_delete_file(pid, "file_5.txt");

    // Crea un archivo grande que debería quedar fragmentado
    printf("[3] Creando archivo grande para ocupar frames fragmentados...\n");
    crear_archivo_local("grande.txt", "linea grande para poblar memoria...\n", 300);

    osrmsFile* fbig = os_open(pid, "archivo_grande.txt", 'w');
    if (!fbig) {
        printf("Error: os_open falló para archivo grande\n");
    } else {
        int escritos = os_write_file(fbig, "grande.txt");
        printf("    ↳ Se escribieron %d bytes en 'archivo_grande.txt'\n", escritos);
        os_close(fbig);
    }

    printf("\n=== Procesos activos ===\n");
    os_ls_processes();

    printf("\n=== Archivos por proceso ===\n");
    os_ls_files(pid);

    os_unmount();
    return 0;
}