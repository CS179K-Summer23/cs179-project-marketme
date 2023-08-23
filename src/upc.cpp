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
        std::string upc = "194425013742";

        // Set the path to the directory containing DLL/DYLIB files
        std::string dllPath = "../libraries"; // Works on both Windows and macOS
        
        // Construct the URL for the UPCItemDB API
        std::string url = "https://api.upcitemdb.com/prod/trial/lookup?upc=" + upc;

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

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