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
    printf("memoria_path: %s\n", memory_path);
    memoria_montada_path = memory_path;
    memoria_montada = fopen(memoria_montada_path, "r+b");
    printf(memoria_montada == NULL ? "Error al abrir el archivo\n" : "Archivo abierto correctamente\n");
    // Creo que de alternativa a abrir un archivo que quiera leer en binario podría ser rb
    // TODO: Investigar por qué sugiere r+b ---> Creo que es para que no lo traduzca
}

void os_ls_processes(){
    //printf("llego aquí\n");
    // Leo la memoria por Bytes, para ello coloco el puntero en el inicio de la tabla PCB
    fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    // Una vez seteado eso, leo y voy guardando en las entradas PCB
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    //fread(&TablaPCB, tamaño_entrada_PCB, entradas_tabla_PCB, memoria_montada);
    fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    // Muestro los procesos
    //printf("Mostrando procesos:\n");
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_actual = &TablaPCB[i];
        if(entrada_actual->estado == 1){
            printf("%d %s\n", entrada_actual->pid, entrada_actual->nombre_proceso);
        }
    } 
}
// // funciones procesos

// // funciones archivos