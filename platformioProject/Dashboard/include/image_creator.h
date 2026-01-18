// #include "../lib/GUI/GUI_Paint.h"
#include "GUI_Paint.h"
#include "dashboard.h"

#ifndef IMAGE_CREATOR_H
#define IMAGE_CREATOR_H

void initialize_dashboard(Dashboard* dashboard);
void close_dashboard(Dashboard* dashboard);

void update_dashboard(Dashboard* dashboard, struct Data* newData);

void clear_dashboard(Dashboard* dashboard);
void add_row_to_dashboard(Dashboard* dashboard);
void add_header_to_dashboard(Dashboard* dashboard);
void add_BB_status_to_dashboard(Dashboard* dashboard);
void add_footer_to_dashboard(Dashboard* dashboard);

//for testing
void save_dashboard_to_txt(Dashboard* dashboard, const char* blackImageFile, const char* redImageFile);




#endif // IMAGE_CREATOR_H

