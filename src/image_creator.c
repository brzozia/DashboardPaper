#include "image_creator.h"
#include <stdlib.h>

void initialize_dashboard(Dashboard* dashboard)
{
    // divided by 8 because 8 pixels can be stored in one byte
    UWORD Imagesize = ((SCREEN_WIDTH % 8 == 0)? (SCREEN_WIDTH / 8 ): (SCREEN_WIDTH / 8 + 1)) * SCREEN_HEIGHT;
    
    if((dashboard->blackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((dashboard->redImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }

    Paint_NewImage(dashboard->blackImage, SCREEN_WIDTH, SCREEN_HEIGHT , 0, WHITE);
    Paint_NewImage(dashboard->redImage, SCREEN_WIDTH, SCREEN_HEIGHT , 0, WHITE);

    dashboard->current_rows_no = 0;
}

void update_dashboard(Dashboard* dashboard, struct Data* newData) 
{
    dashboard->loadedData = newData;
    dashboard->current_rows_no = 0;
    clear_dashboard(dashboard);

    add_header_to_dashboard(dashboard);
    for(UBYTE i = 0; i < ROWS; i++)
    {
        add_row_to_dashboard(dashboard);
    }
    add_BB_status_to_dashboard(dashboard);
    add_footer_to_dashboard(dashboard);

}

void clear_dashboard(Dashboard* dashboard)
{
    Paint_SelectImage(dashboard->blackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(dashboard->redImage);
    Paint_Clear(WHITE);
}

void add_row_to_dashboard(Dashboard* dashboard)
{

    Paint_SelectImage(dashboard->blackImage);
    UWORD y_pos_start = HEADER_HEIGHT + dashboard->current_rows_no * ROW_HEIGHT;
    UWORD y_pos_end = y_pos_start + ROW_HEIGHT;

    Paint_DrawRectangle(NAME_COLUMN_START_X, y_pos_start, SCREEN_WIDTH, y_pos_start+ROW_HEIGHT,
                        BLACK, ROWS_LINE_WIDTH, DRAW_FILL_EMPTY
                        );

    //Vertical separating lines
    UWORD line_positions_x[4] = {NAME_COLUMN_START_X, IP_COLUMN_START_X, DATE_COLUMN_START_X, STATUS_COLUMN_START_X};
    for(UBYTE i=0; i < 4; i++)
    {
        Paint_DrawLine(line_positions_x[i], y_pos_start, line_positions_x[i], y_pos_end,
                   BLACK, ROWS_LINE_WIDTH, LINE_STYLE_SOLID
                  );
    }
    
    
    for (size_t i = 0; i < 4; i++)
    {
        char* firstLetter = dashboard->loadedData->ipData[dashboard->current_rows_no][i];
        Paint_DrawString_EN(line_positions_x[i] + TEXT_OFFSET_X, y_pos_start + TEXT_OFFSET_Y,
                            firstLetter, &Font20, BLACK, WHITE
                            );
    }
    
    dashboard->current_rows_no += 1;
}

void add_header_to_dashboard(Dashboard* dashboard)
{

    Paint_SelectImage(dashboard->blackImage);
    Paint_DrawString_EN(TEXT_OFFSET_X, TEXT_OFFSET_Y,
                            "LAST USAGES", &Font20, BLACK, WHITE
                            );
}

void add_BB_status_to_dashboard(Dashboard* dashboard)
{
    Paint_SelectImage(dashboard->blackImage);
    // Horizontal line
    Paint_DrawLine(0, BLACK_BOX_POS_Y, SCREEN_WIDTH, BLACK_BOX_POS_Y,
                   BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID
                  );
    
    UWORD line_positions_x[4] = {TEMP_POS_X, HUMID_POS_X, LIGHT_POS_X, BB_STATUS_POS_X};
char labels[4][30] = {"TEMP:   *", "HUMIDITY:", "LIGHT", "LAST LOG:"};

    //vertical lines 
    for(UBYTE i=0; i < 4; i++)
    {
        Paint_DrawLine(line_positions_x[i], BLACK_BOX_POS_Y, line_positions_x[i], SCREEN_HEIGHT-FOOTER_HEIGHT,
                   BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID
                  );
        Paint_DrawString_EN(line_positions_x[i] + TEXT_OFFSET_X, BLACK_BOX_POS_Y + TEXT_OFFSET_Y,
                            labels[i], &Font16, BLACK, WHITE
                            );
    }

    Paint_DrawString_EN(TEXT_OFFSET_X, BLACK_BOX_POS_Y + TEXT_OFFSET_Y,
                            "BLACK BOX STATUS", &Font16, BLACK, WHITE
                            );
    

    
}

void add_footer_to_dashboard(Dashboard* dashboard)
{
    Paint_SelectImage(dashboard->blackImage);
    Paint_DrawLine(0, SCREEN_HEIGHT - FOOTER_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - FOOTER_HEIGHT,
                   BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID
                  );
    Paint_DrawString_EN(TEXT_OFFSET_X, SCREEN_HEIGHT - FOOTER_HEIGHT + TEXT_OFFSET_Y,
                            "LAST UPDATE", &Font20, BLACK, WHITE
                            );
    

    char timestr[64];
    struct tm *tm_info = localtime(&dashboard->loadedData->lastUpdate);
    if (tm_info != NULL) {
        strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);
    }
                     
    Paint_DrawString_EN(UPDATE_TIME_POS_X, SCREEN_HEIGHT - FOOTER_HEIGHT + TEXT_OFFSET_Y,
                            timestr, &Font20, BLACK, WHITE
                            );
}

void close_dashboard(Dashboard* dashboard)
{
    if(dashboard->blackImage != NULL) {
        free(dashboard->blackImage);
        dashboard->blackImage = NULL;
    }
    if(dashboard->redImage != NULL) {
        free(dashboard->redImage);
        dashboard->redImage = NULL;
    }
}

void save_dashboard_to_txt(Dashboard* dashboard, const char* blackImageFile, const char* redImageFile)
{
    // Save black image to bmp file
    FILE* blackFile = fopen(blackImageFile, "wb");
    if (blackFile != NULL) {
        fwrite(dashboard->blackImage, sizeof(UBYTE), SCREEN_WIDTH * SCREEN_HEIGHT / 8, blackFile);
        fclose(blackFile);
    } else {
        printf("Error opening black image file for writing.\n");
    }

    // Save red image to bmp file
    FILE* redFile = fopen(redImageFile, "wb");
    if (redFile != NULL) {
        fwrite(dashboard->redImage, sizeof(UBYTE), SCREEN_WIDTH * SCREEN_HEIGHT / 8, redFile);
        fclose(redFile);
    } else {
        printf("Error opening red image file for writing.\n");
    }
}