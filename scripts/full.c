#include <stdio.h>	
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h> 
#include "../osms_API/osms_API.h"

// con memformat.bin clean
// requiere archivo local entrada.txt (placeholder para otro archivo, puede cambiar ruta)

int main(int argc, char const *argv[]){
    os_mount((char *)argv[1]);

    // Crear procesos
    printf("\n== Creando procesos ==\n");
    os_start_process(1, "uno");
    os_start_process(2, "dos");
    os_start_process(3, "tres");
    os_ls_processes();

    // Renombrar proceso
    printf("\n== Renombrando proceso 2 ==\n");
    os_rename_process(2, "segundo");
    os_ls_processes();

    // Crear archivos en proceso 1
    printf("\n== Creando archivos en proceso 1 ==\n");
    for (int i = 0; i < 3; i++) {
        char nombre[16];
        sprintf(nombre, "file_%d.txt", i);
        osrmsFile* f = os_open(1, nombre, 'w');
        if (f) {
            os_write_file(f, "entrada.txt");
            os_close(f);
        }
    }
    os_ls_files(1);

    // Leer archivo de proceso 1
    printf("\n== Leyendo archivo file_1.txt de proceso 1 ==\n");
    osrmsFile* f1 = os_open(1, "file_1.txt", 'r');
    if (f1) {
        os_read_file(f1, "salida_file_1.txt");
        os_close(f1);
    }

    // Borrar archivo de proceso 1
    printf("\n== Borrando archivo file_0.txt de proceso 1 ==\n");
    os_delete_file(1, "file_0.txt");
    os_ls_files(1);

    // Crear y borrar archivos en proceso 2
    printf("\n== Creando y borrando archivos en proceso 2 ==\n");
    osrmsFile* f2 = os_open(2, "borrar.txt", 'w');
    if (f2) {
        os_write_file(f2, "entrada.txt");
        os_close(f2);
    }
    os_ls_files(2);
    os_delete_file(2, "borrar.txt");
    os_ls_files(2);

    // Terminar proceso 3
    printf("\n== Terminando proceso 3 ==\n");
    os_finish_process(3);
    os_ls_processes();

    // Mostrar bitmap de frames
    printf("\n== Bitmap de frames ==\n");
    os_frame_bitmap();

    os_unmount();
    return 0;
}