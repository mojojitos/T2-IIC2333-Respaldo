#include <stdio.h>
#include <string.h>
#include "../osms_API/osms_API.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <ruta_memoria>\n", argv[0]);
        return 1;
    }
    os_mount(argv[1]);

    int pid = 1; // El mismo PID usado para poblar la memoria

    // Lista de archivos a leer (ajusta según los archivos que realmente existan)
    const char* archivos[] = {
        "file_0.txt",
        "file_2.txt",
        "file_4.txt",
        "archivo_grande.txt"
    };
    int num_archivos = sizeof(archivos) / sizeof(archivos[0]);

    for (int i = 0; i < num_archivos; i++) {
        printf("Leyendo archivo lógico '%s' del proceso %d...\n", archivos[i], pid);
        osrmsFile* f = os_open(pid, (char*)archivos[i], 'r');
        if (!f) {
            printf("  No se pudo abrir '%s'\n", archivos[i]);
            continue;
        }
        char nombre_copia[64];
        snprintf(nombre_copia, sizeof(nombre_copia), "copia_%s", archivos[i]);
        int leidos = os_read_file(f, nombre_copia);
        printf("  Se leyeron %d bytes y se guardó como '%s'\n", leidos, nombre_copia);
        os_close(f);
    }

    os_unmount();
    return 0;
}
