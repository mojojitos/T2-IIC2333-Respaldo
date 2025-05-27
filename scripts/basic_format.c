#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../osms_API/osms_API.h"

// con memformat.bin clean

int main(int argc, char const *argv[]){

    os_mount((char *)argv[1]);

    os_start_process(1, "proceso1");
    os_start_process(2, "proceso2");
    os_ls_processes();

    osrmsFile* f1 = os_open(1, "a.mp4", 'w');
    if (!f1) { printf("Error abriendo a.mp4 para escritura\n"); return 1; }
    int written1 = os_write_file(f1, "memoria/archivos/chatarra.mp4");
    printf("Proceso 1 escribió %d bytes en a.mp4\n", written1);
    os_close(f1);

    osrmsFile* f2 = os_open(2, "b.mp4", 'w');
    if (!f2) { printf("Error abriendo b.mp4 para escritura\n"); return 1; }
    int written2 = os_write_file(f2, "memoria/archivos/chatarra.mp4");
    printf("Proceso 2 escribió %d bytes en b.mp4\n", written2);
    os_close(f2);

    printf("Archivos de proceso 1:\n");
    os_ls_files(1);
    printf("Archivos de proceso 2:\n");
    os_ls_files(2);

    osrmsFile* f1r = os_open(1, "a.mp4", 'r');
    if (!f1r) { printf("Error abriendo a.mp4 para lectura\n"); return 1; }
    int read1 = os_read_file(f1r, "salida1.mp4");
    printf("Leídos %d bytes de a.mp4 a salida1.mp4\n", read1);
    os_close(f1r);

    os_delete_file(1, "a.mp4");
    printf("Archivos de proceso 1 tras borrar a.mp4:\n");
    os_ls_files(1);

    os_frame_bitmap();

    return 0;
}