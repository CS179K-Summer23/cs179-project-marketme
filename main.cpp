#include <iostream>
#include <string>
#include <algorithm>
#include "src/mainmenuhelpers.h"
#include "src/CheckoutSystem.h"
// #include "src/email.h"
#include "src/ProductSearch.h"
#include "src/report.h"
#include "src/user.h"

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
void displayReportMenu();

// GLOBAL VARIABLES
// replace string with a non-expired access code from google playground
const std::string access = "ya29.a0AfB_byArrB92uZ9t7uwnF1l83xKDCwX5tY0XL03VphjM-qoSsS9E5UlpbcrDawicBJoAnlDyB9TJhcZ8ksojb_5N5CFsSBQ7gqtUCy8T_F-MKX6dKAMETDYizOeT429sQ5Iu62p9ojrA_K0xaivyXC3g0gvWpS8b0UQ5aAsOaCgYKAYcSARESFQHsvYlsVAW3rA6lVd8yBGD0n4LI2A0175"; 
vector<User> subscribers;
ReportGenerator reportGen;

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
  std::cout << "5. Report Generator\n"; // New Function!
  std::cout << "6. Exit\n";
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
    displayReportMenu();
  case 6:
    std::cout << "Exiting the system...\n";
    reportGen.generateReport(1);
    newInventory();
    // reportEmail(access, reportGen);
    break;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displayMainMenu();
  }
}

void displayReportMenu() {
  bool isManager = true; // Placeholder for now. FIX ME
  int retryAttempts = 0; // EasterEgg

  if (!isManager) {
    std::cout << "Unavailable function. Please contact your manager for this action.\n";
    retryAttempts++;
    if (retryAttempts >= 2) {
      std::cout << "Warning: This action will be reported!\n";
      // Nah, just easter egg
    }
    displayMainMenu();
    return;
  }

  int choice;
  std::string startDate, endDate;

  std::cout << "\n=========== Report Menu ===========\n";
  std::cout << "1. Daily Report\n";
  std::cout << "2. Monthly Report\n";
  std::cout << "3. Yearly Report\n";
  std::cout << "4. All-Time Report\n";
  std::cout << "5. Custom Date Report\n";
  std::cout << "6. Custom Date Range Report\n";
  std::cout << "7. Back to Main Menu\n";
  std::cout << "Please enter your choice (1-7): ";
  std::cin >> choice;

  switch (choice) {
  case 1:
  case 2:
  case 3:
  case 4:
    reportGen.generateReport(choice);
    break;
  case 5:
    std::cout << "Enter the custom date (YYYY-MM-DD): ";
    std::cin >> startDate;
    reportGen.generateReport(choice, startDate);
    break;
  case 6:
    std::cout << "Enter the start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter the end date (YYYY-MM-DD): ";
    std::cin >> endDate;
    reportGen.generateReport(choice, startDate, endDate);
    break;
  case 7:
    displayMainMenu();
    return;
  default:
    std::cout << "Invalid choice. Please try again.\n";
    displayReportMenu();
  }

  displayMainMenu();
}

void displayProductManagementMenu() {
  int choice;
  std::cout << "\n=========== Product Management ===========\n";
  std::cout << "1. Add Product(s)\n";
  std::cout << "2. Delete Product(s)\n";
  std::cout << "3. Update a product\n";
  std::cout << "4. Advanced \n";
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

    ProductSearch productSearch("data/products.json");
    vector<json> results;

    switch (choice) {
        case 1:
            std::cout << "Enter Product ID: ";
            std::cin >> searchQuery;
            results = productSearch.searchById(searchQuery);
            break;
        case 2:
            std::cout << "Enter Product Name: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::getline(std::cin, searchQuery);
            results = productSearch.searchByName(searchQuery);
            break;
        case 3:
            std::cout << "Scan Barcode: ";
            std::cin >> searchQuery;
            results = productSearch.searchByBarcode(searchQuery);
            break;
        case 4:
            displayMainMenu();
            return;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            displaySearchMenu();
            return;
    }

    if (results.empty()) {
        std::cout << "\nNo results found.\n";
        displaySearchMenu();
        return;
    }

    for (const auto& product : results) {
        std::cout << "\nName: " << product["name"] 
                  << "\nPrice: $" << product["price"] 
                  << "\nCategory: " << product["category"] 
                  << "\nDescription: " << product["description"] 
                  << "\nExpiration Date: " << product["expiration_date"]
                  << "\nID: " << product["id"] 
                  << "\nIn Stock: " << product["quantity"] << "\n";
    }

    displaySearchMenu();
    return;
}

void displayFilterMenu() {
  int choice;
  std::string filterQuery;
  std::cout << "\n=========== Advanced Filter ===========\n";
  std::cout << "1. Filter by Price Range\n";
  std::cout << "2. Filter by Category\n";
  std::cout << "3. Sort by Name Alphabetically\n";
  std::cout << "4. Filter by Quantity Range\n";
  std::cout << "5. Filter by Prefix\n";
  std::cout << "6. Back to Product Management\n";
  choice = acceptNumber("Please enter your choice (1-6)");
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
    std::cout << "Subscribing to Newsletter...\n";
    // subscribe(access, subscribers);
    for (const auto& user : subscribers)
    {
      cout << user._ename << endl;
    }
    displayEmailMenu();
    break;
  case 2:
    std::cout << "Unsubscribing from Newsletter...\n";  
    // unsubscribe(access, subscribers);
    for (const auto& user : subscribers)
    {
      cout << user._ename << endl;
    }
    displayEmailMenu();
    break;
  case 3:
    std::cout << "Sending Emails...\n";
    // newsletter(access, subscribers);
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
