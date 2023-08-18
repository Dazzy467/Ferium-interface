#pragma once
#include <iostream>
#include <string>
#include <curl/curl.h>


namespace modrinth
{

size_t WriteCallback(void* ptr, size_t size, size_t count, void* stream) {
    std::string* responseData = static_cast<std::string*>(stream);
    responseData->append(static_cast<char*>(ptr), size * count);
    return size * count;
}

std::string HttpGet(const std::string& url) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "cURL initialization failed." << std::endl;
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Disable SSL verification

    std::string responseData;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }

    curl_easy_cleanup(curl);

    return responseData;
}
}