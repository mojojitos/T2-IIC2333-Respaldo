#include <stdio.h>	
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// con memformat.bin clean
// requiere archivo local entrada.txt (placeholder para otro archivo, puede cambiar ruta)

int main(int argc, char const *argv[]){
    os_mount((char *)argv[1]);

    // Crear muchos procesos
    for (int i = 1; i <= 5; i++) {
        char nombre[16];
        sprintf(nombre, "proc%d", i);
        os_start_process(i, nombre);
    }

    // Crear muchos archivos por proceso
    for (int pid = 1; pid <= 5; pid++) {
        for (int j = 0; j < 10; j++) {
            char nombre_archivo[16];
            sprintf(nombre_archivo, "file%d_%d.txt", pid, j);
            osrmsFile* f = os_open(pid, nombre_archivo, 'w');
            if (f) {
                os_write_file(f, "entrada.txt"); // Mismo archivo de entrada para todos
                os_close(f);
            }
        }
    }

    // Listar archivos de cada proceso
    for (int pid = 1; pid <= 5; pid++) {
        os_ls_files(pid);
    }

    // Eliminar archivos pares de cada proceso
    for (int pid = 1; pid <= 5; pid++) {
        for (int j = 0; j < 10; j += 2) {
            char nombre_archivo[16];
            sprintf(nombre_archivo, "file%d_%d.txt", pid, j);
            os_delete_file(pid, nombre_archivo);
        }
    }

    // Listar archivos de cada proceso después de borrar
    for (int pid = 1; pid <= 5; pid++) {
        os_ls_files(pid);
    }

    // Terminar todos los procesos
    for (int pid = 1; pid <= 5; pid++) {
        os_finish_process(pid);
    }

    os_ls_processes();

    return 0;
}