#ifndef MAINMENUHELPERS_H
#define MAINMENUHELPERS_H

#include <iostream>
#include <unordered_set>
#include "product.h"
#include "productDatabase.h"
#include "filter.h"
#include "upc.h"
#include "report.h"

using namespace std;

void addMenu();
void deleteProduct();
bool updateProduct(string productId = "", int requestedQuantity = -1);
void filterPriceRange();
void filterCategory();
void filterQuantityRange();
void filterPrefix();
void filterExpiry();
void filterExpiredProducts();
void showCategories();
void printProducts(const vector<json>& products);
void removeExpiredProducts();
void newInventory();

void addMenu() {
  productDatabase& manage = productDatabase::getInstance("data/products.json");
  // cout << &manage << endl;

  string id, name, barcode;
  double price;
  int quantity;

  cout << "Start scanning barcodes. Or Press Enter finish scanning." << endl;
 
  while (true) {
    cout << "Scan: ";
    getline(cin, barcode);

    while (barcode != ":DONE" && barcode != "" && barcode != "DONE") {
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
      if (barcode == "") {
        break; // Double-Exit the loop when done scanning
      }
    }

    if (barcode == "") {
      break; // Exit the loop when done scanning
    }
  }
}

void addMenuManually() {
  productDatabase& manage = productDatabase::getInstance("data/products.json");

  string id, name, description, category, sku, expiration_date;
  double price;
  int quantity;
  string barcode = "NA";
  sku = "NA";


  cout << "This menu acts as a supplementary option for adding products that are not found on the mainstream database. For accurate record-keeping, we encourage you to provide as much detail as possible when entering product information." << endl;


  cout << "Enter product name: ";
  getline(cin, name);

  cout << "Enter description: ";
  getline(cin, description);

  cout << "Enter price: ";
  cin >> price;
  cin.ignore(); 

  cout << "Enter quantity: ";
  cin >> quantity;
  cin.ignore();

  cout << "Enter category: ";
  getline(cin, category);

  cout << "Enter expiration date (YYYY-MM-DD): ";
  getline(cin, expiration_date);


  Product newProduct(id, name, description, price, quantity, category, sku, barcode, expiration_date);

  // we do not validate stuff here to maximize manual overwrite capabilities
  // eg. you can input expire date as 9999-99-99 if item would not expired.

  manage.addProduct(newProduct);
  //manage.commitChangesToDisk();
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
    } else if (barcode == "" || barcode == ":DONE") {
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
  cin.ignore();

  if(min > max){
    cout << "Invalid option\n\n";
    return;
  }

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  PriceRangeFilter priceFilter(manage, min, max);

  vector<json> res = priceFilter.apply();

  printProducts(res);
}

void filterCategory(){
  showCategories();
  cout << endl;
  string name;
  cout << "Enter a category name: ";
  getline(cin, name);

  name.erase(0, name.find_first_not_of(" \t")); 
  name.erase(name.find_last_not_of(" \t") + 1); 
  
  productDatabase& manage = productDatabase::getInstance("data/products.json");

  CategoryFilter categoryFilter(manage, name);

  vector<json> res = categoryFilter.apply();

  printProducts(res);
}

void filterName(){
  cout << "Sorting all products alphabetically\n";

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  NameFilter nameFilter(manage);

  vector<json> res = nameFilter.apply();
  
  printProducts(res);
}

void filterQuantityRange(){
  int min = INT_MIN;
  int max = INT_MAX;

  cout << "Enter the minimum quantity: ";
  cin >> min;
  cout << "Enter the maximum quantity: ";
  cin >> max;
  cin.ignore();

  if(min > max){
    cout << "Invalid option\n\n";
    return;
  }

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  QuantityFilter quantityFilter(manage, min, max);

  vector<json> res = quantityFilter.apply();

  printProducts(res);
}

void filterPrefix(){
  string prefix;
  cout << "Enter the prefix: ";
  cin >> prefix;
  cin.ignore();

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  PrefixFilter prefixFilter(manage, prefix);

  vector<json> res = prefixFilter.apply();
  
  printProducts(res);  
}

void filterExpiry(){
  cout << "Enter the first expiration date: ";
  string firstExpiration = inputExpirationDate();
  cout << "Enter the second expiration date: ";
  string secondExpiration = inputExpirationDate();
  cin.ignore();

  if (firstExpiration > secondExpiration) {
    cout << "Error: The first expiration date cannot be after the second expiration date." << endl;
    return; 
  }

  productDatabase& manage = productDatabase::getInstance("data/products.json");

  ExpirationDateRangeFilter expirationdatefilter(manage, firstExpiration, secondExpiration);

  vector<json> res = expirationdatefilter.apply();

  printProducts(res);
}

void filterExpiredProducts(){
  productDatabase& manage = productDatabase::getInstance("data/products.json");

  ExpiredProductsFilter expirederoductsfilter(manage);

  vector<json> res = expirederoductsfilter.apply();

  printProducts(res);
}

void showCategories() {
  unordered_set<string> categories;
  productDatabase& manage = productDatabase::getInstance("data/products.json");
  const int colWidth = 18;

  json inventory = manage.getData();

  for (const auto& product : inventory["products"]) {
    categories.insert(product["category"]);
  }

  cout << "-" << string(colWidth * 6, '-') << endl;

  int count = 0;
  for (const auto& category : categories) {
    if (count == 3) {
      cout << "|" << endl;
      count = 0; 
    }
    cout << "|" << setw(colWidth * 2 - 1) << left << category;
      count++;
  }

  while (count != 0 && count < 3) {
    cout << setw(colWidth * 2) << "|" ;
    count++;
  }
  if (count == 3) {
    cout << "|" << endl;
  }
  
  cout << "-" << string(colWidth * 6, '-') << endl;
}



void printProducts(const vector<json>& products) {
  if(products.size() == 0){
    cout << endl;
    cout << "No results found" << endl;
    return;
  }

  const int colWidth = 18; // Adjust as needed

  // Print the header
  cout << "|" << setw(colWidth * 2) << left << "Name" << "|";
  cout << setw(colWidth) << "ID" << "|";
  cout << setw(colWidth * 2) << "Category" << "|";
  cout << setw(colWidth) << "Quantity" << "|";
  cout << setw(colWidth) << "Price" << "|";
  cout << setw(colWidth) << "Expiration Date" << "|";
  cout << setw(colWidth * 4) << "Description" << "|" << endl;

  cout << "|" << string(colWidth * 2, '-') << "|";
  cout << string(colWidth, '-') << "|";
  cout << string(colWidth * 2, '-') << "|";
  cout << string(colWidth, '-') << "|";
  cout << string(colWidth, '-') << "|";
  cout << string(colWidth, '-') << "|";
  cout << string(colWidth * 4, '-') << "|" << endl;

  int count = 0;
  int countItems = 1;
  string moreItems = "";

  for (const auto& product : products) {
    if(count >= 15){
      cout << "|" << string(colWidth * 2, '-') << "|";
      cout << string(colWidth, '-') << "|";
      cout << string(colWidth * 2, '-') << "|";
      cout << string(colWidth, '-') << "|";
      cout << string(colWidth, '-') << "|";
      cout << string(colWidth, '-') << "|";
      cout << string(colWidth * 4, '-') << "|" << endl;
      getline(cin, moreItems);
      if(moreItems.empty()){
        count = 0;
        continue;
      }
      else{
        return;
      }
      moreItems = "";
    }
    
    count++;
    string name = to_string(countItems) + ". ";
    name += product["name"];
    string id = product["id"];
    string category = product["category"];
    int quantity = product["quantity"];
    double price = product["price"];
    string expiration_date = product["expiration_date"];
    string description = product["description"];

    if(name.length() > colWidth * 2){
      name = name.substr(0, colWidth * 2 - 3) + "...";
    }

    if(category.length() > colWidth * 2){
      category = category.substr(0, colWidth * 2 - 3) + "...";
    }

    if (description.length() > colWidth * 4) {
      description = description.substr(0, colWidth * 4 - 3) + "...";
    }

    cout << "|" << setw(colWidth * 2) << left << name << "|";
    cout << setw(colWidth) << id << "|";
    cout << setw(colWidth * 2) << category << "|";
    cout << setw(colWidth) << quantity << "|";
    cout << setw(colWidth) << price << "|";
    cout << setw(colWidth) << expiration_date << "|";
    cout << setw(colWidth * 4) << description << "|" << endl;
    countItems++;
  }
}

void removeExpiredProducts(){
  ReportGenerator report;

  if(report.askToDeleteExpiredItems()){
    report.deleteExpiredItems();
  }
}

void newInventory(){
  productDatabase& manage = productDatabase::getInstance("data/products.json");

  manage.commitChangesToDisk();
}

#endif