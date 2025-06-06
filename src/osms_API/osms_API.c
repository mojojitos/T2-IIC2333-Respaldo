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
int tamaño_entrada_pagina_invertida = 3; // Bytes
int entradas_TIP = 65536;
// Datos bitmap
int tamaño_bitmap_bytes = 8192;
int tamaño_bitmap_bits = 65536;
// Datos Conjunto Frames
int tamaño_frame = 32768;
int cantidad_frames = 65536;








// funciones generales
void os_mount(char* memory_path){
    //printf("[Test Command]: OS Mount\n");
    memoria_montada_path = memory_path;
    memoria_montada = fopen(memoria_montada_path, "r+b");
    printf(memoria_montada == NULL ? "[Test] Error al abrir el archivo\n" : "[Test] Archivo abierto correctamente\n");
    fclose(memoria_montada);
}

void os_ls_processes(){
    //printf("[Test Command]: OS ls processes\n");
    // Leo la memoria por Bytes, para ello coloco el puntero en el inicio de la tabla PCB
    memoria_montada = fopen(memoria_montada_path, "r+b");
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
            printf("    %d %s\n", entrada_actual->pid, entrada_actual->nombre_proceso);
        }
    } 
    fclose(memoria_montada);
}

int os_exists(int process_id, char* file_name){
    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS exists\n");
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
                            //printf("[Test success] Lo he encontrado\n");
                            fclose(memoria_montada);
                            return 1;
                        }
                    } 
                }
                break;
            }
        }
    }
    //printf("[Test error] No lo he encontrado\n");
    fclose(memoria_montada);
    return 0;
}


int32_t leer_5Bytes_little_endian(const char* data) {
    int32_t result = 0;

    // Iterate over the 5 bytes, least significant first (little-endian).
    for (int i = 0; i < 5; i++) {
        result |= (uint8_t)data[i] << (8 * i);  // Shift and accumulate
    }

    return result;
}

void os_ls_files(int process_id){
    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS ls files\n");
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
                    char valido = tabla_archivos[j*24];
                    if(valido == 1){
                        //printf("    [Test] Archivo válido (%d) encontrado!\n", valido);
                        Entrada_Tabla_Archivos* entrada_archivo_actual = (Entrada_Tabla_Archivos*) &tabla_archivos[j*sizeof(Entrada_Tabla_Archivos)];
                        int tamaño_int = leer_5Bytes_little_endian(entrada_archivo_actual->tamaño_archivo_bytes);
                        //printf("    DirV: (0x%x) Nombre: (%s), tamaño : (%d)B\n", entrada_archivo_actual->dir_virtual, entrada_archivo_actual->nombre_archivo, tamaño_int);                        
                        unsigned int mascara_VPN = 0XFFF;
                        unsigned int VPN = (entrada_archivo_actual->dir_virtual >> 5) & mascara_VPN;
                        //unsigned int mascara_offset = 0x7FFF;
                        //unsigned int offset = (entrada_archivo_actual->dir_virtual >> 17) & mascara_offset;
                        //printf("    VPN (0x%x), offset (0x%x)\n", VPN, offset);
                        printf("    0x%x %d 0x%x %s\n", VPN, tamaño_int, entrada_archivo_actual->dir_virtual, entrada_archivo_actual->nombre_archivo);
                    }  
                }
                break;
            }
        }
    }
    fclose(memoria_montada);
    //printf("[Test error] No lo he encontrado\n");
}

void os_frame_bitmap(){
    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS frame bitmap\n");

    fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    //unsigned char* bitmap = calloc(tamaño_bitmap_bytes, sizeof(unsigned int));
    unsigned char bitmap[tamaño_bitmap_bytes];
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
    printf("    USADOS %d\n", n_ocupados);
    printf("    LIBRES %d\n", n_libres);
    //free(bitmap);
    fclose(memoria_montada);
}




// // funciones procesos
void print_entrada_PCB(Entrada_Tabla_PCB* entrada){
    printf("        [Test] Estado: (%d), Nombre: (%s) Id_proceso: (%d), Tabla archivos: (%s)\n", entrada->estado, entrada->nombre_proceso, entrada->pid, entrada->tabla_archivos);
}

int os_start_process(int process_id, char* process_name){
    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS start process\n");

    int bytes_nombre = strlen(process_name) + 1;
    //printf("[Test] El nombre ingresado tiene %d bytes\n", bytes_nombre);
    if(bytes_nombre > 14){
        printf("[Test error] (Start Process) No es un largo de nombre permitido, excede los 14 Bytes\n");
        fclose(memoria_montada);
        return -1;
    }
    
    int check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Start Process) Ha fallado colocar el puntero en la memoria\n");
        fclose(memoria_montada);
        return -1;
    }
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    size_t bytes_leidos_check = fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    if(bytes_leidos_check != entradas_tabla_PCB){
        printf("[Test error] (Start Process) Ha fallado la cantidad de bits leídos\n");
        fclose(memoria_montada);
        return -1;
    }
    int insertado = 0;
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_PCB_actual = &TablaPCB[i];
        //printf("    [Test] [%d] Valido: (%d)\n", i, entrada_PCB_actual->estado);
        if(entrada_PCB_actual->estado == 0){
            insertado++;
            //printf("        [Test] He encontrado primera entrada libre\n");
            entrada_PCB_actual->estado = 1;
            strcpy(entrada_PCB_actual->nombre_proceso, process_name);
            entrada_PCB_actual->pid = process_id;
            // Voy a setear lo de la tabla de archivos en 0 por si las moscas
            memset(entrada_PCB_actual->tabla_archivos, 0, sizeof(entrada_PCB_actual->tabla_archivos));
            //print_entrada_PCB(entrada_PCB_actual);
            break;
        }
    }
    if(insertado != 0){
        //printf("\n Escribiendo cambios en la memoria:\n");
        check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
        if(check_puntero != 0){
            printf("[Test error] (Start Process) Ha fallado colocar el puntero en la memoria\n");
            fclose(memoria_montada);
            return -1;
        }
        size_t bytes_escritos_check = fwrite(TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
        if(bytes_escritos_check != entradas_tabla_PCB){
            printf("[Test error] (Start Process) Ha fallado en la cantidad de bits escritos\n");
            fclose(memoria_montada);
            return -1;
        }

/*         printf("\n Revisando la memoria ahora:\n");
        fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
        fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
        for(int i=0; i<entradas_tabla_PCB; i++){
            Entrada_Tabla_PCB* entrada_PCB_actual = &TablaPCB[i];
            printf("    [Test] [%d] Valido: (%d)\n", i, entrada_PCB_actual->estado);
            if(entrada_PCB_actual->estado == 1){
                print_entrada_PCB(entrada_PCB_actual);
            }
        } */
        //printf("[Test Success]: (Start Process) Proceso pid=(%d) iniciado, retornando 0\n", process_id);
        fclose(memoria_montada);
        return 0;
    }
    printf("[Test error] (Start Process) No se ha podido iniciar un proceso nuevo\n");
    fclose(memoria_montada);
    return -1; //TODO: Agregar el return 0 si está bien

}

void print_entrada_Archivo_valido(Entrada_Tabla_Archivos* entrada){
    int tamaño_int = leer_5Bytes_little_endian(entrada->tamaño_archivo_bytes);
    unsigned int mascara_VPN = 0XFFF;
    unsigned int mascara_offset = 0x7FFF;
    unsigned int VPN = (entrada->dir_virtual >> 5) & mascara_VPN;
    unsigned int offset = (entrada->dir_virtual >> 17) & mascara_offset;
    printf("    DirV: (0x%x) Tamaño: (%d) VPN: (0x%x) Offset: (0x%x) Nombre: (%s)\n", VPN, tamaño_int, entrada->dir_virtual, offset, entrada->nombre_archivo);
}


void print_bitmap_completo(){
    fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    unsigned int bitmap[tamaño_bitmap_bytes];
    fread(bitmap, 1, tamaño_bitmap_bytes, memoria_montada);
    for (int i = 0; i < tamaño_bitmap_bits; ++i) {
        int byte_index = i / 8;
        int bit_offset = i % 8;
        int bit_value = (bitmap[byte_index] >> bit_offset) & 1;
        printf("        - Bit [%d]: %d\n", i, bit_value);
    }
}





























void print_bitmap_zone(int n_frame){
    fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    unsigned char* bitmap = calloc(tamaño_bitmap_bytes, sizeof(unsigned int));
    fread(bitmap, 1, tamaño_bitmap_bytes, memoria_montada);

    for (int byte_actual = n_frame/8 -1; byte_actual < n_frame/8 +1; byte_actual++) {
        for (int bit_actual = 0; bit_actual < 8; bit_actual++) {
            // Extrae cada bit del byte_actual
            int valor_bit = (bitmap[byte_actual] >> bit_actual) & 1;
            int n_bit = byte_actual*8+bit_actual;
            if(n_bit != n_frame){
                printf("[Test] [%d] Byte %d, Bit %d: -- (%d)\n", byte_actual*8+bit_actual ,byte_actual, bit_actual, valor_bit);
            }
            else{
                printf("[Test] ** [%d] Byte %d, Bit %d: -- (%d)\n", byte_actual*8+bit_actual ,byte_actual, bit_actual, valor_bit);
            }
            
        }
    }
    free(bitmap);
}


int liberar_frame_bitmap(int n_frame){
    //printf("[Test Command]: OS frame bitmap\n");
    unsigned char bitmap[tamaño_bitmap_bytes];

    int check_puntero = fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Finish Process/Free TIP/Free bitmap) Ha fallado colocar puntero en memoria\n");
        return -1;
    }
    size_t bytes_leidos_check = fread(bitmap, 1, tamaño_bitmap_bytes, memoria_montada);
    if(bytes_leidos_check != tamaño_bitmap_bytes){
        printf("[Test error] (Finish Process/Free TIP) Ha fallado cantidad de bytes leídos\n");
        return -1;
    }

    //print_bitmap_zone(n_frame);
    for (int byte_actual = n_frame/8 -1; byte_actual < n_frame/8 +1; byte_actual++) {
        for (int bit_actual = 0; bit_actual < 8; bit_actual++) {
            // Extrae cada bit del byte_actual
            //int valor_bit = (bitmap[byte_actual] >> bit_actual) & 1;
/*             if(byte_actual*8+bit_actual != n_frame){
                printf("[Test] [%d] Byte %d, Bit %d: -- (%d)\n", byte_actual*8+bit_actual ,byte_actual, bit_actual, valor_bit);
            }
            else{
                printf("[Test] ** [%d] Byte %d, Bit %d: -- (%d)\n", byte_actual*8+bit_actual ,byte_actual, bit_actual, valor_bit);
            } */
            if(byte_actual*8+bit_actual == n_frame){
                //printf("Modificando\n");
                bitmap[byte_actual] &= ~(1U << bit_actual); // Setear en 0
                //bitmap[byte_actual] |= (1U << bit_actual); // Setear en 1
            }
        }
    }
    
    //printf("\n");
    
/*     for (int byte_actual = n_frame/8 -1; byte_actual < n_frame/8 +1; byte_actual++) {
        for (int bit_actual = 0; bit_actual < 8; bit_actual++) {
            // Extrae cada bit del byte_actual
            int valor_bit = (bitmap[byte_actual] >> bit_actual) & 1;
            if(byte_actual*8+bit_actual != n_frame){
                printf("[Test] [%d] Byte %d, Bit %d: -- (%d)\n", byte_actual*8+bit_actual ,byte_actual, bit_actual, valor_bit);
            }
            else{
                printf("[Test] ** [%d] Byte %d, Bit %d: -- (%d)\n", byte_actual*8+bit_actual ,byte_actual, bit_actual, valor_bit);
            }
            //bitmap[byte_actual] = 1;
        }
    } */

    //printf("\n");
    check_puntero = fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Finish Process/Free TIP/Free bitmap) Ha fallado colocar el puntero en la memoria\n");
        return -1;
    }
    size_t bytes_escritos_check = fwrite(bitmap, 1, tamaño_bitmap_bytes, memoria_montada);
    if(bytes_escritos_check != tamaño_bitmap_bytes){
        printf("[Test error] (Finish Process/Free TIP/Free bitmap) Ha fallado la cantidad de bytes escritos\n");
        return -1;
    }
    //printf("[Test Success]: (Finish Process/Free TIP/Free bitmap) Bit (%d) en Bitmap liberado\n", n_frame);
    return 0;
    //print_bitmap_zone(n_frame);
}



int liberar_frame_real(int n_frame){ // Ahora funcional
    //printear_frames();
    // Obtengo la posición del frame dentro del conjunto de frames y seteo el puntero de
    // lectura allí
    long posicion_frame = (tamaño_frame*n_frame);
    int check_puntero = fseek(memoria_montada, inicio_tabla_frames + posicion_frame, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Finish Process/Free TIP/Free frame) Ha fallado colocar puntero en memoria\n");
        return -1;
    }
    // Como quiero reemplazar algo bien grande (y pq se cayó las otras veces) hago calloc
    // de un "nuevo frame" y seteo todo en 0
    unsigned int* frame_nuevo_vacio = calloc(tamaño_frame, sizeof(unsigned int));
    for(int i=0; i<tamaño_frame; i++){
        frame_nuevo_vacio[i] = 0;
    }
    // Escribo y libero
    size_t bytes_escritos_check = fwrite(frame_nuevo_vacio, 1, tamaño_frame, memoria_montada);
    free(frame_nuevo_vacio);
    if(bytes_escritos_check != tamaño_frame){
        printf("[Test error] (Finish Process/Free TIP/Free frame) Ha fallado la cantidad de bytes escritos\n");
        return -1;
    }
    /* unsigned int* frame_actual = calloc(tamaño_frame, sizeof(unsigned int));
    fseek(memoria_montada, inicio_tabla_frames + posicion_frame, SEEK_SET);
    fread(frame_actual, tamaño_frame, 1, memoria_montada);
    printf("Actual nuevo/modificado [%d]: %x\n", n_frame, *frame_actual);
    free(frame_actual); */
    //printf("[Test Success]: (Finish Process/Free TIP/Free frame) Frame (%d) en el Conjunto de Frames liberado\n", n_frame);
    return 0;
}


int liberar_memoria_TIP(int id_proceso, unsigned int VPN, unsigned int offset){
    int check_puntero = fseek(memoria_montada, inicio_tabla_paginas_inv, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Finish Process/Free TIP) Ha fallado colocar el puntero en la memoria\n");
        return -1;
    }
    Entrada_TIP TablaTIP[entradas_TIP];
    size_t bytes_leidos_check = fread(&TablaTIP, sizeof(Entrada_TIP), entradas_TIP, memoria_montada);
    if(bytes_leidos_check != entradas_TIP){
        printf("[Test error] (Finish Process/Free TIP) Ha fallado cantidad de bytes leídos\n");
        return -1;
    }
    for(int i=0; i<entradas_TIP; i++){
        Entrada_TIP* entrada_actual = &TablaTIP[i];
        unsigned int mascara_validez = 0x1;
        unsigned int bit_validez = entrada_actual->bits & mascara_validez;
        if(bit_validez == 1){
            unsigned int mascara_pid = 0xFF;
            unsigned int pid = (entrada_actual->bits >> 3) & mascara_pid;
            
            unsigned int mascara_VPN = 0xFFF;
            unsigned int neo_VPN = (entrada_actual->bits >> 12) & mascara_VPN;
            //printf("    Frame [%d], validez: (%d), PID: (%d) VPN: (0x%x)\n", i, bit_validez, pid, neo_VPN);
            if(pid == id_proceso && VPN == neo_VPN){
                int n_frame = i;
                //printf("    [Test]Actual================\n");
                //printf("        [Test]Frame [%d], validez: (%d), PID: (%d) VPN: (0x%x)\n", i, bit_validez, pid, neo_VPN);
                int check_liberar_bitmap = liberar_frame_bitmap(n_frame);
                if(check_liberar_bitmap != 0){
                    printf("[Test error] (Finish Process/Free TIP) Ha habido un error en liberar bitmap\n");
                    return -1;
                }
                int check_liberar_frame_real = liberar_frame_real(n_frame);
                if(check_liberar_frame_real != 0){
                    printf("[Test error] (Finish Process/Free TIP) Ha habido un error en liberar frame\n");
                    return -1;
                }
                entrada_actual->bits = 0;

                check_puntero = fseek(memoria_montada, inicio_tabla_paginas_inv, SEEK_SET);
                if(check_puntero != 0){
                    printf("[Test error] (Finish Process/Free TIP) Ha fallado colocar el puntero en la memoria\n");
                    return -1;
                }
                size_t bytes_escritos_check = fwrite(TablaTIP, sizeof(Entrada_TIP), entradas_TIP, memoria_montada);
                if(bytes_escritos_check != entradas_TIP){
                    printf("[Test error] (Finish Process/Free TIP) Ha fallado cantidad de bytes escritos\n");
                    return -1;
                }
                
                //return 0;
            }
        }
    }
    //printf("[Test Success]: (Finish Process/Free TIP) Páginas-frames asociados a pid=(%d) y VPN=(0x%x) liberados\n", id_proceso, VPN);
    return 0; 
    // Dejo este caso como OK, dado que planteo la lógica de:
    // Dado que la TIP refleja en cada entrada el frame asociado por índice, cada frame
    // solo (asumo/supongo) que puede tener elementos/info de 1 mismo proceso, pero
    // potencialmente múltiples archivos. Por ello al liberar/finalizar un proceso puedo
    // liberar 1 sola vez dicho frame y dejarlo con validez=0 (+vacío) para futuras búsquedas
    // Así, un archivo que quiera liberar un frame ya liberado pasaría a este return 0
}

int liberar_memoria_proceso(Entrada_Tabla_PCB* PCB_actual){
    // Recorrer sus archivos obteniendo el VPN y offset
    char* tabla_archivos = PCB_actual->tabla_archivos;
    for(int j=0; j<10; j++){
        char valido = tabla_archivos[j*24];
        if(valido == 1){
            Entrada_Tabla_Archivos* entrada_archivo_actual = (Entrada_Tabla_Archivos*) &tabla_archivos[j*sizeof(Entrada_Tabla_Archivos)];
            //print_entrada_Archivo_valido(entrada_archivo_actual);
            //int tamaño_int = leer_5Bytes_little_endian(entrada_archivo_actual->tamaño_archivo_bytes);
            unsigned int mascara_VPN = 0XFFF;
            unsigned int mascara_offset = 0x7FFF;
            unsigned int VPN = (entrada_archivo_actual->dir_virtual >> 5) & mascara_VPN;
            unsigned int offset = (entrada_archivo_actual->dir_virtual >> 17) & mascara_offset;
            // Llamado funcion de liberar la TIP
            int exitoso = liberar_memoria_TIP(PCB_actual->pid, VPN, offset); 
            if(exitoso != 0){
                printf("[Test error] (Finish Process) Ha habido un error al liberar TIP\n");
                return -1;
            }
        }  
    }
    //printf("[Test Success]: (Finish Process) Memoria asociada al proceso pid=(%d) liberada\n", PCB_actual->pid);
    return 0;
}

int os_finish_process(int process_id){
    /* see_process_frames(process_id);
    printf("AAAAAAAAAAAAAAA============AAAAAAAAAAAAAa");
    return 0; */

    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS finish process\n");
    int check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Finish Process) Ha fallado colocar el puntero en la memoria\n");
        fclose(memoria_montada);
        return -1;
    }
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    size_t bytes_leidos_check = fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    if(bytes_leidos_check != entradas_tabla_PCB){
        printf("[Test error] (Finish Process) Ha fallado cantidad de bytes leídos\n");
        fclose(memoria_montada);
        return -1;
    }
    int encontrado = 0;
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_PCB_actual = &TablaPCB[i];
        /* if(entrada_PCB_actual->estado == 1){
            print_entrada_PCB(entrada_PCB_actual);
        } */
        if(entrada_PCB_actual->estado == 1 && entrada_PCB_actual->pid == process_id){
            encontrado++;
            //printf("        [Test] He encontrado el proceso buscado\n");
            int exitoso = liberar_memoria_proceso(entrada_PCB_actual);
            if(exitoso != 0){
                printf("[Test error] (Finish Process) Ha ocurrido un error al liberar la memoria\n");
                fclose(memoria_montada);
                return -1;
            }
            entrada_PCB_actual->pid = 0;
            memset(entrada_PCB_actual->tabla_archivos, 0, sizeof(entrada_PCB_actual->tabla_archivos));
            memset(entrada_PCB_actual->nombre_proceso, 0, sizeof(entrada_PCB_actual->nombre_proceso));
            entrada_PCB_actual->estado = 0;
            //print_entrada_PCB(entrada_PCB_actual);
            break;
        }
    }
    if(encontrado != 0){
        //printf("\n Escribiendo cambios en la memoria:\n");
        check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
        if(check_puntero != 0){
            printf("[Test error] (Finish process) Ha fallado colocar el puntero en la memoria\n");
            fclose(memoria_montada);
            return -1;
        }
        size_t bytes_escritos_check = fwrite(TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
        if(bytes_escritos_check != entradas_tabla_PCB){
            printf("[Test error] (Finish Process) Ha fallado cantidad de bytes escritos\n");
            fclose(memoria_montada);
            return -1;
        }
        //printf("[Test Success]: (Finish Process) Se ha liberado proceso pid=(%d)\n", process_id);
        fclose(memoria_montada);
        return 0;
    }
    printf("[Test error] (Finish Process) No se ha encontrado un proceso con ese id\n");
    fclose(memoria_montada);
    return -1; //TODO: Agregar el return 0 si está bien
}












int see_frame_bitmap(int n_frame){
    printf("[Test Command]: See pid bitmap\n");
    unsigned char bitmap[tamaño_bitmap_bytes];

    int check_puntero = fseek(memoria_montada, inicio_frame_bitmap, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (See Process/See TIP/See bitmap) Ha fallado colocar puntero en memoria\n");
        return -1;
    }
    size_t bytes_leidos_check = fread(bitmap, 1, tamaño_bitmap_bytes, memoria_montada);
    if(bytes_leidos_check != tamaño_bitmap_bytes){
        printf("[Test error] (See Process/See TIP/See bitmap) Ha fallado cantidad de bytes leídos\n");
        return -1;
    }
    printf("¿¿¿¿¿¿¿¿¿¿¿¿\n");
    print_bitmap_zone(n_frame);
    printf("¿¿¿¿¿¿¿¿¿¿¿¿\n");
    return 0;
    //print_bitmap_zone(n_frame);
}
int see_memoria_TIP(int id_proceso, unsigned int VPN, unsigned int offset){
    int check_puntero = fseek(memoria_montada, inicio_tabla_paginas_inv, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (See Process/See TIP) Ha fallado colocar el puntero en la memoria\n");
        return -1;
    }
    Entrada_TIP TablaTIP[entradas_TIP];
    size_t bytes_leidos_check = fread(&TablaTIP, sizeof(Entrada_TIP), entradas_TIP, memoria_montada);
    if(bytes_leidos_check != entradas_TIP){
        printf("[Test error] (See Process/See TIP) Ha fallado cantidad de bytes leídos\n");
        return -1;
    }
    for(int i=0; i<entradas_TIP; i++){
        Entrada_TIP* entrada_actual = &TablaTIP[i];
        unsigned int mascara_validez = 0x1;
        unsigned int bit_validez = entrada_actual->bits & mascara_validez;
        if(bit_validez == 1){
            unsigned int mascara_pid = 0xFF;
            unsigned int pid = (entrada_actual->bits >> 3) & mascara_pid;

            unsigned int mascara_VPN = 0xFFF;
            unsigned int neo_VPN = (entrada_actual->bits >> 12) & mascara_VPN;
            if(pid == id_proceso){
                printf("        [Test sup] Frame [%d], PID: (%d y busco %d) VPN: (0x%x y busco 0x%x)\n", i, pid, id_proceso, neo_VPN, VPN);
            }
            if(pid == id_proceso && VPN == neo_VPN){
                int n_frame = i;
                //printf("    [Test]Actual================\n");
                printf("        [Test] Frame [%d], validez: (%d), PID: (%d) VPN: (0x%x)\n", i, bit_validez, pid, neo_VPN);
                int check_liberar_bitmap = see_frame_bitmap(n_frame);
                if(check_liberar_bitmap != 0){
                    printf("[Test error] (Finish Process/Free TIP) Ha habido un error en liberar bitmap\n");
                    return -1;
                }
                /* int check_liberar_frame_real = liberar_frame_real(n_frame);
                if(check_liberar_frame_real != 0){
                    printf("[Test error] (Finish Process/Free TIP) Ha habido un error en liberar frame\n");
                    return -1;
                } */
            }
        }
    }
    //printf("[Test Success]: (Finish Process/Free TIP) Páginas-frames asociados a pid=(%d) y VPN=(0x%x) liberados\n", id_proceso, VPN);
    return 0; 
}
int see_memoria_proceso(Entrada_Tabla_PCB* PCB_actual){
    // Recorrer sus archivos obteniendo el VPN y offset
    char* tabla_archivos = PCB_actual->tabla_archivos;
    for(int j=0; j<10; j++){
        char valido = tabla_archivos[j*24];
        if(valido == 1){
            Entrada_Tabla_Archivos* entrada_archivo_actual = (Entrada_Tabla_Archivos*) &tabla_archivos[j*sizeof(Entrada_Tabla_Archivos)];
            //print_entrada_Archivo_valido(entrada_archivo_actual);
            //int tamaño_int = leer_5Bytes_little_endian(entrada_archivo_actual->tamaño_archivo_bytes);
            unsigned int mascara_VPN = 0XFFF;
            unsigned int mascara_offset = 0x7FFF;
            unsigned int VPN = (entrada_archivo_actual->dir_virtual >> 5) & mascara_VPN;
            unsigned int offset = (entrada_archivo_actual->dir_virtual >> 17) & mascara_offset;
            // Llamado funcion de liberar la TIP
            printf("VPN  (bits 5-16):     %u (0x%X)\n", VPN, VPN);
            printf("Offset (bits 17-31):  %u (0x%X)\n", offset, offset);
            int exitoso = see_memoria_TIP(PCB_actual->pid, VPN, offset); 
            if(exitoso != 0){
                printf("[Test error] (Finish Process) Ha habido un error al liberar TIP\n");
                return -1;
            }
        }  
    }
    //printf("[Test Success]: (Finish Process) Memoria asociada al proceso pid=(%d) liberada\n", PCB_actual->pid);
    return 0;
}

int see_process_frames(int process_id){
    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS finish process\n");
    int check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (See Process) Ha fallado colocar el puntero en la memoria\n");
        fclose(memoria_montada);
        return -1;
    }
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    size_t bytes_leidos_check = fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    if(bytes_leidos_check != entradas_tabla_PCB){
        printf("[Test error] (See Process) Ha fallado cantidad de bytes leídos\n");
        fclose(memoria_montada);
        return -1;
    }
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_PCB_actual = &TablaPCB[i];
        /* if(entrada_PCB_actual->estado == 1){
            print_entrada_PCB(entrada_PCB_actual);
        } */
        if(entrada_PCB_actual->estado == 1 && entrada_PCB_actual->pid == process_id){
            //printf("        [Test] He encontrado el proceso buscado\n");
            int exitoso = see_memoria_proceso(entrada_PCB_actual);
            if(exitoso != 0){
                printf("[Test error] (See Process) Ha ocurrido un error al liberar la memoria\n");
                fclose(memoria_montada);
                return -1;
            }
            break;
        }
    }
    fclose(memoria_montada);
    return 0; //TODO: Agregar el return 0 si está bien
}













































int os_rename_process(int process_id, char* new_name){
    memoria_montada = fopen(memoria_montada_path, "r+b");
    //printf("[Test Command]: OS rename process\n");

    int bytes_nombre = strlen(new_name) + 1;
    //printf("[Test] El nombre ingresado tiene %d bytes\n", bytes_nombre);
    if(bytes_nombre > 14){
        printf("[Test] (Rename Process) No es un largo de nombre permitido, excede los 14 Bytes\n");
        fclose(memoria_montada);
        return -1;
    }

    int check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
    if(check_puntero != 0){
        printf("[Test error] (Rename Process) Ha fallado colocar el puntero en la memoria\n");
        fclose(memoria_montada);
        return -1;
    }
    Entrada_Tabla_PCB TablaPCB[entradas_tabla_PCB];
    size_t bytes_leidos_check = fread(&TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
    if(bytes_leidos_check != entradas_tabla_PCB){
        printf("[Test error] (Rename Process) Ha fallado cantidad de bytes leídos\n");
        fclose(memoria_montada);
        return -1;
    }
    int encontrado = 0;
    for(int i=0; i<entradas_tabla_PCB; i++){
        Entrada_Tabla_PCB* entrada_PCB_actual = &TablaPCB[i];
        if(entrada_PCB_actual->estado == 1 && entrada_PCB_actual->pid == process_id){
            encontrado++;
            strcpy(entrada_PCB_actual->nombre_proceso, new_name);
            print_entrada_PCB(entrada_PCB_actual);
            break;
        }
    }
    if(encontrado != 0){
        //printf("\n Escribiendo cambios en la memoria:\n");
        check_puntero = fseek(memoria_montada, inicio_tabla_PCB, SEEK_SET);
        if(check_puntero != 0){
            printf("[Test error] (Rename Process) Ha fallado colocar el puntero en la memoria\n");
            fclose(memoria_montada);
            return -1;
        }
        size_t bytes_escritos_check = fwrite(TablaPCB, sizeof(Entrada_Tabla_PCB), entradas_tabla_PCB, memoria_montada);
        if(bytes_escritos_check != entradas_tabla_PCB){
            printf("[Test error] (Rename Process) Ha fallado cantidad de bytes escritos\n");
            fclose(memoria_montada);
            return -1;
        }
        //printf("[Test Success]: (Rename Process) Se ha modificado el nombre del proceso pid=(%d) a (%s)\n", process_id, new_name);
        fclose(memoria_montada);
        return 0;
    }
    printf("[Test error] (Rename Process) No se ha encontrado un proceso con ese id\n");
    fclose(memoria_montada);
    return -1;
}

// // funciones archivos