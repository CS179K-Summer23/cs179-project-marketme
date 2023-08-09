#include <iostream>
#include <string>
#include "src/mainmenuhelpers.h"

void displayMainMenu();
void displayProductManagementMenu();
void displaySearchMenu();
void displayFilterMenu();
void displayEmailMenu();
void handleProductManagement();
void handleSearch();
void handleFilter();
void handleEmailOperations();
void handleCheckoutSystem();

int main() {
  std::cout << "[Please be advised that some functions are not yet supported! -- MarketMe-Team]\n";
	std::cout << "[This Version Supports Scanner]\n";
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
    handleCheckoutSystem();
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
    std::cout
        << "Searching by Product ID. (This functionality is not yet implemented, please be patience.)\n";
    displaySearchMenu();
    break;
  case 2:
    std::cout << "Enter Product Name: ";
    std::cin >> searchQuery;
    std::cout
        << "Searching by Product Name. (This functionality is not yet implemented, please be patience.)\n";
    displaySearchMenu();
    break;
  case 3:
    std::cout
        << "Please scan the barcode. (This functionality is not yet implemented, please be patience.)\n";
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
  std::cout << "3. Back to Product Management\n";
  std::cout << "Please enter your choice (1-3): ";
  std::cin >> choice;
  switch (choice) {
  case 1:
    std::cout << "Enter Price Range (e.g., 10-20): ";
    std::cin >> filterQuery;
    std::cout
        << "Filtering by Price Range. (This functionality is not yet implemented, please be patience.)\n";
    displayFilterMenu();
    break;
  case 2:
    std::cout << "Enter Category: ";
    std::cin >> filterQuery;
    std::cout
        << "Filtering by Category. (This functionality is not yet implemented, please be patience.)\n";
    displayFilterMenu();
    break;
  case 3:
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
    std::cout
        << "Subscribing to Newsletter. (This functionality is not yet implemented, please be patience.)\n";
    displayEmailMenu();
    break;
  case 2:
    std::cout << "Unsubscribing from Newsletter. (Functionality not yet "
                 "implemented.)\n";
    displayEmailMenu();
    break;
  case 3:
    std::cout << "Sending Emails. (This functionality is not yet implemented, please be patience.)\n";
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

void handleProductManagement() { displayProductManagementMenu(); }

void handleSearch() { displaySearchMenu(); }

void handleFilter() { displayFilterMenu(); }

void handleEmailOperations() { displayEmailMenu(); }

void handleCheckoutSystem() {
  std::cout
      << "You chose Checkout System. (This functionality is not yet implemented, please be patience.)\n";
  displayMainMenu();
}
