#include <stdio.h>
#include <string.h>
#include <LittleFS.h>
#include "paper_data.h"

#define CSV_FILE "/blackbox.csv"

uint8_t read_ips_from_csv(struct Data *data, long *position);
