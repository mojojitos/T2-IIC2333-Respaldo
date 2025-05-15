#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdbool.h> // bool, true, false
#include "osms_API.h"


#include <limits.h>
char* path;
// Asumo que memoria montada es la memoria a la que se refiere aquí
char* memoria_montada_path = NULL;
FILE* memoria_montada = NULL;

void charToBinary(char ch, char *binaryStr) {
    for (int i = 7; i >= 0; i--) {
        // Extract each bit and store '0' or '1' in the string
        binaryStr[7 - i] = (ch & (1 << i)) ? '1' : '0';
    }
    binaryStr[8] = '\0';  // Null-terminate the string
}

void print_binary(char c) {
    for (int i = CHAR_BIT - 1; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
    printf("\n");
}

// Defino valores generales
// Donde empieza cada una de las partes de la memoria (en Bytes)
int inicio_tabla_PCB = 0; // Desde 0B
int inicio_tabla_paginas_inv = 8192; // 8KB
int inicio_frame_bitmap = 204800;// 8192 + 3*2^16= 8192 + 196608 = 204800; // 200KB
int inicio_tabla_frames = 212992;// 204800 + 2^13 = 204800 + 8192 = 212992; // 212992 = 208KB
// Datos Tablas PCB
int tamaño_entrada_PCB = 256;
int entradas_tabla_PCB = 32;
int pos_tabla_archivos = 240;
// Datos Tabla archivos de proceso
int entradas_tabla_archivos = 10;
int tamaño_entrada_archivo = 24;
// Datos Tabla de páginas invertida
int tamaño_entrada_pagina_invertida = 3;

// funciones generales
void os_mount(char* memory_path){
    printf("[Test Command]: OS Mount\n");
    memoria_montada_path = memory_path;
    memoria_montada = fopen(memoria_montada_path, "r+b");
    printf(memoria_montada == NULL ? "[Test] Error al abrir el archivo\n" : "[Test] Archivo abierto correctamente\n");
    // Creo que de alternativa a abrir un archivo que quiera leer en binario podría ser rb
    // TODO: Investigar por qué sugiere r+b ---> Creo que es para que no lo traduzca
}

void os_ls_processes(){
    printf("[Test Command]: OS ls processes\n");
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
/* int os exists(int process id, char* file name). Funci´on que verifica si un archivo con nom-
bre file name existe en la memoria del proceso con id process id. Retorna 1 si existe y 0 en caso contra-
rio. */
/* Entrada_Tabla_PCB* Buscar_proceso(int process_id){

    return proceso_encontrado;
} */
int os_exists(int process_id, char* file_name){
    printf("[Test Command]: OS exists\n");
    Entrada_Tabla_PCB* proceso_encontrado = NULL;
    fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_actual = &TablaPCB[i];
        if(entrada_actual->estado == 1){
            printf("[Test] Buscando PID = [%d], actual: PID:(%d) Nombre: (%s)\n",process_id, entrada_actual->pid, entrada_actual->nombre_proceso);
            
            if(entrada_actual->pid == process_id){
                printf("[Test] Encontré el Proceso, buscando en su tabla de archivo\n");
                proceso_encontrado = entrada_actual;
                char* tabla_archivos = proceso_encontrado->tabla_archivos;

                for(int j=0; j<10; j++){
                    char valido = tabla_archivos[j*24];
                    //printf("--- Válido (%d), comparo (%d)\n", valido, valido==1);
                    //print_binary(valido);
                    if(valido == 1){
                        printf("    [Test] Archivo válido (%d) encontrado!\n", valido);
                        /* char archivo[25];
                        memcpy(archivo, &tabla_archivos[j*24], 24);
                        archivo[24] = '\0';
                        printf("---Coso: %s\n", archivo); */
                        char nombre[15] = "";
                        memcpy(nombre, &tabla_archivos[j*24+1], 14);
                        nombre[14] = '\0';        
                        printf("    [Test] Comparo Nombre: (%s) v/s buscado: (%s)\n", nombre, file_name);
                        int condicion = strcmp(nombre, file_name);
                        printf("    [Test] Resultado de comparación: %d\n", condicion);
                        if(condicion == 0){
                            //printf("Lo he encontrado\n");
                            return 1;
                        }
                    } 
                }
                break;
            }
        }
    }
    //printf("No lo he encontrado\n");
    return 0;
}
// // funciones procesos

// // funciones archivos