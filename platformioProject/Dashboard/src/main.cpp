// #include "../examples/EPD_Test.h"   //Examples
// #include "csv_reader.h"
#include "image_creator.h"
#include "scraper.h"
#include "Debug.h"
#include <ArduinoJson.h>
#include <cstring>

#define CONFIG_FILE "/config.json"
#define TEST_CANVAS_CREATION
#define UPLOAD_TO_EPAPER // uncomment to compile on RPI

#ifdef UPLOAD_TO_EPAPER
#include <EPD_7in5b_V2.h>
#endif

boolean useWifi;

void listFiles() {
  Serial.println("Listing LittleFS files:");
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.print("FILE: ");
    Serial.println(file.name());
    file = root.openNextFile();
  }
}

boolean readWifiConfig(char *fileName, const char *ssid, const char *password)
{
    File file = LittleFS.open(fileName, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }
    listFiles();
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        file.close();
        return false;
    }

    const boolean useWifi = doc["wifi"]["use"];
    if (useWifi)
    {
        ssid = doc["wifi"]["ssid"];
        password = doc["wifi"]["password"];
        Serial.println("--- Data from file.json ---");
        Serial.printf("SSID: %s\n", ssid);
    }
    file.close();

    return useWifi;
}


void setup(void)
{
    Serial.begin(115200);

    // Initialize littleFS
    if (!LittleFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS.");
        return;
    }
    const char *ssid;
    const char *password;
    useWifi = readWifiConfig(CONFIG_FILE, ssid, password);

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


void loop(){
    struct Data dataToWrite;
    scrapeData(useWifi, &dataToWrite);
    delay(3000);

// #ifdef TEST_CANVAS_CREATION
//     struct Data pseudoData;
//     pseudoData.lastUpdate = time(NULL);

//     strcpy(pseudoData.ipData[0][0], "Device_A");
//     strcpy(pseudoData.ipData[0][1], "192.168.1.1");
//     strcpy(pseudoData.ipData[0][2], "2023-01-01");
//     strcpy(pseudoData.ipData[0][3], "Active");

//     strcpy(pseudoData.ipData[1][0], "Device_B");
//     strcpy(pseudoData.ipData[1][1], "192.168.1.2");
//     strcpy(pseudoData.ipData[1][2], "2023-01-02");
//     strcpy(pseudoData.ipData[1][3], "Inactive");

//     strcpy(pseudoData.ipData[2][0], "Device_C");
//     strcpy(pseudoData.ipData[2][1], "192.168.1.3");
//     strcpy(pseudoData.ipData[2][2], "2023-01-03");
//     strcpy(pseudoData.ipData[2][3], "Active");

//     strcpy(pseudoData.ipData[3][0], "Device_D");
//     strcpy(pseudoData.ipData[3][1], "192.168.1.4");
//     strcpy(pseudoData.ipData[3][2], "2023-01-04");
//     strcpy(pseudoData.ipData[3][3], "Inactive");

    Dashboard dashboard;
    initialize_dashboard(&dashboard);

    // Update the dashboard with new data
    update_dashboard(&dashboard, &dataToWrite);
save_dashboard_to_txt(&dashboard, "blackImage.txt", "redImage.txt");
// #endif // TEST_CANVAS_CREATION

    Serial.println("Dashboard images created.");

// #ifdef UPLOAD_TO_EPAPER
//     if (DEV_Module_Init() != 0)
//     {
//         Serial.println("Failed to initialize module.\r\n");
//         return;
//     }

//     Serial.println("1");
//     EPD_7IN5B_V2_Init();
//     Serial.println("2");
//     EPD_7IN5B_V2_Clear();
//     Serial.println("3");

//     Serial.println("start displaying for 2s");
//     EPD_7IN5B_V2_Display(dashboard.blackImage, dashboard.redImage);
//     DEV_Delay_ms(2000);

//     Serial.println("start clearing before deep sleep");
//     EPD_7IN5B_V2_Init();
//     EPD_7IN5B_V2_Clear();

//     Serial.println("entering sleep mode");
//     EPD_7IN5B_V2_Sleep();
//     DEV_Delay_ms(2000); // important, at least 2s ()
//     // close 5V
//     Serial.println("close 5V, Module enters 0 power consumption ...\r\n");
//     DEV_Module_Exit();
// #endif

// #ifdef TEST_CANVAS_CREATION
//     // Close the dashboard and free resources
    close_dashboard(&dashboard);
// #endif // TEST_CANVAS_CREATION
}
