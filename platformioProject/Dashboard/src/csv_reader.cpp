#include "csv_reader.h"

int8_t readDataFromCsv(struct Data *dataToWrite, long *position){
    File file;
    openFile(&file, CSV_FILE, READ_MODE);

    uint8_t row = 0;
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        uint8_t i = 0, el = 0, startIdx = 0;
        while (row != 0 && line.length() - 1 != i)
        {
            if (line[i] == ',' && el == 0)
            {
                dataToWrite->bbTemp = atof(line.substring(startIdx, i).c_str());
                startIdx = i + 1;
                el += 1;
            }
            else if (line[i] == ',' && el == 1)
            {
                dataToWrite->bbHumid = atoi(line.substring(startIdx, i).c_str());
                startIdx = i + 1;
                el += 1;
            }
            else if (line[i] == ',' && el == 2)
            {
                dataToWrite->bbLight = atoi(line.substring(startIdx, i).c_str());
                startIdx = i + 1;
                el += 1;
            }
            else if (el == 3)
            {
                strcpy(dataToWrite->bbLog, line.substring(startIdx, line.length() - 1).c_str());
            }
            i += 1;
        }
        row += 1;
    }
    return 0;
}
