#include <stdio.h>
#include <string.h>
#include "paper_data.h"
#include "fs_helper.h"

#define CSV_FILE "/blackbox.csv"

int8_t readDataFromCsv(struct Data *data, long *position);
