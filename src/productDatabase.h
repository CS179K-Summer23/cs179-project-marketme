#ifndef PRODUCTDATABASE_H
#define PRODUCTDATABASE_H

#include "product.h"

#include <iomanip>

#include <fstream>

#include <iostream>

using namespace std;
using json = nlohmann::json;

class productDatabase {
  public: productDatabase(const string & database) {
    _database = database;
  }

  void addProduct(Product & product);
  void updateProduct(const string & id,
    const string & option,
      const string & change);
  void delProduct(const string & id);
  void viewProduct(const string & id);
  bool exists(const string & id);
  string getProductIDByBarcode(const string & barcode) const;
  int getProductQuantityByID(const string & id) const;
  Product getProductDetailsByID(const string & id) const;

  private: string _database;
};

Product productDatabase::getProductDetailsByID(const string & id) const {
  ifstream inputFile(_database);
  json inventory;
  inputFile >> inventory;
  inputFile.close();

  for (const auto & product: inventory["products"]) {
    if (product["id"] == id) {
      return Product(
        product["id"].get < string > (),
        product["name"].get < string > (),
        product["description"].get < string > (),
        product["price"].get < double > (),
        product["quantity"].get < int > (),
        product["category"].get < string > (),
        product["sku"].get < string > (),
        product["barcode"].get < string > (),
        product["expiration_date"].get < string > ()
      );
    }
  }
  throw runtime_error("Product with ID '" + id + "' does not exist in the inventory");
}

int productDatabase::getProductQuantityByID(const string & id) const {
  ifstream inputFile(_database);
  json inventory;
  inputFile >> inventory;
  inputFile.close();

  for (const auto & product: inventory["products"]) {
    if (product["id"] == id) {
      return product["quantity"].get < int > ();
    }
  }

  return -1; // Return -1 if no product found with the given ID
}

string productDatabase::getProductIDByBarcode(const string & barcode) const {
  ifstream inputFile(_database);
  json inventory;
  inputFile >> inventory;
  inputFile.close();

  for (const auto & product: inventory["products"]) {
    if (product["barcode"] == barcode) {
      return product["id"].get < string > ();
    }
  }

  return ""; // Return empty string if no product found with the given barcode
}

void productDatabase::addProduct(Product & product) {
  // Validate input
  if (!product.validate()) {
    cout << "Product failed validation. Please check the input data!" << endl;
    return; // End Early
  }

  cout << "Central Debug 1, Double or Int: " << product.getPrice() << endl;
  cout << "Central Debug 2.5, Double or Int: " << product._price << endl;
  //FIX ME


  string inventoryPath = _database;

  // opens file of the products
  ifstream inputFile(inventoryPath);

  // puts json structure from file to json object
  json inventory;
  inputFile >> inventory;
  inputFile.close();

  // check to see if the product already exists in the inventory
  unordered_set < string > existingProduct;

  for (const auto & product: inventory["products"]) {
    existingProduct.insert(product["id"]);
  }

  string productID = product._id;
  if (existingProduct.find(productID) != existingProduct.end()) {
    cout << "Product with ID '" << productID << "' already exists." << endl;
    return;
  }

  // turns new product info to json
  json newProduct = product.toJson();

  // pushes new product into inventory json
  inventory["products"].push_back(newProduct);

  cout << "Product with ID '" << productID << "' has been added to the inventory." <<
    endl;

  // makes json pretty
  ofstream outputFile(inventoryPath);
  outputFile << std::fixed << std::setprecision(2); // Set precision to 2 decimal places to store up to 99 cents
  outputFile << inventory.dump(4);
}

void productDatabase::updateProduct(const string & id,
  const string & option,
    const string & change) {
  // Validate input  

  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  bool foundProduct = false;

  for (auto & prod: inventory["products"]) {
    if (prod["id"] == id) {
      foundProduct = true;

      if (prod[option].is_number()) {
        if (option == "quantity") {
          prod[option] = stoi(change);
        } else if (option == "price") {
          prod[option] = stod(change);
        }
      } else {
        prod[option] = change;
      }
      break;
    }
  }

  if (foundProduct) {
    cout << "Product with ID '" << id << "' has been updated in the inventory" << endl;
    ofstream outputFile(inventoryPath);
    outputFile << inventory.dump(4);
  } else {
    cout << "Product with ID '" << id << "' does not exist in the inventory" << endl;
  }
}

void productDatabase::viewProduct(const string & id) {
  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  for (const auto & product: inventory["products"]) {
    if (product["id"] == id) {
      cout << "ID: " << product["id"] << endl;
      cout << "Name: " << product["name"] << endl;
      cout << "Description: " << product["description"] << endl;
      cout << "Price: " << product["price"] << endl;
      cout << "Quantity: " << product["quantity"] << endl;
      cout << "Category: " << product["category"] << endl;
      cout << "SKU: " << product["sku"] << endl;
      cout << "Barcode: " << product["barcode"] << endl;
      cout << "Expiration Date: " << product["expiration_date"] << endl;
      return;
    }
  }

  cout << "Product with ID '" << id << "' does not exist in the inventory" << endl;
}

void productDatabase::delProduct(const string & id) {
  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  bool foundProduct = false;

  for (auto it = inventory["products"].begin(); it != inventory["products"].end(); ++it) {
    if (( * it)["id"] == id) {
      foundProduct = true;
      it = inventory["products"].erase(it);
      it--;
      break;
    }
  }

  if (foundProduct) {
    ofstream outputFile(inventoryPath);
    outputFile << inventory.dump(4);
  } else {
    cout << "Product with ID '" << id << "' does not exist in the inventory" << endl;
  }
}

bool productDatabase::exists(const string & id) {
  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  for (const auto & product: inventory["products"]) {
    if (product["id"] == id) {
      return true;
    }
  }
  return false;
}

#endif