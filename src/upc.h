#ifndef UPC_H
#define UPC_H

// Include Windows headers before curl.h
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <string>
#include <regex>
#include <chrono>
#include <curl/curl.h>
#include "mainmenuhelpers.h"
#include "productDatabase.h"

std::string inputExpirationDate();
double acceptNumber(const string & prompt);

// Callback function to handle response data
size_t UPCCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    std::string *response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

void UPC(const string& id, const string& barcode, productDatabase& manage) {
    // Initialize cURL
    CURL *curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    

    if (curl) {
        // Construct the URL for the UPCItemDB API
        std::string url = "https://api.upcitemdb.com/prod/trial/lookup?upc=" + barcode;

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UPCCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        
        } 
        else {
            
            json parse = json::parse(response);

        // Check if the "items" field exists
            if (parse.contains("items")) {
                
                const json& items = parse["items"]; // Access the array of items

                if (!items.empty()) {
                    const json& item = items[0]; // Access the first (and only) item

                    // Extract fields from the item
                    std::string name = item["title"];
                    std::string description = item["description"];
                    double price = item["lowest_recorded_price"];
                    std::string category = item["category"];
                    std::string sku;

                    if (item.contains("sku")){
                        sku = item["sku"];
                    }
                    else {
                        sku = "N/A";
                    }
                    
                    // Print the extracted information
                    std::cout << "Found item!" << std::endl;
                    std::cout << "Item Name: " << name << std::endl;
                    std::cout << "Description: " << description << std::endl;
                    std::cout << "Price: $" << price << std::endl;
                    std::cout << "Category: " << category << std::endl;
                    std::cout << "SKU: " << sku << std::endl;

                    int quantity = acceptNumber("Enter product quantity");
                    
                    std::string expirationDate = inputExpirationDate();

                    Product newProduct(id, name, description, price, quantity, category, sku, barcode, expirationDate);
                    manage.addProduct(newProduct);
                }
                
                else {
                    std::cout << "This item can't be found in the UPC Item Database!" << std::endl;
                }
            }
            else {
                std::cout << "This item can't be found in the UPC Item Database!" << std::endl;
            }
        }
        // Clean up
        curl_easy_cleanup(curl);
    }
}

std::string inputExpirationDate() {
    std::string expirationDate;

    while (true) {
        std::cout << "Enter expiration date (YYYY-MM-DD): ";
        std::cin >> expirationDate;

        // Define a regular expression pattern for the format "YYYY-MM-DD"
        std::regex pattern("\\d{4}-\\d{2}-\\d{2}");

        // Use regex_match to check if the input matches the pattern
        if (std::regex_match(expirationDate, pattern)) {
            // Parse the expiration date into a chrono::system_clock::time_point
            std::tm tm = {};
            std::istringstream ss(expirationDate);
            ss >> std::get_time(&tm, "%Y-%m-%d");

            if (!ss.fail()) {
                auto expirationTime = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                auto currentTime = std::chrono::system_clock::now();

                if (expirationTime >= currentTime) {
                    break;
                } else {
                    std::cout << "Expiration date must be in the future." << std::endl;
                }
            } else {
                std::cout << "Invalid date format. Please use YYYY-MM-DD format." << std::endl;
            }
        } else {
            std::cout << "Invalid date format. Please use YYYY-MM-DD format." << std::endl;
        }
    }

    return expirationDate;
}


double acceptNumber(const string & prompt) {
  string input;
  double number;

  while (true) {
    cout << prompt << ": ";
    cin >> input;
    cin.ignore(std::numeric_limits < std::streamsize > ::max(), '\n');

    try {
      number = stod(input);
      break; // If stod() succeeds, exit the loop   STING TO DOUBLE
    } catch (const invalid_argument & e) {
      cout << "Invalid input. Please enter a valid number." << endl;
    } catch (const out_of_range & e) {
      cout << "Invalid input. Number out of range." << endl;
    }
  }

  return number;
}

#endif //UPC_H