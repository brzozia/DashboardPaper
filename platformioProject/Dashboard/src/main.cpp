// #include "../examples/EPD_Test.h"   //Examples
#include "csv_reader.h"
#include "image_creator.h"
#include "scraper.h"
#include "Debug.h"
#include <cstring>

#define TEST_CANVAS_CREATION
#define UPLOAD_TO_EPAPER // uncomment to compile on RPI

#ifdef UPLOAD_TO_EPAPER
#include <EPD_7in5b_V2.h>
#endif

boolean useWifi = false;
const char *ssid = "";
const char *password = "";

void setup(void)
{
    Serial.begin(115200);
    Serial.println("Waiting 10s before starting program...");
    delay(10000); // Wait for 10 seconds before starting program

    // Initialize littleFS
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS.");
        return;
    }

    // Connect to Wifi if needed
    if (useWifi)
    {
        WiFi.begin(ssid, password);
        Serial.print("Connecting to WiFi.");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nConnected to WiFi!");
    }
    Serial.println("Setup completed.");
}

void loop()
{
    // Create object for data and gather data
    struct Data dataToWrite;
    readDataFromCsv(&dataToWrite, 0);
    scrapeData(useWifi, &dataToWrite);
    dataToWrite.lastUpdate = time(NULL);

    Serial.println(dataToWrite.ipData[0][0]);
    Serial.println(dataToWrite.ipData[0][1]);
    Serial.println(dataToWrite.ipData[0][2]);
    
    Serial.println(dataToWrite.ipData[1][0]);
    Serial.println(dataToWrite.ipData[1][1]);
    Serial.println(dataToWrite.ipData[1][2]);

    Serial.println(dataToWrite.ipData[2][0]);
    Serial.println(dataToWrite.ipData[2][1]);
    Serial.println(dataToWrite.ipData[2][2]);

    Serial.println(dataToWrite.bbTemp);
    Serial.println(dataToWrite.bbHumid);
    Serial.println(dataToWrite.lastUpdate);

    // Create canvas and prints to dashboard
    #ifdef TEST_CANVAS_CREATION

    Dashboard dashboard;
    initialize_dashboard(&dashboard);

    // Update the dashboard with new data
    update_dashboard(&dashboard, &dataToWrite);
    // save_dashboard_to_txt(&dashboard, "blackImage.txt", "redImage.txt");
    #endif // TEST_CANVAS_CREATION

    Serial.println("Dashboard images created.");

    // #ifdef UPLOAD_TO_EPAPER
    Serial.println("Uploading to e-paper...");
    if (DEV_Module_Init() != 0)
    {
        Serial.println("Failed to initialize module.");
        return;
    }

    Serial.println("1");
    EPD_7IN5B_V2_Init();
    Serial.println("2");
    EPD_7IN5B_V2_Clear();
    Serial.println("3");

    Serial.println("Start displaying for 60s");
    EPD_7IN5B_V2_Display(dashboard.blackImage, dashboard.redImage);
    DEV_Delay_ms(60000);

    Serial.println("Start clearing before deep sleep");
    EPD_7IN5B_V2_Init();
    EPD_7IN5B_V2_Clear();

    Serial.println("Entering sleep mode");
    EPD_7IN5B_V2_Sleep();
    DEV_Delay_ms(2000); // important, at least 2s ()
    // close 5V
    Serial.println("Close 5V, Module enters 0 power consumption ...");
    DEV_Module_Exit();
    // #endif

    #ifdef TEST_CANVAS_CREATION
    // Close the dashboard and free resources
    close_dashboard(&dashboard);
    #endif // TEST_CANVAS_CREATION
    Serial.println("Looping back for next update. 30s delay...");
    delay(90000); // Wait for 90 seconds before next update
}
