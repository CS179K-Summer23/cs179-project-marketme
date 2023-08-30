#ifndef CHECKOUTSYSTEM_H
#define CHECKOUTSYSTEM_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <curl/curl.h>
#include <limits>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cctype>

#include "product.h"
#include "productDatabase.h"
#include "mainmenuhelpers.h"

#include <cctype> 

using namespace std;
using json = nlohmann::json;

void saveTransaction(const json & transaction);
string getCurrentDate();
double calculateTax(double total);
bool isCouponValid(const json & coupon, double total);

bool containsIgnoreCase(const std::string& mainString, const std::string& searchString) {
    std::string mainStringLower = mainString;
    std::string searchStringLower = searchString;
    std::transform(mainString.begin(), mainString.end(), mainStringLower.begin(), ::tolower);
    std::transform(searchString.begin(), searchString.end(), searchStringLower.begin(), ::tolower);
    return mainStringLower.find(searchStringLower) != std::string::npos;
}

void saveTransaction(const json & transaction) {
  json transactions;
  ifstream inFile("data/transactions.json");

  if (inFile.is_open()) {
    inFile >> transactions;
    inFile.close();
  }

  transactions.push_back(transaction);

  ofstream outFile("data/transactions.json");
  outFile << transactions.dump(4);
  outFile.close();
}

string getCurrentDate() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = * localtime( & now);
  strftime(buf, sizeof(buf), "%Y-%m-%d", & tstruct);
  return buf;
}

bool isValidZipCode(const std::string &zipCode) {
   return zipCode.length() == 5 && std::all_of(zipCode.begin(), zipCode.end(), ::isdigit);
}

size_t TaxCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}
double calculateTax(double total) {
  double tax = 0.00;
  std::string zipcode;

  while (true) {
      std::cout << "Enter a valid ZIP code: ";
      std::cin >> zipcode;

      if (isValidZipCode(zipcode)) {
          break;
      } else {
          std::cout << "Invalid ZIP code. Please enter a 5-digit numeric ZIP code." << std::endl;
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
  }
  std::string api_key = "Bearer 6499c341e5735fd183a5a8e76ec3a674";
  std::string api_url = "https://api.taxjar.com/v2/rates/" + zipcode;

  CURL *curl = curl_easy_init();
  CURLcode res;

  if (curl) {
      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, ("Authorization: " + api_key).c_str());

      curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      std::string response_data;
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TaxCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

      res = curl_easy_perform(curl);

      if (res != CURLE_OK) {
          std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
      } else {
          std::cout << "Response:\n" << response_data << std::endl;
      }
        json response = json::parse(response_data);
        if (response.contains("rate") && response["rate"].contains("combined_rate")) {
          std::string taxStr = response["rate"]["combined_rate"].get<std::string>();
          tax = std::stod(taxStr);
        }
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
  } else {
      std::cerr << "cURL initialization failed." << std::endl;
  }
  
  return total * tax;
}

bool isCouponValid(const json & coupon, double total) {
  string currentDate = getCurrentDate();
  if (coupon["expiry_date"] <= currentDate) return false;
  if (total < coupon["min_order_amount"]) return false;
  return true;
}

// Age less than 18: Return value is 0
// Age is 18 or older but less than 21: Return value is 2
// Age is 21 or older: Return value is 1
int checkAge() {
  string scannedCode;
  cout << "Please scan the CA DMV ID or manually enter MMYY format: ";

  cin >> scannedCode;

  if (scannedCode == "0") {
    cout << "No ID provided." << endl;
    return 0; // Return 0, indicating no ID provided
  }

  if (scannedCode.length() == 4) {
  } else if (scannedCode.length() == 16) {
    scannedCode = scannedCode.substr(12, 4); // We need the last 4 digits
  } else {
    cout << "Invalid ID format. Please provide a valid ID." << endl;
    return 0; // Invalid length/format
  }

  int month = stoi(scannedCode.substr(0, 2));
  int yearSuffix = stoi(scannedCode.substr(2, 2));

  // Safely remove for personal identity 
  scannedCode = "XXXXXXXXXXXXXXXXXXXXXXXXXXXX";

  time_t now = time(0);
  struct tm tstruct;
  tstruct = * localtime( & now);
  int currentYearSuffix = (tstruct.tm_year + 1900) % 100;
  int currentYear = tstruct.tm_year + 1900;
  int currentMonth = tstruct.tm_mon + 1;

  int year;
  if (yearSuffix <= currentYearSuffix) {
    year = 2000 + yearSuffix;
  } else {
    year = 1900 + yearSuffix;
  }

  int age = currentYear - year;
  if (month > currentMonth) {
    age--; // Adjust age if birth month is in the future 
  }

  if (age < 18) {
    return 0;
  } else if (age < 21) {
    return 2;
  } else {
    return 1;
  }
}
// Age less than 18: Return value is 0
// Age is 18 or older but less than 21: Return value is 2
// Age is 21 or older: Return value is 1

// Function to display cart contents in a tabular format
void displayCart(const vector < pair < Product, int >> & cart) {
  cout << "\nYour Cart:\n";
  cout << "-----------------------------------------------------------\n";
  cout << "| Product Name                 | Quantity | Total Price   |\n";
  cout << "-----------------------------------------------------------\n";

  for (const auto & item: cart) {
    printf("| %-28s | %-8d | $%-12.2f |\n", item.first.getName().c_str(), item.second, item.first.getPrice() * item.second);
  }

  cout << "-----------------------------------------------------------\n";
}

void CheckoutSystem() {
  //productDatabase manage("data/products.json");
  productDatabase &manage = productDatabase::getInstance("data/products.json");
  string barcode, productId;
  double total = 0.0;
  double discountValue = 0.0;
  vector < pair < Product, int >> cart;

  cout << "Scan or enter product details (or type :DONE or :D to finish): ";
  cin.ignore(numeric_limits < streamsize > ::max(), '\n');
  getline(cin, barcode);

  while (barcode != ":DONE" && barcode != ":D") {
    productId = manage.getProductIDByBarcode(barcode);

    if (productId.empty()) {
      cout << "Enter Product ID: ";
      cin >> productId;
      cin.ignore();
    }

    if (!manage.exists(productId)) {
      cout << "Product not found or out of stock!" << endl;
    } else {
      Product product = manage.getProductDetailsByID(productId);
      cout << "Scanned Product: " << product.getName() << " | Price: $" << product.getPrice() << endl;

      int quantity = 1;
      cout << "Type 'U' to manually update the quantity or scan the next product: ";
      string choice;
      getline(cin, choice);

      if (choice == "U" || choice == "u") {
        cout << "Enter quantity: ";
        cin >> quantity;
        cin.ignore();
      } else {
        cart.push_back({
          product,
          quantity
        });
        total += product.getPrice() * quantity;
        cout << "Added to cart: " << product.getName() << " x" << quantity << " = $" << product.getPrice() * quantity << endl;
        barcode = choice;
        continue;
      }

      int availableQuantity = manage.getProductQuantityByID(productId);
      if (quantity > availableQuantity) {
        cout << "Not enough stock. Available quantity: " << availableQuantity << endl;
      } else {
        cart.push_back({
          product,
          quantity
        });
        total += product.getPrice() * quantity;
        cout << "Added to cart: " << product.getName() << " x" << quantity << " = $" << product.getPrice() * quantity << endl;
      }
    }

    cout << "Scan or enter product details (or type :DONE or :D to finish): ";
    getline(cin, barcode);

  }

  displayCart(cart);

  cout << "\nTotal: $" << total << endl;

  // Age check for alcohol
  bool containsAlcohol = false;
  for (const auto& item : cart) {
        if (containsIgnoreCase(item.first.getCategory(), "alcohol")) {
            containsAlcohol = true;
            break;
        }
    }

  if (containsAlcohol) {
    if (checkAge() != 1) {
      for (const auto & item: cart) {
        if (item.first.getCategory() == "alcohol") {
          total -= item.first.getPrice() * item.second; // Subtracting the price of each alcohol item
        }
      }
      cart.erase(remove_if(cart.begin(), cart.end(), [](const pair < Product, int > & item) {
        return item.first.getCategory() == "alcohol";
      }), cart.end());
      cout << "Alcohol items removed from cart due to age restriction." << endl;
      displayCart(cart);
      cout << "\n Your New Total: $" << total << endl;
    }
  }

  cin.ignore();
  // Age check for alcohol

  // Coupon code
  int retryCount = 0;
  cout << "Enter coupon code (or press Enter to skip): ";
  string coupon;
  getline(cin, coupon);

  while (!coupon.empty() && retryCount < 10) {
    json coupons;
    ifstream couponFile("data/coupons.json");
    if (couponFile.is_open()) {
      couponFile >> coupons;
      couponFile.close();

      bool validCoupon = false;
      for (const auto & code: coupons["coupons"]) {
        if (code["code"] == coupon) {
          if (isCouponValid(code, total)) {
            discountValue = std::min(code["max_discount"].get < double > (), total * (stoi(coupon.substr(coupon.size() - 2)) / 100.0));
            total -= discountValue;
            cout << "Discount of $" << discountValue << " applied! New total: $" << total << endl;
            validCoupon = true;
            break;
          } else {
            cout << "Coupon conditions not met or coupon expired." << endl;
          }
        }
      }
      if (!validCoupon) {
        retryCount++;
        cout << "Invalid coupon. Try again or press Enter to skip (Attempt " << retryCount << "/10): ";
        getline(cin, coupon);
      } else {
        break; // Exit loop if valid coupon was found and applied.
      }
    }
  }

  if (retryCount == 10) {
    cout << "Too many invalid tries. Please try again later." << endl;
    cout << "Do you wish to proceed to checkout without the coupon? (Y/N): ";
    string choice;
    getline(cin, choice);
    if (choice == "N" || choice == "n") {
      // Handle aborting the checkout or returning to the main menu.
      return;
    }
  }

  double tax = calculateTax(total);
  tax = std::ceil(tax * 100) / 100.0; // Always round up
  cout << "Tax: $" << tax << endl;
  total += tax;
  total = std::ceil(total * 100) / 100.0; // Always round up
  cout << "Final Total (after tax and promotions): $" << total << endl;

  cout << "Amount Paid In Full? (Y/N): ";
  getline(cin, coupon);

  if (coupon == "Y" || coupon == "y") {
    for (const auto & item: cart) {
      string productId = item.first._id;
      int purchasedQuantity = item.second;

      bool updated = updateProduct(productId, purchasedQuantity);
      if (!updated) {
        cout << "Failed to update inventory for product: " << item.first.getName() << endl;
      }
    }

    // Save transaction details
    json transaction;
    transaction["date"] = getCurrentDate();
    transaction["total"] = total;
    transaction["tax"] = round(calculateTax(total) * 100) / 100.0;
    transaction["discount"] = discountValue;
    transaction["operator"] = "Default Operator";

    json itemList;
    for (const auto & item: cart) {
      json jItem;
      jItem["id"] = item.first._id;
      jItem["name"] = item.first.getName();
      jItem["quantity"] = item.second;
      jItem["price_per_item"] = item.first.getPrice();
      jItem["total_price"] = item.first.getPrice() * item.second;
      itemList.push_back(jItem);
    }
    transaction["items"] = itemList;

    saveTransaction(transaction);
  } else {
    cout << "Transaction Failed! Inventory unchanged!" << endl;
  }
}

#endif
