#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[])
{
    os_mount((char *)argv[1]);
    printf("[1] Memoria montada correctamente.\n");

    // 2. Crear archivo demo real (fuente para escritura)
    FILE* demo = fopen("demo.txt", "w");
    fprintf(demo, "Este es un archivo de prueba.\nLínea 2.\nLínea 3.");
    fclose(demo);
    printf("[2] Archivo demo.txt creado.\n");

    // 3. Abrir en modo write
    osrmsFile* f = os_open(0, "archivo_prueba.txt", 'w');
    if (!f) {
        printf("[ERROR] No se pudo abrir archivo en modo 'w'\n");
        return 1;
    }
    printf("[3] Archivo 'archivo_prueba.txt' abierto en modo 'w'\n");

    // 4. Escribir desde demo.txt
    int escritos = os_write_file(f, "demo.txt");
    printf("[4] Se escribieron %d bytes en memoria virtual.\n", escritos);

    // 5. Cerrar el descriptor
    os_close(f);
    printf("[5] Archivo cerrado.\n");

    // 6. Reabrir archivo en modo lectura
    f = os_open(0, "archivo_prueba.txt", 'r');
    if (!f) {
        printf("[ERROR] No se pudo abrir archivo en modo 'r'\n");
        return 1;
    }
    printf("[6] Archivo reabierto en modo 'r'\n");

    // 7. Leer desde memoria a archivo real
    int leidos = os_read_file(f, "archivo_copiado.txt");
    printf("[7] Se leyeron %d bytes desde memoria virtual a 'archivo_copiado.txt'\n", leidos);

    // 8. Cerrar descriptor
    os_close(f);

    // 9. Eliminar archivo
    os_delete_file(0, "archivo_prueba.txt");


    printf("==== TEST COMPLETADO ====\n");
    return 0;
}
