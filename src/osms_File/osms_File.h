#pragma once

// Aquí se define la estructura de un Osms_File

typedef struct osms_file {
    unsigned char byte_validez;
    char nombre_archivo[14];
    unsigned int tamaño_archivo;
    // TODO: Convendrá colocar la dir virtual como int/char?
    unsigned int dir_virtual;
    char modo_abierto;
} OsmsFile;
