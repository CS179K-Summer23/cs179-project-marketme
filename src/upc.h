#ifndef UPC_H
#define UPC_H

// Include Windows headers before curl.h
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#include <iostream>
#include <curl/curl.h>
#include "mainmenuhelpers.h"
#include "productDatabase.h"

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
                    int quantity;
                    std::string category = item["category"];
                    
                    std::string sku;
                    std::string expirationDate;

                    
                    
                    if (item.contains("sku")){
                        sku = item["sku"];
                    }
                    else {
                        sku = "N/A";
                    }
                    if (item.contains("expiration_date")){
                        expirationDate = item["expiration_date"];
                    }
                    else {
                        expirationDate = "N/A";
                    }
                    
                    // Print the extracted information
                    std::cout << "Found item!" << std::endl;
                    std::cout << "Item Name: " << name << std::endl;
                    std::cout << "Description: " << description << std::endl;
                    std::cout << "Price: $" << price << std::endl;
                    std::cout << "Category: " << category << std::endl;
                    std::cout << "SKU: " << sku << std::endl;
                    std::cout << "Expiration Date: " << expirationDate << std::endl;

                    quantity = acceptNumber("Enter product quantity");

                    Product newProduct(id, name, description, price, quantity, category, sku, barcode, expirationDate);
                    manage.addProduct(newProduct);
                }
                
                else {
                std::cout << "This item can't be found in the UPC Item Database!" << std::endl;
                }
            } 
        }
        // Clean up
        curl_easy_cleanup(curl);
    }
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