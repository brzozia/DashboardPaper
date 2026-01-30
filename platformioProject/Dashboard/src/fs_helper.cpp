#include "fs_helper.h"

int8_t openFile(File* file, char* fileName, char* mode){
    *file = LittleFS.open(fileName, mode);
    if (!file)
    {
        Serial.println("Failed to open HTML file for reading");
        return -1;
    }
    return 0;
}