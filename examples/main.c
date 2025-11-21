#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}

int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
#if epd7in5
    EPD_7in5_test();
    
#elif epd7in5V2
    EPD_7in5_V2_test();

#elif epd7in5V2_old
    EPD_7in5_V2_test_old();
    
#elif epd7in5bc
    EPD_7in5bc_test();
    
#elif epd7in5bV2
    EPD_7in5b_V2_test();

#elif epd7in5bV2_old
    EPD_7in5b_V2_test_old();
    
#elif epd7in5HD
    EPD_7in5_HD_test();
    
#elif epd7in5bHD
    EPD_7in5b_HD_test();
    
#else
    printf("Please specify the EPD model when making. \r\n");
    printf("Example: When you run the EPD_7in5_V2_test() program, input: sudo make clean && make EPD=epd7in5V2 \r\n");
    printf("Don't know which program you need to run? Refer to the user manual (Wiki) and main.c \r\n");
#endif
    
    return 0;
}
