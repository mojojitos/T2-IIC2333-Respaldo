#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../osms_API/osms_API.h"

// con memformat.txt

int main(int argc, char const *argv[]){

    os_mount((char *)argv[1]);

    os_start_process(1, "proceso1");
    os_start_process(2, "proceso2");
    os_ls_processes();

    osrmsFile* f1 = os_open(1, "a.txt", 'w');
    if (!f1) { printf("Error abriendo a.txt para escritura\n"); return 1; }
    int written1 = os_write_file(f1, "memoria/touched/archivos/symbol.txt");
    printf("Proceso 1 escribió %d bytes en a.txt\n", written1);
    os_close(f1);

    osrmsFile* f2 = os_open(2, "b.txt", 'w');
    if (!f2) { printf("Error abriendo b.txt para escritura\n"); return 1; }
    int written2 = os_write_file(f2, "memoria/touched/archivos/symbol.txt");
    printf("Proceso 2 escribió %d bytes en b.txt\n", written2);
    os_close(f2);

    printf("Archivos de proceso 1:\n");
    os_ls_files(1);
    printf("Archivos de proceso 2:\n");
    os_ls_files(2);

    osrmsFile* f1r = os_open(1, "a.txt", 'r');
    if (!f1r) { printf("Error abriendo a.txt para lectura\n"); return 1; }
    int read1 = os_read_file(f1r, "salida1.txt");
    printf("Leídos %d bytes de a.txt a salida1.txt\n", read1);
    os_close(f1r);

    os_delete_file(1, "a.txt");
    printf("Archivos de proceso 1 tras borrar a.txt:\n");
    os_ls_files(1);

    os_frame_bitmap();

    //os_start_process(3, "neo_proceso");

    //os_rename_process(3, "test_ren");

    os_ls_processes();

    os_frame_bitmap();

    os_finish_process(1);

    os_ls_processes();

    os_frame_bitmap();

    //liberar_frame_bitmap(65520);
    //print_bitmap_completo();
    //os_frame_bitmap();

    os_unmount();
    return 0;
}