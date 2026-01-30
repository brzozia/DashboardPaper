#include "scraper.h"
#include <WiFi.h>

int extractStringBetweenTags(String *line, String *content, int searchFrom, String startTag, String endTag)
{
  int startPos = (*line).indexOf(startTag, searchFrom);
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

void parseTableRow(String *line, Data *data, uint8_t rowId)
{
  uint8_t resultIdx = 0;
  int searchIndex = 0;
  int endPos = 0;

  String content;
  String endTag = "</td>";

  // Check for element between tags "<td>""</td>"
  while (endPos != -1 && resultIdx < COLUMNS - 1)
  {
    endPos = extractStringBetweenTags(line, &content, searchIndex, "<td>", endTag);
    if (endPos != -1)
    {
      strcpy(data->ipData[rowId][resultIdx], content.c_str());
      searchIndex += endPos + endTag.length();
      resultIdx += 1;
    }
  }

  // Check for element between tags "<td class=act-on/act-off>""</td>"
  endPos = extractStringBetweenTags(line, &content, 0, "<td class=\"act-on\">", endTag);
  if (endPos != -1)
  {
    strcpy(data->ipData[rowId][resultIdx], content.c_str());
  }
  endPos = extractStringBetweenTags(line, &content, 0, "<td class=\"act-off\">", endTag);
  if (endPos != -1)
  {
    strcpy(data->ipData[rowId][resultIdx], content.c_str());
  }
}

void getCookie(String *cookie)
{
  // Connect to server to get cookie needed to access webiste with wiener data
  WiFiClient client;
  if (client.connect("wiener.lan", 80))
  {
    client.println("POST / HTTP/1.1");
    client.println("Host: wiener.lan");
    client.println("User-Agent: ESP32");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: 10");
    client.println();
    client.println("username=paperBot");

    // Parse response
    Serial.println("Server response:");
    while (client.connected())
    {
      String body = client.readStringUntil('\n');
      String setCookieName = "Set-Cookie:";
      if (body.startsWith(setCookieName))
      {
        *cookie = body.substring(setCookieName.length());
        (*cookie).trim();
        Serial.println("Cookie found: " + *cookie);
        break;
      }
      if (body == "\r")
      {
        Serial.println("All headers parsed. No cookie found.");
        break;
      }
    }
    client.stop();
  }
}

void scrapeFromWeb(struct Data *data)
{
  // Tries to get cookie from server
  String cookie = "";
  uint8_t counter = 0;
  do{
    getCookie(&cookie);
  } while (cookie.isEmpty() && ++counter < GET_COOKIE_TRIES);
  
  if (cookie.isEmpty())
  {
    Serial.println("No cookie obtained, aborting.");
    return;
  }

  // GET wiener data with cookie
  int16_t httpResponseCode;
  HTTPClient http;
  http.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS);
  http.begin(SERVER_URL);
  http.addHeader("Accept", "text/html");
  http.addHeader("Cookie", cookie);
  http.addHeader("Connection", "keep-alive");

  Serial.println("Getting protected content...");
  httpResponseCode = http.GET();
  if (httpResponseCode != 200)
  {
    Serial.println("Failed to get protected content, HTTP response code: " + String(httpResponseCode));
    http.end();
    return;
  }
  else
  {
    Serial.println("Protected content retrieved successfully.");
    String payload = http.getString();
    http.end();
    // Save website content to file
    File file;
    if (openFile(&file, HTML_SERVER_FILE, WRITE_MODE) == 0 && file.available())
    {
      file.print(payload);
      file.close();
      Serial.println("HTML content saved to file.");
    }
  }
}

int8_t scrapeFromFile(struct Data *data)
{
  File file;
  if (openFile(&file, HTML_SERVER_FILE, READ_MODE) != 0)
  {
    Serial.println("Failed to open HTML file for reading");
    return -1;
  }

  // Read lines of file one by one
  uint8_t rowId = 0;
  boolean tableStarted = false;
  while (file.available() && rowId < ROWS)
  {
    String line = file.readStringUntil('\n');
    if (tableStarted) // If target table found then start processing
    {
      int16_t searchFrom = 0;
      while (rowId < ROWS)
      {
        int16_t trStart = line.indexOf("<tr>", searchFrom);
        if (trStart == -1)
        {
          break;
        }
        int16_t trEnd = line.indexOf("</tr>", trStart);
        if (trEnd != -1)
        {
          String rowLine = line.substring(trStart, trEnd);
          if (rowLine.indexOf("<th>") == -1) // Skip the header row (the one with <th>)
          {
            parseTableRow(&rowLine, data, rowId);
            rowId += 1;
          }
          searchFrom = trEnd + 5;
        }
      }
    }
    // Check if target table found
    if (!tableStarted && line.indexOf("<table class=\"log-table\">") != -1)
    {
      tableStarted = true;
    }
  }
  Serial.println("Reading file finished.");
  file.close();
  return 0;
}

int8_t scrapeData(boolean getFromNet, struct Data *data)
{
  if (getFromNet == true && WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Scraping data from website...");
    scrapeFromWeb(data); // and save to file
    return scrapeFromFile(data);
  }
  else
  {
    Serial.println("Scraping data from file...");
    return scrapeFromFile(data);
  }
}
