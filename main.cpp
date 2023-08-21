#include <iostream>
#include <string>
#include <algorithm>
#include "src/mainmenuhelpers.h"
#include "src/CheckoutSystem.h"
#include "src/email.h"

void displayMainMenu();
void displayProductManagementMenu();
void displaySearchMenu();
void displayFilterMenu();
void displayEmailMenu();
void handleProductManagement();
void handleSearch();
void handleFilter();
void handleEmailOperations();
void displayCheckoutSystem();

// GLOBAL VARIABLES
// replace string with a non-expired access code from google playground
const std::string access = "ya29.a0AfB_byBqw30auozVLlqxW4TGfTrj5mJJmcykJnDTqWLA-sBPuqO2M97fF2W4bax3PDQTKsFigNETJj93twxoWNFVO_GDdby8Zfiq5NiHPxT_plOdju4jNUGk02GBQk1d6icGU1dvptDZdrSW9_QkiEakMG-nQAUX9wfH5gaCgYKAf8SARESFQHsvYlsBxE_lutNZi2kkmTDxKD50Q0173"; 
std::string content = ""; 
std::string encode = "";


int main() {
  std::cout << "[This Version Supports Scanner, Keyboard]\n";
  std::cout << "[Advance Search; Search; Emails Functions are not yet supported. -- MarketMe-Team]\n";
  displayMainMenu();
  return 0;
}

void displayMainMenu() {
  int choice;
  std::cout << "\n=========== Main Menu ===========\n";
  std::cout << "1. Product Management\n";
  std::cout << "2. Search\n";
  std::cout << "3. Email Operations\n";
  std::cout << "4. Checkout System\n";
  std::cout << "5. Exit\n";
  std::cout << "Please enter your choice (1-5): ";
  std::cin >> choice;
  switch (choice) {
  case 1:
    handleProductManagement();
    break;
  case 2:
    handleSearch();
    break;
  case 3:
    handleEmailOperations();
    break;
  case 4:
    displayCheckoutSystem();
    break;
  case 5:
    std::cout << "Exiting the system...\n";
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displayMainMenu();
  }
}

void displayProductManagementMenu() {
  int choice;
  std::cout << "\n=========== Product Management ===========\n";
  std::cout << "1. Add Product(s)\n";
  std::cout << "2. Delete Product(s)\n";
  std::cout << "3. Update a product\n";
  std::cout << "4. Advanced Filter\n";
  std::cout << "5. Back to Main Menu\n";
  std::cout << "Please enter your choice (1-5): ";
  std::cin >> choice;
  switch (choice) {
  case 1:
    addMenu();
    displayProductManagementMenu();
    break;
  case 2:
    deleteProduct();
    displayProductManagementMenu();
    break;
  case 3:
    updateProduct();
    displayProductManagementMenu();
    break;
  case 4:
    handleFilter();
    break;
  case 5:
    displayMainMenu();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displayProductManagementMenu();
  }
}

void displaySearchMenu() {
  int choice;
  std::string searchQuery;
  std::cout << "\n=========== Search Menu ===========\n";
  std::cout << "1. Search by Product ID\n";
  std::cout << "2. Search by Product Name\n";
  std::cout << "3. Scan Barcode\n";
  std::cout << "4. Back to Main Menu\n";
  std::cout << "Please enter your choice (1-4): ";
  std::cin >> choice;
  switch (choice) {
  case 1:
    std::cout << "Enter Product ID: ";
    std::cin >> searchQuery;
    std::cout <<
      "Searching by Product ID. (This functionality is not yet implemented, please be patience.)\n";
    displaySearchMenu();
    break;
  case 2:
    std::cout << "Enter Product Name: ";
    std::cin >> searchQuery;
    std::cout <<
      "Searching by Product Name. (This functionality is not yet implemented, please be patience.)\n";
    displaySearchMenu();
    break;
  case 3:
    std::cout <<
      "Please scan the barcode. (This functionality is not yet implemented, please be patience.)\n";
    displaySearchMenu();
    break;
  case 4:
    displayMainMenu();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displaySearchMenu();
  }
}

void displayFilterMenu() {
  int choice;
  std::string filterQuery;
  std::cout << "\n=========== Advanced Filter ===========\n";
  std::cout << "1. Filter by Price Range\n";
  std::cout << "2. Filter by Category\n";
  std::cout << "3. Filter by Name Alphabetically\n";
  std::cout << "4. Filter by Quantity Range\n";
  std::cout << "5. Filter by Prefix\n";
  std::cout << "6. Back to Product Management\n";
  std::cin >> choice;
  switch (choice) {
  case 1:
    filterPriceRange();
    displayFilterMenu();
    break;
  case 2:
    filterCategory();
    displayFilterMenu();
    break;
  case 3:
    filterName();
    displayFilterMenu();
    break;
  case 4:
    filterQuantityRange();
    displayFilterMenu();
    break;
  case 5:
    filterPrefix();
    displayFilterMenu();
  case 6:
    displayProductManagementMenu();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displayFilterMenu();
  }
}

void displayEmailMenu() {
  int choice;
  std::cout << "\n=========== Email Operations ===========\n";
  std::cout << "1. Subscribe to Newsletter\n";
  std::cout << "2. Unsubscribe from Newsletter\n";
  std::cout << "3. Send Emails\n";
  std::cout << "4. Back to Main Menu\n";
  std::cout << "Please enter your choice (1-4): ";
  std::cin >> choice;
  switch (choice) {
  case 1:
    std::cout <<
      "Subscribing to Newsletter. (This functionality is not yet implemented, please be patience.)\n";
    content = "To: pcwong165@gmail.com\r\n"
              "Subject: Welcome!\r\n"
              "\r\n"
              "You have successfully subscribed to MarketMe newsletters!\r\n";
    encode = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    sendEmail(access, encode);
    content = "";
    encode = "";
    displayEmailMenu();
    break;
  case 2:
    std::cout << "Unsubscribing from Newsletter. (Functionality not yet "
    "implemented.)\n";
    content = "To: pcwong165@gmail.com\r\n"
              "Subject: We'll miss you!\r\n"
              "\r\n"
              "You have successfully unsubscribed from MarketMe.\r\n";
    encode = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    sendEmail(access, encode);
    content = "";
    encode = "";
    displayEmailMenu();
    break;
  case 3:
    std::cout << "Sending Emails. (This functionality is not yet implemented, please be patience.)\n";
    content = "To: pcwong165@gmail.com\r\n"
              "Subject: Newsletter\r\n"
              "\r\n"
              "Check out today's hot deals and items!\r\n";
    encode = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    sendEmail(access, encode);
    content = "";
    encode = "";
    displayEmailMenu();
    break;
  case 4:
    displayMainMenu();
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displayEmailMenu();
  }
}

void handleProductManagement() {
  displayProductManagementMenu();
}

void handleSearch() {
  displaySearchMenu();
}

void handleFilter() {
  displayFilterMenu();
}

void handleEmailOperations() {
  displayEmailMenu();
}

void displayCheckoutSystem() {
  CheckoutSystem();
  displayMainMenu();
}
