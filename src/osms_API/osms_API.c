#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "osms_API.h"

char* path;
// Asumo que memoria montada es la memoria a la que se refiere aquí
char* memoria_montada_path = NULL;
FILE* memoria_montada = NULL;


// funciones generales
void os_mount(char* memory_path){
    memoria_montada_path = memory_path;
    memoria_montada = fopen(memoria_montada_path, "r+b");
    // Creo que de alternativa a abrir un archivo que quiera leer en binario podría ser rb
    // TODO: Investigar por qué sugiere r+b ---> Creo que es para que no lo traduzca
}
// // funciones procesos

// // funciones archivos