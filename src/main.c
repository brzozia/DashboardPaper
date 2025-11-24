#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
// #include "../examples/EPD_Test.h"   //Examples
#include "csv_reader.h"

// void Handler(int signo)
// {
//     //System Exit
//     printf("\r\nHandler:exit\r\n");
//     DEV_Module_Exit();
//     exit(0);
// }

int main(void)
{
    // Exception handling:ctrl + c
    // signal(SIGINT, Handler);

    // Init data struct
    struct Data dataToWrite;
    long readStartPoint = 0; // if new data in csv will append at the end we will read only new data. New data will override old data (from the top) - but who knows how data will be saved to csv (or not to csv??)

    // Read IPs data
    uint8_t csv_out = read_ips_from_csv(&dataToWrite, &readStartPoint);

    if(csv_out == 1){
        printf("Reading and parsing data from csv file not successfull.");
    }

    // some tests
    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLUMNS;j++){
            printf(dataToWrite.ipData[i][j]);
            printf(", ");
        }
        printf("\n");
    }


    // TODO 
    // read data from black box
    // print data do paper
    // wait some time

    // refresh paper ?

    // loop 

    
    
// #if epd7in5bV2
//     EPD_7in5b_V2_test();

// #elif epd7in5bV2_old
//     EPD_7in5b_V2_test_old();
    
// #else
//     printf("Please specify the EPD model when making. \r\n");
//     printf("Example: When you run the EPD_7in5_V2_test() program, input: sudo make clean && make EPD=epd7in5V2 \r\n");
//     printf("Don't know which program you need to run? Refer to the user manual (Wiki) and main.c \r\n");
// #endif
    
    return 0;
}
