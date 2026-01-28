#include <iostream>
#include <string>
#include <curl/curl.h>

// Callback do zbierania response body
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string response;
    std::string cookie;

    const std::string url = "http://wiener.lan";
    const std::string postFields = "username=paperBot";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // -------------------------
        // 1️⃣ POST login
        // -------------------------
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        headers = curl_slist_append(headers, "Origin: http://wiener.lan");
        headers = curl_slist_append(headers, "Referer: http://wiener.lan/");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:147.0) Gecko/20100101 Firefox/147.0");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // follow redirects

        // zapisz cookie do zmiennej
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt"); // tymczasowo do pliku
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "POST failed: " << curl_easy_strerror(res) << std::endl;
        else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            std::cout << "POST status: " << http_code << std::endl;
            std::cout << "Response body:\n" << response << std::endl;
        }

        // -------------------------
        // 2️⃣ GET chronionej strony
        // -------------------------
        response.clear();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "GET failed: " << curl_easy_strerror(res) << std::endl;
        else {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            std::cout << "GET protected status: " << http_code << std::endl;
            std::cout << "--- Protected page content ---\n" << response << "\n--- End ---" << std::endl;
        }

        // cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
