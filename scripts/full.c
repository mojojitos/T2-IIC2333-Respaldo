#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../osms_API/osms_API.h"

// con memoria/memformat.bin clean
// requiere archivos locales: memoria/archivos/prueba.txt, memoria/archivos/spongebob.mp4, memoria/archivos/cat.jpg, memoria/archivos/dolphin.md, memoria/archivos/homero.txt

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

    // Crear archivos pequeños en proceso 1
    printf("\n== Creando archivos pequeños en proceso 1 ==\n");
    for (int i = 0; i < 6; i++) {
        char nombre[20];
        sprintf(nombre, "file_%d.txt", i);
        osrmsFile* f = os_open(1, nombre, 'w');
        if (f) {
            os_write_file(f, "memoria/archivos/prueba.txt");
            os_close(f);
        }
    }
    os_ls_files(1);

    // Borrar algunos archivos pequeños para dejar huecos
    printf("\n== Borrando archivos pequeños para fragmentar ==\n");
    os_delete_file(1, "file_1.txt");
    os_delete_file(1, "file_3.txt");
    os_delete_file(1, "file_4.txt");
    os_ls_files(1);

    // Escribir archivos grandes en proceso 1
    printf("\n== Escribiendo archivos grandes en proceso 1 ==\n");
    osrmsFile* fsp = os_open(1, "spongebob.mp4", 'w');
    if (fsp) { os_write_file(fsp, "memoria/archivos/spongebob.mp4"); os_close(fsp); }
    osrmsFile* fcat = os_open(1, "cat.jpg", 'w');
    if (fcat) { os_write_file(fcat, "memoria/archivos/cat.jpg"); os_close(fcat); }
    os_ls_files(1);

    // Escribir archivos grandes en proceso 2
    printf("\n== Escribiendo archivos grandes en proceso 2 ==\n");
    osrmsFile* fdol = os_open(2, "dolphin.md", 'w');
    if (fdol) { os_write_file(fdol, "memoria/archivos/dolphin.md"); os_close(fdol); }
    osrmsFile* fhom = os_open(2, "homero.txt", 'w');
    if (fhom) { os_write_file(fhom, "memoria/archivos/homero.txt"); os_close(fhom); }
    os_ls_files(2);

    // Leer los archivos grandes y guardarlos como salida_* en la raíz
    printf("\n== Leyendo archivos grandes de proceso 1 ==\n");
    osrmsFile* freadsp = os_open(1, "spongebob.mp4", 'r');
    if (freadsp) { os_read_file(freadsp, "salida_spongebob.mp4"); os_close(freadsp); }
    osrmsFile* freadcat = os_open(1, "cat.jpg", 'r');
    if (freadcat) { os_read_file(freadcat, "salida_cat.jpg"); os_close(freadcat); }

    printf("\n== Leyendo archivos grandes de proceso 2 ==\n");
    osrmsFile* readdol = os_open(2, "dolphin.md", 'r');
    if (readdol) { os_read_file(readdol, "salida_dolphin.md"); os_close(readdol); }
    osrmsFile* readhom = os_open(2, "homero.txt", 'r');
    if (readhom) { os_read_file(readhom, "salida_homero.txt"); os_close(readhom); }

    // Mostrar bitmap de frames
    printf("\n== Bitmap de frames ==\n");
    os_frame_bitmap();

    return 0;
}