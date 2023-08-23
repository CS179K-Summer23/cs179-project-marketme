#include <iostream>
#include <curl/curl.h>

// Callback function to handle response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    std::string *response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

int main() {
    // Initialize cURL
    CURL *curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the UPC code you want to query
        std::string upc = "194425013742"; // Replace with the desired UPC code

        // Construct the URL for the UPCItemDB API
        std::string url = "https://api.upcitemdb.com/prod/trial/lookup?upc=" + upc;

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Determine the path to the dynamic library based on the operating system
        std::string dllPath;
        #ifdef _WIN32 // Windows
            dllPath = "..\\libraries\\libcurl.dll"; // Update this for Windows path
        #else // macOS or Linux
            dllPath = "../libraries/libcurl.dylib"; // Update this for macOS/Linux path
        #endif

        // Set the path to the dynamic library using CURLOPT_LIBRARY_PATH (Windows only)
        #ifdef _WIN32
            curl_easy_setopt(curl, CURLOPT_LIBRARY_PATH, dllPath.c_str());
        #endif

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Print the API response
            std::cout << "Response:\n" << response << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}