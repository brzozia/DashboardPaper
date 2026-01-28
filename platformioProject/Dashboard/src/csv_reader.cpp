#include "csv_reader.h"

uint8_t read_ips_from_csv(struct Data *dataToWrite, long *position)
{

    // Open file
    File file = LittleFS.open(CSV_FILE, "r");
    if (!file)
    {
        Serial.println("Failed to open HTML file for reading");
        return 1;
    }
    
    uint8_t row = 0;
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        uint8_t i = 0, el=0;
        u_int8_t startIdx = 0;
        while (row != 0 && line[i] != '\0' && line[i] != '\n')
        {
            if (line[i] == ',' && el == 0)
            {
                dataToWrite->bbTemp = atof(line.substring(startIdx, i).c_str());
                startIdx = i+1;
                el += 1;
            }
            else if (line[i] == ',' && el == 1)
            {
                dataToWrite->bbHumid = atoi(line.substring(startIdx, i).c_str());
                startIdx = i+1;
                el += 1;
            }
            else if (line[i] == ',' && el == 2)
            {
                dataToWrite->bbLight = atoi(line.substring(startIdx, i).c_str());
                startIdx = i+1;
                el += 1;
            }
            else if ((line[i] == '/0' || line[i] == '/n') && el == 3)
            {
                strcpy(dataToWrite->bbLog, line.substring(startIdx, i).c_str());
            }
            i += 1;
        }
        row += 1;
    }
    return 0;
}

