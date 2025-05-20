#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{

    // montar la memoria
    os_mount((char *)argv[1]);

    os_ls_processes();

    os_exists(6, "cat.jpg");

    os_ls_files(6);

    os_frame_bitmap();

    os_start_process(3, "neo_proceso");

    os_rename_process(3, "test_ren");

    os_ls_processes();

    os_frame_bitmap();

    os_finish_process(6);

    os_ls_processes();

    os_frame_bitmap();

    //liberar_frame_bitmap(65520);
    //print_bitmap_completo();
    //os_frame_bitmap();

}