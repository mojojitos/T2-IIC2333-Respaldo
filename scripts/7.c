#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount(argv[1]);

    printf("=== Procesos activos ===\n");
    os_ls_processes();

    printf("\n=== Archivos por proceso ===\n");
    // Recorre todos los posibles PID (ajusta el rango si tu sistema usa otro)
    for (int pid = 0; pid < 32; pid++) {
        printf("\n[PID %d]\n", pid);
        os_ls_files(pid);
    }

    os_unmount();
    return 0;
}
