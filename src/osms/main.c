#include <stdio.h>
#include <string.h>
#include "../osms_API/osms_API.h"

int main(int argc, char const *argv[]){
    os_mount((char *)argv[1]);

    int pid = 1; // El mismo PID usado para poblar la memoria

    // Lista de archivos lógicos y sus fuentes locales
    const char* archivos_logicos[] = {
        "file_0.txt",
        "file_2.txt",
        "file_4.txt",
        "archivo_grande.txt"
    };
    const char* archivos_fuente[] = {
        "nuevo_tiny_0.txt",
        "nuevo_tiny_2.txt",
        "nuevo_tiny_4.txt",
        "nuevo_grande.txt"
    };
    int num_archivos = sizeof(archivos_logicos) / sizeof(archivos_logicos[0]);

    for (int i = 0; i < num_archivos; i++) {
        printf("Escribiendo en archivo lógico '%s' del proceso %d desde '%s'...\n", archivos_logicos[i], pid, archivos_fuente[i]);
        osrmsFile* f = os_open(pid, (char*)archivos_logicos[i], 'w');
        if (!f) {
            printf("  No se pudo abrir '%s' para escritura\n", archivos_logicos[i]);
            continue;
        }
        int escritos = os_write_file(f, (char*)archivos_fuente[i]);
        printf("  Se escribieron %d bytes en '%s'\n", escritos, archivos_logicos[i]);
        os_close(f);
    }

    os_unmount();
    return 0;
}