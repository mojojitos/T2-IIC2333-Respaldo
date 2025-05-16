#include <stdio.h>	// FILE, fopen, fclose, etc.
#include <stdlib.h> // malloc, calloc, free, etc
#include <string.h> //para strcmp
#include <stdint.h>
#include <stdbool.h> // bool, true, false
#include "osms_API.h"


#include <limits.h>
char* path;
// Asumo que memoria montada es la memoria a la que se refiere aquí
char* memoria_montada_path = NULL;
FILE* memoria_montada = NULL;

/* void charToBinary(char ch, char *binaryStr) {
    for (int i = 7; i >= 0; i--) {
        // Extract each bit and store '0' or '1' in the string
        binaryStr[7 - i] = (ch & (1 << i)) ? '1' : '0';
    }
    binaryStr[8] = '\0';  // Null-terminate the string
} */

/* void print_binary(char c) {
    for (int i = CHAR_BIT - 1; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
    printf("\n");
} */

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
// Datos bitmap
int tamaño_bitmap_bytes = 8192;
int tamaño_bitmap_bits = 65536;

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

int os_exists(int process_id, char* file_name){
    printf("[Test Command]: OS exists\n");
    fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_actual = &TablaPCB[i];
        if(entrada_actual->estado == 1){
            //printf("[Test] Buscando PID = [%d], actual: PID:(%d) Nombre: (%s)\n",process_id, entrada_actual->pid, entrada_actual->nombre_proceso);
            
            if(entrada_actual->pid == process_id){
                //printf("[Test] Encontré el Proceso, buscando en su tabla de archivo\n");
                char* tabla_archivos = entrada_actual->tabla_archivos;

                for(int j=0; j<10; j++){
                    char valido = tabla_archivos[j*24];
                    if(valido == 1){
                        //printf("    [Test] Archivo válido (%d) encontrado!\n", valido);
                        char nombre[15] = "";
                        memcpy(nombre, &tabla_archivos[j*24+1], 14);
                        nombre[14] = '\0';        
                        //printf("    [Test] Comparo Nombre: (%s) v/s buscado: (%s)\n", nombre, file_name);
                        int condicion = strcmp(nombre, file_name);
                        //printf("    [Test] Resultado de comparación: %d\n", condicion);
                        if(condicion == 0){
                            printf("[Test] Lo he encontrado\n");
                            return 1;
                        }
                    } 
                }
                break;
            }
        }
    }
    printf("[Test] No lo he encontrado\n");
    return 0;
}


int32_t read_little_endian_5bytes(const char* data) {
    int32_t result = 0;

    // Iterate over the 5 bytes, least significant first (little-endian).
    for (int i = 0; i < 5; i++) {
        result |= (uint8_t)data[i] << (8 * i);  // Shift and accumulate
    }

    return result;
}
/* 
void print_address(char addr[4]) {
    // Cast the char array to an integer pointer to read the address.
    uintptr_t address = 0;

    // Construct the address assuming little-endian format.
    for (int i = 0; i < 4; i++) {
        address |= (unsigned char)addr[i] << (8 * i);
    }

    printf("Memory address: 0x%08lX\n", (unsigned long)address);
} */

void os_ls_files(int process_id){
    printf("[Test Command]: OS ls files\n");
    Entrada_Tabla_PCB* proceso_encontrado = NULL;
    fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_actual = &TablaPCB[i];
        if(entrada_actual->estado == 1){
            //printf("[Test] Buscando PID = [%d], actual: PID:(%d) Nombre: (%s)\n",process_id, entrada_actual->pid, entrada_actual->nombre_proceso);
            
            if(entrada_actual->pid == process_id){
                //printf("[Test] Encontré el Proceso, buscando en su tabla de archivo\n");
                proceso_encontrado = entrada_actual;
                char* tabla_archivos = proceso_encontrado->tabla_archivos;

                for(int j=0; j<10; j++){
                    //Entrada_Tabla_Archivos* entrada_archivo_actual = (Entrada_Tabla_Archivos*) &tabla_archivos[j*sizeof(Entrada_Tabla_Archivos)];
                    //printf("    Nombre: (%s), tamaño : (%s)B\n", entrada_archivo_actual->nombre_archivo, entrada_archivo_actual->tamaño_archivo);

                    char valido = tabla_archivos[j*24];
                    if(valido == 1){
                        //printf("    [Test] Archivo válido (%d) encontrado!\n", valido);
                        Entrada_Tabla_Archivos* entrada_archivo_actual = (Entrada_Tabla_Archivos*) &tabla_archivos[j*sizeof(Entrada_Tabla_Archivos)];
                        int tamaño_int = read_little_endian_5bytes(entrada_archivo_actual->tamaño_archivo_bytes);
                        //printf("    DirV: (0x%x) Nombre: (%s), tamaño : (%d)B\n", entrada_archivo_actual->dir_virtual, entrada_archivo_actual->nombre_archivo, tamaño_int);
                        // Manejar lo de dirV, sus bits, cosa de extraer VPN y offset
                        // Aplico lo de las mascaras
                        unsigned int mascara_primeros_5 = 0x1F;
                        unsigned int mascara_VPN = 0XFFF;
                        unsigned int mascara_offset = 0x7FFF;
                        unsigned int primeros_5 = entrada_archivo_actual->dir_virtual & mascara_primeros_5;
                        unsigned int VPN = (entrada_archivo_actual->dir_virtual >> 5) & mascara_VPN;
                        unsigned int offset = (entrada_archivo_actual->dir_virtual >> 17) & mascara_offset;
                        //printf("    bit basura: (0x%x), VPN (0x%x), offset (0x%x)\n", primeros_5, VPN, offset);
                        printf("0x%x %d 0x%x %s\n", VPN, tamaño_int, entrada_archivo_actual->dir_virtual, entrada_archivo_actual->nombre_archivo);
                    }  
                }
                break;
            }
        }
    }
    //printf("No lo he encontrado\n");
}

void os_frame_bitmap(){
    //TODO: Aquí aplica lo de little endian???
    // Ver bien cómo hacer esto, si guardo variable o no
    // y cómo esa bariable queda guardada
    printf("[Test Command]: OS frame bitmap\n");
    unsigned char bitmap[tamaño_bitmap_bytes];

    fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    fread(bitmap, 1, tamaño_bitmap_bytes, memoria_montada);

    int n_libres=0;
    int n_ocupados=0;

    for (int byte_actual = 0; byte_actual < tamaño_bitmap_bytes; byte_actual++) {
        for (int bit_actual = 0; bit_actual < 8; bit_actual++) {
            // Extrae cada bit del byte_actual
            int valor_bit = (bitmap[byte_actual] >> bit_actual) & 1;
            if(valor_bit == 1){
                //printf("[Test] Byte %d, Bit %d: %d\n", byte_actual, bit_actual, valor_bit);
                n_ocupados++;
            }
            //printf("[Test] Byte %d, Bit %d: %d\n", byte_actual, bit_actual, valor_bit);
            else{
                n_libres++;
            }
        }
    }
    printf("USADOS %d\n", n_ocupados);
    printf("LIBRES %d\n", n_libres);
}
// // funciones procesos

// // funciones archivos