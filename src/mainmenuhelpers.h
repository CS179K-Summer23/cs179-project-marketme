#ifndef MAINMENUHELPERS_H
#define MAINMENUHELPERS_H

#include <iostream>
#include "product.h"
#include "productDatabase.h"
#include "filter.h"
#include "upc.h"

using namespace std;

void addMenu();
void deleteProduct();
bool updateProduct(string productId = "", int requestedQuantity = -1);
void filterPriceRange();
void filterCategory();
void filterQuantityRange();
void filterPrefix();
void newInventory();

void addMenu() {
  productDatabase& manage = productDatabase::getInstance("data/products.json");
  // cout << &manage << endl;

  string id, name, barcode;
  double price;
  int quantity;

  cout << "Start scanning barcodes. Type :DONE to finish scanning." << endl;
 
  while (true) {
    cout << "Scan: ";
    getline(cin, barcode);

    while (barcode != ":DONE") {
      cout << "Scanned barcode: " << barcode << endl;
      id = barcode;

      if (manage.exists(barcode)) {
        int currentQuantity = manage.getProductQuantityByID(id);
        manage.updateProduct(id, "quantity", to_string(currentQuantity + 1)); // Add 1 to the current quantity
        cout << "Product inventory updated. Total quantity: " << currentQuantity + 1 << endl;

        cout << "Type 'U' to manually update the quantity or scan the next product: ";
        string choice;
        getline(cin, choice);

        if (choice == "U" || choice == "u") {
          int newQuantity = acceptNumber("Enter the new quantity");
          manage.updateProduct(id, "quantity", to_string(newQuantity));
          cout << "Quantity updated to: " << newQuantity << endl;
        } else {
          barcode = choice; // Treat choice as the next barcode
          continue; // Skip the rest of the loop and process the new barcode
        }

      } else {
        UPC(id, barcode, manage);
      }


      cout << "Scan: ";
      getline(cin, barcode);
      if (barcode == ":DONE") {
        break; // Double-Exit the loop when done scanning
      }
    }

    if (barcode == ":DONE") {
      break; // Exit the loop when done scanning
    }
  }
}

void deleteProduct() {
  productDatabase& manage = productDatabase::getInstance("data/products.json");
  // cout << &manage << endl;
  string productID, barcode;

  static bool skipConfirmation = false; // Flag 1 
  static bool askedBefore = false; // Flag 2      

  if (!askedBefore) {
    char choice;
    cout << "Do you want to skip confirmation on each deletion? (Y/N): ";
    cin >> choice;
    if (toupper(choice) == 'Y') {
      skipConfirmation = true;
    }
    askedBefore = true;
    cin.ignore(std::numeric_limits < std::streamsize > ::max(), '\n'); // Clear the input buffer
  }

  while (true) {
    cout << "Please scan the barcode of the product to delete (or type :OVERWRITE, O, :DONE, D or HELP for manual input by ID): ";
    getline(cin, barcode);

    if (barcode == ":OVERWRITE" || barcode == "O" || barcode == "HELP") {
      cout << "Enter Product ID: ";
      cin >> productID;
    } else if (barcode == ":DONE" || barcode == "D") {
      break; // Exit the loop
    } else {
      cout << "Scanned barcode: " << barcode << endl;
      productID = manage.getProductIDByBarcode(barcode);

      if (productID.empty()) {
        cout << "No product found with the scanned barcode." << endl;
        continue; // Go to the next iteration
      }
    }

    // Confirmation
    if (!skipConfirmation) {
      char confirmation;
      cout << "Are you sure you want to delete the product with ID '" << productID << "'? (Y/N): ";
      cin >> confirmation;

      if (toupper(confirmation) != 'Y') {
        cout << "Deletion cancelled." << endl;
        continue; // Go to the next iteration
      }
    }

    manage.delProduct(productID);
    cout << "Product with ID '" << productID << "' has been deleted." << endl;
  }
}

bool updateProduct(string productId, int requestedQuantity) {
  productDatabase& manage = productDatabase::getInstance("data/products.json");
  // cout << &manage << endl;
  string id, barcode;
  int option;
  string whatoption;
  string change = "";

  if (productId.empty()) {
    cin.ignore(std::numeric_limits < std::streamsize > ::max(), '\n'); // Clear the input buffer
    cout << "Please scan the barcode of the product to update (or type :OVERWRITE, O, or HELP for manual input by ID): ";
    getline(cin, barcode);

    if (barcode == ":OVERWRITE" || barcode == "O" || barcode == "HELP") {
      cout << "Enter Product ID: ";
      cin >> id;
    } else {
      cout << "Scanned barcode: " << barcode << endl;
      id = manage.getProductIDByBarcode(barcode);
    }
  } else {
    id = productId;
  }

  if (!manage.exists(id)) {
    cout << "ID does not exist in the inventory" << endl;
    return false;
  }

  if (requestedQuantity != -1) {
    int currentQuantity = manage.getProductQuantityByID(id);
    if (currentQuantity >= requestedQuantity) {
      manage.updateProduct(id, "quantity", to_string(currentQuantity - requestedQuantity));
      return true;
    } else {
      return false;
    }
  }

  char continueModifying = 'Y';
  while (continueModifying == 'Y' || continueModifying == 'y') {
    cout << "Choose an option " << endl;
    cout << "1. Name" << endl;
    cout << "2. Description" << endl;
    cout << "3. Category" << endl;
    cout << "4. SKU" << endl;
    cout << "5. Barcode" << endl;
    cout << "6. Expiration Date" << endl;
    cout << "7. Quantity" << endl;
    cout << "8. Price" << endl;
    cout << "9. Cancel" << endl;
    cin >> option;

    switch (option) {
    case 1:
      cout << "New name: ";
      cin.ignore();
      getline(cin, change);
      whatoption = "name";
      break;
    case 2:
      cout << "New description: ";
      cin.ignore();
      getline(cin, change);
      whatoption = "description";
      break;
    case 3:
      cout << "New category: ";
      cin.ignore();
      getline(cin, change);
      whatoption = "category";
      break;
    case 4:
      cout << "New SKU: ";
      cin >> change;
      whatoption = "SKU";
      break;
    case 5:
      cout << "New barcode: ";
      cin >> change;
      whatoption = "barcode";
      break;
    case 6:
      cout << "New expiration date: ";
      cin >> change;
      whatoption = "expiration_date";
      break;
    case 7:
      change = to_string(acceptNumber("New quantity"));
      whatoption = "quantity";
      break;
    case 8:
      change = to_string(acceptNumber("New price"));
      whatoption = "price";
      break;
    case 9:
      cout << "Canceled" << endl;
      break;
    default:
      cout << "Invalid Option" << endl;
      break;
    }

    if (change != "") {
      manage.updateProduct(id, whatoption, change);
    }

    cout << "Do you want to continue modifying? (Y/N): ";
    cin >> continueModifying;
  }

  return true; // Return true by default if no quantity check is required
}

void filterPriceRange(){
  int min = INT_MIN;
  int max = INT_MAX;

  cout << "Enter the minimum price: ";
  cin >> min;
  cout << "Enter the maximum price: ";
  cin >> max;

  if(min > max){
    cout << "Invalid option\n\n";
    return;
  }

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  PriceRangeFilter priceFilter(manage, min, max);

  vector<json> res = priceFilter.apply();

  if(res.size() == 0){
    cout << endl;
    cout << "No results found" << endl;
  }

  int count = 1;
  cout << endl;
  for(auto i : res){
    cout << count << "." << endl;
    count++;
    manage.viewProduct(i["id"]);
    cout << endl;
  }

}

void filterCategory(){
  string name;
  cout << "Enter a category name: ";
  cin >> name;
  
  productDatabase& manage = productDatabase::getInstance("data/products.json");

  CategoryFilter categoryFilter(manage, name);

  vector<json> res = categoryFilter.apply();

  if(res.size() == 0){
    cout << endl;
    cout << "No results found" << endl;
  }

  int count = 1;
  cout << endl;
  for(auto i : res){
    cout << count << "." << endl;
    count++;
    manage.viewProduct(i["id"]);
    cout << endl;
  }
}

void filterName(){
  cout << "Sorting all products alphabetically\n";

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  NameFilter nameFilter(manage);

  vector<json> res = nameFilter.apply();

  if(res.size() == 0){
    cout << endl;
    cout << "No results found" << endl;
  }

  
  int count = 1;
  cout << endl;
  for(auto i : res){
    cout << count << "." << endl;
    count++;
    manage.viewProduct(i["id"]);
    cout << endl;
  }
}

void filterQuantityRange(){
  int min = INT_MIN;
  int max = INT_MAX;

  cout << "Enter the minimum quantity: ";
  cin >> min;
  cout << "Enter the maximum quantity: ";
  cin >> max;

  if(min > max){
    cout << "Invalid option\n\n";
    return;
  }

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  QuantityFilter quantityFilter(manage, min, max);

  vector<json> res = quantityFilter.apply();

  if(res.size() == 0){
    cout << endl;
    cout << "No results found" << endl;
  }

  int count = 1;
  cout << endl;
  for(auto i : res){
    cout << count << "." << endl;
    count++;
    manage.viewProduct(i["id"]);
    cout << endl;
  }
}

void filterPrefix(){
  string prefix;
  cout << "Enter the prefix: ";
  cin >> prefix;

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  PrefixFilter prefixFilter(manage, prefix);

  vector<json> res = prefixFilter.apply();

  if(res.size() == 0){
    cout << endl;
    cout << "No results found" << endl;
  }

  
  int count = 1;
  cout << endl;
  for(auto i : res){
    cout << count << "." << endl;
    count++;
    manage.viewProduct(i["id"]);
    cout << endl;
  }
}

void newInventory(){
  productDatabase& manage = productDatabase::getInstance("data/products.json");

  manage.commitChangesToDisk();
}

#endif