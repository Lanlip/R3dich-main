#include "common.hpp"
#include <iostream>
#include "secureget.hpp"
#include "json.hpp"
//#include "skCrypter.h"

#define CURL_STATICLIB
#pragma warning(disable: 1104)
#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Normaliz.lib")

#include "../utility/logger/logger.hpp"
#include "../utility/skCrypter.h"

extern "C" {
#include <../curl/curl.h>
}

using json = nlohmann::json;
std::string token = "";

size_t curl_callback_2(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

std::string PostJSON2(const std::string& url, const std::string& data) {
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback_2);
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

secget::Token secget::get_token(const std::string& id) {
    using authlevel = secget::Token;

    try {
        // Prepare JSON body for the POST request
        json body = {
            {std::string(skCrypt("key")), id}
        };

        std::string response = PostJSON2(std::string(skCrypt("https://freehack.zip/api/get_token")), body.dump());

        json response_json = json::parse(response, nullptr, false);

        if (response_json.is_discarded()) {
            std::cerr << skCrypt("Failed to parse JSON: ") << response << std::endl;
            return authlevel::TOKEN_ERROR;
        }

        if (response_json.contains("access_token")) {
            token = response_json["access_token"].get<std::string>();
            Logger::Instance()->Log(skCrypt("Authentication"), skCrypt("Access authorization complete :)"));
            token_true = true;
            return authlevel::TOKEN_TRUE;
        }
        else if (response_json.contains("msg")) {
            std::string msg = response_json["msg"].get<std::string>();
            std::cerr << "Error Message: " << msg << '\n';
            return authlevel::TOKEN_FALSE;
        }
        else {
            std::cerr << skCrypt("Unexpected response format. JSON does not contain 'access_token' or 'msg'.") << std::endl;
            return authlevel::TOKEN_ERROR;
        }
    }
    catch (const json::exception& e) {
        std::cerr << skCrypt("JSON parse error: ") << e.what() << '\n';
        return authlevel::TOKEN_ERROR;
    }
    catch (const std::exception& e) {
        std::cerr << skCrypt("Request failed, error: ") << e.what() << '\n';
        return authlevel::TOKEN_ERROR;
    }
}

std::string secget::Get(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    struct curl_slist* headers = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");
        std::string auth_header = "Authorization: Bearer " + token;
        headers = curl_slist_append(headers, auth_header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback_2);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_PINNEDPUBLICKEY, "sha256//dE4Hs0VhIAkNoBPQCDXbxwfRUi4Xeh55I1F999wfNdw=");

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << skCrypt("curl_easy_perform() failed: ") << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return readBuffer;
}
