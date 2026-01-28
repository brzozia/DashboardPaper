#include "scraper.h"

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
  HTTPClient http;
  http.begin(SERVER_URL);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "username=paperBot";

  Serial.println("Sending form");
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0)
  {
    Serial.print("Kod odpowiedzi HTTP: ");
    Serial.println(httpResponseCode);
    String payload = http.getString(); // TODO zapisac jako plik i czytac po linijce? nw czy ramu wystarczy inaczej
    Serial.println("--- Otrzymana treść strony ---");
    Serial.println(payload); // TODO
    Serial.println("--- Koniec treści ---");
  }
  else
  {
    Serial.print("Błąd podczas wysyłania POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
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
    scrapeFromWeb(data);
    return;
  }
  else
  {
    Serial.println("Scraping data from file...");
    scrapeFromFile(data);
    return;
  }
}
