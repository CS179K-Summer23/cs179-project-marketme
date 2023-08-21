#ifndef CHECKOUTSYSTEM_H
#define CHECKOUTSYSTEM_H

#include "product.h"
#include "productDatabase.h"
#include "mainmenuhelpers.h"
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cmath>

using namespace std;
using json = nlohmann::json;

void saveTransaction(const json & transaction);
string getCurrentDate();
double calculateTax(double total);
bool isCouponValid(const json & coupon, double total);

void CheckoutSystem() {
  productDatabase manage("data/products.json");
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

  // Display cart contents
  cout << "\nYour Cart:\n";
  for (const auto & item: cart) {
    cout << item.first.getName() << " x" << item.second << " = $" << item.first.getPrice() * item.second << endl;
  }

  cout << "\nTotal: $" << total << endl;

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
    transaction["operator"] = "System"; // Modify as needed

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

double calculateTax(double total) {
  const double TAX_RATE = 0.07; // FIX ME WITH API
  return total * TAX_RATE;
}

bool isCouponValid(const json & coupon, double total) {
  string currentDate = getCurrentDate();
  if (coupon["expiry_date"] <= currentDate) return false;
  if (total < coupon["min_order_amount"]) return false;
  return true;
}

#endif // CHECKOUTSYSTEM_H