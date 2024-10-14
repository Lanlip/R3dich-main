#include "common.hpp"
#include <iostream>
#include "auth.hpp"
#include "json.hpp"
#include "../utility/skCrypter.h"

#define CURL_STATICLIB
#pragma warning(disable: 1104)
#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Normaliz.lib")

extern "C" {
#include <../curl/curl.h>
}

using json = nlohmann::json;

// Helper function to handle curl callback
size_t curl_callback(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

// Function to send HTTP POST request and get the response
std::string PostJSON(const std::string& url, const std::string& data) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    std::string response_string;
    struct curl_slist* headers = NULL;

    if (!curl) {
        curl_global_cleanup();
        return "";
    }

    headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(curl, CURLOPT_PINNEDPUBLICKEY, "sha256//dE4Hs0VhIAkNoBPQCDXbxwfRUi4Xeh55I1F999wfNdw=");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode status = curl_easy_perform(curl);
    if (status != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        curl_global_cleanup();
        return "";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    return response_string;
}

auth::LoginStatus auth::login(const std::string& id) {
    using status = auth::LoginStatus;

    try {
        json body = {
            {std::string(skCrypt("key")), id}
        };

        std::string response = PostJSON(std::string(skCrypt("https://freehack.zip/api/keylog")), body.dump());

        if (response == "true") {
            return status::LOGIN_TRUE;
        }
        if (response == "false") {
            return status::LOGIN_FALSE;
        }
        if (response == "false-ip") {
            return status::LOGIN_FALSE_IPLOCKED;
        }
        else {
            return status::LOGIN_FALSE_ERROR;

        }

    }
    catch (const std::exception& e) {
        std::cerr << "Request failed, error: " << e.what() << '\n';
        return status::LOGIN_FALSE_ERROR;
    }
}

auth::Level auth::get_level(const std::string& id) {
    using authlevel = auth::Level;

    try {
        json body = {
            {std::string(skCrypt("key")), id}
        };

        std::string response = PostJSON(std::string(skCrypt("https://freehack.zip/api/get_authority")), body.dump());

        if (response == "partner") {
            return authlevel::LEVEL_PARTNER;
        }
        else {
            return authlevel::LEVEL_NORMAL;
        }

    }
    catch (const std::exception& e) {
        std::cerr << skCrypt("Request failed, error: ") << e.what() << '\n';
        return authlevel::LEVEL_NORMAL;
    }
}

std::string auth::getExternalIP() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, skCrypt("http://ipinfo.io/ip"));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << skCrypt("curl_easy_perform() failed: ") << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return readBuffer;
}

