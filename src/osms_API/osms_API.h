#pragma once
#include "../osms_File/osms_File.h"

typedef struct entrada_tabla_PCB {
    unsigned char estado;
    char nombre_proceso[14];
    unsigned char pid;
    char tabla_archivos[240];
} Entrada_Tabla_PCB;

typedef struct entrada_tabla_archivos{
    unsigned char byte_validez;
    char nombre_archivo[14];
    //unsigned int tamaño_archivo;
    char tamaño_archivo_bytes[5];
    unsigned int dir_virtual;
} Entrada_Tabla_Archivos;

typedef struct entrada_TIP{
    unsigned int bits;
} Entrada_TIP;

typedef struct frame_individual{
    unsigned int bytes;
} Frame_individual;

// funciones generales

void os_mount(char* memory_path);

void os_ls_processes();

Entrada_Tabla_PCB* Buscar_proceso(int process_id);

int os_exists(int process_id, char* file_name);

void os_ls_files(int process_id);

void os_frame_bitmap();

// // funciones procesos

int os_start_process(int process_id, char* process_name);

int os_finish_process(int process_id); // Arreglar

int os_rename_process(int process_id, char* new_name);


void print_bitmap_completo();




// // funciones archivos

// OsmsFile* os_open(int process_id, char* file_name, char mode);

// int os_read_file(osrmsFile* file desc, char* dest);

// int os_write_file(osrmsFile* file desc, char* src);

// void os_delete_file(int process id, char* file name);

// void os_close(OsmsFile* file_desc);

// BONUS

// int os_cp(int pid src, char* fname src, int pid dst, char* fname dst);

