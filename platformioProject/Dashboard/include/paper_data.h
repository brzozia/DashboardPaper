#ifndef PAPER_DATA_H
#define PAPER_DATA_H

#include <time.h>
#include <stdint.h>

#define ROWS 4
#define COLUMNS 4
#define STRING_SIZE 50

#define BUFFER_SIZE 100

// Data to write on paper will be stored in Data struct
// bb* - black box status
struct Data {
    char ipData[ROWS][COLUMNS][STRING_SIZE];
    time_t lastUpdate;
    float bbTemp;
    uint8_t bbHumid;
    uint8_t bbLight;
    char bbLog[BUFFER_SIZE];
};

#endif // PAPER_DATA_H