#include "scraper.h"
#include <WiFi.h>

uint extractBetween(String *line, int searchIndex, String startTag, String endTag, String *content)
{
  int startPos = (*line).indexOf(startTag, searchIndex);
  if (startPos == -1)
  {
    *content = ""; // Start tag not found
    return -1;
  }
  startPos += startTag.length();
  int endPos = (*line).indexOf(endTag, startPos);
  if (endPos == -1)
  {
    *content = ""; // End tag not found
    return -1;
  }
  *content = (*line).substring(startPos, endPos);
  return endPos;
}

void extractTableRow(String *line, Data *data, uint8_t idx)
{
  uint8_t searchIndex = 0;
  uint8_t resultIdx = 0;
  uint8_t endPos = 0;

  String content;
  String endTag = "</td>";

  while (endPos != -1 && resultIdx < COLUMNS - 2)
  {
    endPos = extractBetween(line, searchIndex, "<td>", endTag, &content);
    searchIndex = endPos + endTag.length();
    if(endPos!=-1){
      strcpy(data->ipData[idx][resultIdx], content.c_str());
      resultIdx += 1;
    }
  }

  endPos = extractBetween(line, 0, "<td class=\"act-on\">", endTag, &content);
  if(endPos!=-1){
    strcpy(data->ipData[idx][resultIdx], content.c_str());
  }
  endPos = extractBetween(line, 0, "<td class=\"act-off\">", endTag, &content);
  if(endPos!=-1){
    strcpy(data->ipData[idx][resultIdx], content.c_str());
  }

  if(resultIdx==2){
    strcpy(data->ipData[idx][++resultIdx], "");
  }
}

void scrapeFromWeb(struct Data *data)
{ // TODO
    WiFiClient client;
  
  //pozyskanie cookie (użycie HTTPClient nie działa)
  String cookie = "";
  if (client.connect("wiener.lan", 80)) {
    client.println("POST / HTTP/1.1");
    client.println("Host: wiener.lan");
    client.println("User-Agent: ESP32");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: 10");
    client.println();
    client.println("username=paperBot");
    
    // Odpowiedź
    Serial.println("Odpowiedź serwera:");
    String line = client.readStringUntil('\n');
    while (client.connected()) {
      String body = client.readStringUntil('\n');
      Serial.println(body);
      Serial.println("koniec lini");
      
      if (body.startsWith("Set-Cookie:")) {
        cookie = body.substring(12);
        cookie.trim();
        Serial.println("Znaleziono cookie: " + cookie);
        break;
      }

      if (body == "\r") {
        Serial.println("Koniec nagłówków");
        break;
      }
    }
    client.stop();
  }


  //GET content
  HTTPClient http;
  http.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS);
  int httpResponseCode;
  if(cookie==""){
    Serial.println("No cookie obtained, aborting.");
    return;
  }



  http.begin(SERVER_URL);
  http.addHeader("Accept", "text/html");
  http.addHeader("Cookie", cookie);
  http.addHeader("Connection", "keep-alive");
  Serial.println("Getting protected content");
  httpResponseCode = http.GET();

    if(httpResponseCode != 200) {
      Serial.println("Failed to get protected content, HTTP response code: " + String(httpResponseCode));
      http.end();
      return;
    } else {
      Serial.println("Protected content retrieved successfully.");
      String payload = http.getString();
      Serial.println("--- Otrzymana treść strony ---");
      Serial.println(payload); // TODO
      Serial.println("--- Koniec treści ---");
      http.end();

      //zapis do pliku
      File file = LittleFS.open(HTML_SERVER_FILE, "w");
      if (!file)
      {
        Serial.println("Failed to open HTML file for writing");
        return;
      }
      file.print(payload);
      file.close();
      Serial.println("HTML content saved to file.");
    }

}

void scrapeFromFile(struct Data *data)
{
  File file = LittleFS.open(HTML_SERVER_FILE, "r");
  if (!file)
  {
    Serial.println("Failed to open HTML file for reading");
    return;
  }
  uint8_t idx = 0;
  boolean tableStart = false;
  while (file.available() && idx < ROWS)
  {
    String line = file.readStringUntil('\n');
    if (line.indexOf("<table class=\"log-table\">") != -1)
    {
      tableStart = true;
    }
    // Read table data
    if (tableStart && line.indexOf("<td") != -1 && idx < ROWS)
    {
      int searchPos = 0;
      while (idx < ROWS)
      {
        int trStart = line.indexOf("<tr>", searchPos);
        if (trStart == -1)
          break;
        int trEnd = line.indexOf("</tr>", trStart);
        if (trEnd == -1)
          break;

        String rowLine = line.substring(trStart, trEnd);
        // Skip the header row (the one with <th>)
        if (rowLine.indexOf("<th>") == -1)
        {
          extractTableRow(&rowLine, data, idx);
          idx += 1;
        }
        searchPos = trEnd + 5;
      }
      break;
    }
  }
  Serial.println("Reading file finished.");
  file.close();
  return;
}

void scrapeData(boolean getFromNet, struct Data *data)
{
  if (getFromNet == true && WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Scraping data from website...");
    scrapeFromWeb(data); //and save to file
    scrapeFromFile(data);
    return;
  }
  else
  {
    Serial.println("Scraping data from file...");
    scrapeFromFile(data);
    return;
  }
}
