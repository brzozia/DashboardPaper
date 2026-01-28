#include <stdio.h>
#include <string.h>
#include <LittleFS.h>
#include "paper_data.h"

#define CSV_FILE "/blackbox.csv"

uint8_t readDataFromCsv(struct Data *data, long *position);
