#include <stdio.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LittleFS.h>
#include <string.h>
#include "paper_data.h"

#define SERVER_URL "http://wiener.lan/"
#define HTML_SERVER_FILE "/wiener-1.html"

void scrapeData(boolean getFromNet, struct Data *data);