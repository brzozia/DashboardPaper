#include <time.h>
#include <stdint.h>

#define ROWS 4
#define COLUMNS 4

#define BUFFER_SIZE 256

// Data to write on paper will be stored in Data struct
// bb* - black box status
struct Data {
    char ipData[ROWS][COLUMNS][100];
    time_t lastUpdate;
    float bbTemp;
    uint8_t bbHumid;
    uint8_t bbLight;
    char bbLog[BUFFER_SIZE];
};
