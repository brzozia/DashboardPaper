#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
// #include "../examples/EPD_Test.h"   //Examples
#include "csv_reader.h"
#include "image_creator.h"

// #define UPLOAD_TO_EPAPER   //uncomment to compile on RPI

#ifdef UPLOAD_TO_EPAPER
#include "../lib/e-Paper/EPD_7in5b_V2.h"
#endif

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
    struct Data pseudoData;
    pseudoData.lastUpdate = time(NULL);

    strcpy(pseudoData.ipData[0][0], "Device_A");
    strcpy(pseudoData.ipData[0][1], "192.168.1.1");
    strcpy(pseudoData.ipData[0][2], "2023-01-01");
    strcpy(pseudoData.ipData[0][3], "Active");

    strcpy(pseudoData.ipData[1][0], "Device_B");
    strcpy(pseudoData.ipData[1][1], "192.168.1.2");
    strcpy(pseudoData.ipData[1][2], "2023-01-02");
    strcpy(pseudoData.ipData[1][3], "Inactive");

    strcpy(pseudoData.ipData[2][0], "Device_C");
    strcpy(pseudoData.ipData[2][1], "192.168.1.3");
    strcpy(pseudoData.ipData[2][2], "2023-01-03");
    strcpy(pseudoData.ipData[2][3], "Active");

    strcpy(pseudoData.ipData[3][0], "Device_D");
    strcpy(pseudoData.ipData[3][1], "192.168.1.4");
    strcpy(pseudoData.ipData[3][2], "2023-01-04");
    strcpy(pseudoData.ipData[3][3], "Inactive");

    Dashboard dashboard;
    initialize_dashboard(&dashboard);

    // Update the dashboard with new data
    update_dashboard(&dashboard, &pseudoData);
    save_dashboard_to_txt(&dashboard, "blackImage.txt", "redImage.txt");

    #ifdef UPLOAD_TO_EPAPER
    if(DEV_Module_Init()!=0){
        return -1;
    }

    EPD_7IN5B_V2_Init();
    EPD_7IN5B_V2_Clear();

    printf("start displaying for 2s");
    EPD_7IN5B_V2_Display(dashboard.blackImage, dashboard.redImage);
    DEV_Delay_ms(2000);  

    printf("start clearing before deep sleep");
    EPD_7IN5B_V2_Init();
    EPD_7IN5B_V2_Clear();

    printf("entering sleep mode");
    EPD_7IN5B_V2_Sleep();
    DEV_Delay_ms(2000);//important, at least 2s ()
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    #endif



    // Close the dashboard and free resources
    close_dashboard(&dashboard);

    return 0;
}
