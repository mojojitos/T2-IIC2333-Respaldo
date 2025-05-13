#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "osms_API.h"

char* path;
// Asumo que memoria montada es la memoria a la que se refiere aquí
char* memoria_montada_path = NULL;
FILE* memoria_montada = NULL;

// Defino valores generales
// Donde empieza cada una de las partes de la memoria (en Bytes)
int inicio_tabla_PCB = 0; // Desde 0B
int inicio_tabla_paginas_inv = 8192; // 8KB
int inicio_frame_bitmap = 204800;// 8192 + 3*2^16= 8192 + 196608 = 204800; // 200KB
int inicio_tabla_frames = 212992;// 204800 + 2^13 = 204800 + 8192 = 212992; // 212992 = 208KB
// Datos Tablas PCB
int tamaño_entrada_PCB = 256;
int entradas_tabla_PCB = 32;
// Datos Tabla archivos de proceso
int entradas_tabla_archivos = 10;
int tamaño_entrada_archivo = 24;
// Datos Tabla de páginas invertida
int tamaño_entrada_pagina_invertida = 3;

// funciones generales
void os_mount(char* memory_path){
    memoria_montada_path = memory_path;
    memoria_montada = fopen(memoria_montada_path, "r+b");
    // Creo que de alternativa a abrir un archivo que quiera leer en binario podría ser rb
    // TODO: Investigar por qué sugiere r+b ---> Creo que es para que no lo traduzca
}
// // funciones procesos

// // funciones archivos