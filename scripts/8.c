#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);

    FILE* demo = fopen("demo.txt", "w");
    fprintf(demo, "Escritura de prueba para test individual.");
    fclose(demo);

    osrmsFile* f = os_open(0, "escritura.txt", 'w');
    int escritos = os_write_file(f, "demo.txt");
    printf("[3] Bytes escritos: %d\n", escritos);
    os_close(f);

    return 0;
}
