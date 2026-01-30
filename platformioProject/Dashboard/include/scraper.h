#include <stdio.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>
#include "paper_data.h"
#include "fs_helper.h"

#define SERVER_URL "http://wiener.lan/"
#define HTML_SERVER_FILE "/wiener-1.html"
#define GET_COOKIE_TRIES 5

int8_t scrapeData(boolean getFromNet, struct Data *data);