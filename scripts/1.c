#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{

    os_mount((char *)argv[1]);

    os_start_process(1, "proc1");
    os_start_process(2, "proc2");
    os_ls_processes();

    osrmsFile* f1 = os_open(1, "hola.txt", 'w');
    os_write_file(f1, "testfile1.txt"); // testfile1.txt debe existir en tu carpeta
    os_close(f1);

    osrmsFile* f2 = os_open(1, "hola.txt", 'r');
    os_read_file(f2, "salida1.txt");
    os_close(f2);

    os_ls_files(1);

    os_delete_file(1, "hola.txt");
    os_ls_files(1);

    os_finish_process(1);
    os_ls_processes();

    //liberar_frame_bitmap(65520);
    //print_bitmap_completo();
    //os_frame_bitmap();

}