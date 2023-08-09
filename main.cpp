#include <iostream>

#include <string>

#include <algorithm>

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
void CheckoutSystem();

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
    CheckoutSystem();
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
  std::cout << "3. Back to Product Management\n";
  std::cout << "Please enter your choice (1-3): ";
  std::cin >> choice;
  switch (choice) {
  case 1:
    std::cout << "Enter Price Range (e.g., 10-20): ";
    std::cin >> filterQuery;
    std::cout <<
      "Filtering by Price Range. (This functionality is not yet implemented, please be patience.)\n";
    displayFilterMenu();
    break;
  case 2:
    std::cout << "Enter Category: ";
    std::cin >> filterQuery;
    std::cout <<
      "Filtering by Category. (This functionality is not yet implemented, please be patience.)\n";
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
    std::cout <<
      "Subscribing to Newsletter. (This functionality is not yet implemented, please be patience.)\n";
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

void CheckoutSystem() {
  productDatabase manage("data/products.json");
  string barcode, productId;
  double total = 0.0;
  vector < pair < Product, int >> cart; // Pair of Product and Quantity

  cout << "Scan or enter product details (or type :DONE or :D to finish): ";
  cin.ignore(numeric_limits < streamsize > ::max(), '\n');
  getline(cin, barcode);

  if (barcode == ":DONE" || barcode == ":D") {
    displayMainMenu(); // END EARLY
    return;
  }

  while (true) {
    if (barcode == ":DONE" || barcode == ":D") {
      break;
    }

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

      int quantity = 1; // Default quantity
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
        barcode = choice; // Treat choice as the next barcode
        continue; // Skip the rest of the loop and process the new barcode
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

  // Display cart
  sort(cart.begin(), cart.end(), [](const pair < Product, int > & a,
    const pair < Product, int > & b) {
    return a.first.getName() < b.first.getName();
  });

  cout << "\nYour Cart:\n";
  int counter = 1;
  for (const auto & item: cart) {
    cout << counter++ << ". " << item.first.getName() << " ____________________$" << item.first.getPrice() * item.second << endl;
  }

  cout << "\nTotal: $" << total << endl;

  // Coupon code
  cout << "Enter coupon code (or press Enter to skip): ";
  string coupon;
  getline(cin, coupon);

  if (!coupon.empty()) {
    json coupons;
    ifstream couponFile("coupons.json");
    couponFile >> coupons;
    couponFile.close();

    for (const auto & code: coupons["coupons"]) {
      if (code["code"] == coupon) {
        int discount = stoi(coupon.substr(coupon.size() - 2));
        total -= total * (discount / 100.0);
        cout << "Discount applied! New total: $" << total << endl;
        break;
      } else {
        cout << "Invalid Discount Code" << endl;
      }
    }
  }

  cout << "Final Total (after tax and promotions): $" << total << endl;

  cout << "Amount Paid In Full? (Y/N): ";
  getline(cin, coupon); // Use getline to handle empty input

  if (coupon == "Y" || coupon == "y") {
    for (const auto & item: cart) {
      string productId = item.first._id; // Assuming the Product class has an ID field
      int purchasedQuantity = item.second;

      bool updated = updateProduct(productId, purchasedQuantity);
      if (!updated) {
        cout << "Failed to update inventory for product: " << item.first.getName() << endl;
      }
    }
  } else if (coupon.empty() || coupon == "N" || coupon == "n") {
    cout << "Transaction Failed! Inventory unchanged!" << endl;
  } else {
    cout << "Invalid input!" << endl;
  }

  displayMainMenu();
}