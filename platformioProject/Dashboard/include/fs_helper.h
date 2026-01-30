#include <stdio.h>
#include <LittleFS.h>

#define READ_MODE "r"
#define WRITE_MODE "w"

int8_t openFile(File* file, char* fileName, char* mode);