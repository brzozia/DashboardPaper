#include "csv_reader.h"

uint8_t read_ips_from_csv(struct Data* dataToWrite, long* position){

    // Open file
    FILE *fp = fopen(CSV_FILE, "r");
    if (!fp) {
        printf("Can't open file %s\n", CSV_FILE);
        return 1;
    }

    // Move the file pointer to the specified position
    if (fseek(fp, *position, SEEK_SET) != 0) {
        printf("Error seeking to position");
        fclose(fp);
        return 1;
    }

    // Read lines and save to our Data struct
    char buf[BUFFER_SIZE];
    uint8_t row = 0;
    while (fgets(buf, BUFFER_SIZE, fp) != NULL) { // reads line
        uint8_t i = 0, j = 0;
        uint8_t col = 0;
        while(buf[i] != '\0' && buf[i] != '\n'){
            if(buf[i] == ','){
                dataToWrite->ipData[row][col][++j] = '\0';
                col += 1;
                j = 0;
            } else {
                dataToWrite->ipData[row][col][j++] = buf[i];
            }
            i+=1;            
        }
        dataToWrite->ipData[row][col][++j] = '\0';
        row += 1;
    }

    dataToWrite -> lastUpdate = time(NULL);
    *position = ftell(fp);

    // Close file
    fclose(fp);
    return 0;
}
