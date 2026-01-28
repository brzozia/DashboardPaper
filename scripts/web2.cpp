#include "httplib.h"
#include <iostream>

int main() {
    httplib::Client cli("http://wiener.lan");

    // POST login
    httplib::Headers headers = {
        {"Content-Type", "application/x-www-form-urlencoded"},
        {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
        {"Origin", "http://wiener.lan"},
        {"Referer", "http://wiener.lan/"},
        {"User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:147.0) Gecko/20100101 Firefox/147.0"}
    };

    auto res = cli.Post("/", headers, "username=paperBot", "application/x-www-form-urlencoded");
    if (res) {
        std::cout << "POST status: " << res->status << "\n";
        std::string cookie;
        if (res->has_header("Set-Cookie"))
            cookie = res->get_header_value("Set-Cookie");
        std::cout << "Session cookie: " << cookie << "\n";

        // GET chronionej strony
        httplib::Headers getHeaders = {
            {"Cookie", cookie},
            {"User-Agent", "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:147.0) Gecko/20100101 Firefox/147.0"}
        };
        auto res2 = cli.Get("/", getHeaders);
        if (res2) {
            std::cout << "GET protected status: " << res2->status << "\n";
            std::cout << "--- Protected page ---\n" << res2->body << "\n";
        }
    } else {
        std::cout << "POST failed\n";
    }
}
