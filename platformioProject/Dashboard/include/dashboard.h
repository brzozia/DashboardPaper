#include "DEV_Config.h"
#include "paper_data.h"
#include <stdio.h>

#ifndef DASHBOARD_H
#define DASHBOARD_H

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 800

#define TEXT_OFFSET_X 10
#define TEXT_OFFSET_Y 17

#define ROW_HEIGHT 50
#define NAME_COLUMN_START_X 1
#define IP_COLUMN_START_X 150
#define DATE_COLUMN_START_X 350
#define STATUS_COLUMN_START_X 550
#define ROWS_LINE_WIDTH DOT_PIXEL_2X2

#define HEADER_HEIGHT 70

#define FOOTER_HEIGHT 50
#define UPDATE_TIME_POS_X 500

#define BLACK_BOX_POS_Y SCREEN_HEIGHT - FOOTER_HEIGHT - ROW_HEIGHT
#define TEMP_POS_X 200
#define HUMID_POS_X 330
#define LIGHT_POS_X 500
#define BB_STATUS_POS_X 600

//Dashboard current contents will be stored in Dashboard struct
typedef struct 
{
    UBYTE* blackImage;
    UBYTE* redImage;
    struct Data* loadedData;
    UBYTE current_rows_no;
} Dashboard;

#endif // DASHBOARD_H