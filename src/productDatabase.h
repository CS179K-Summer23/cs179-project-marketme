#ifndef PRODUCTDATABASE_H
#define PRODUCTDATABASE_H

#include "product.h"

#include <iomanip>

#include <fstream>

#include <iostream>

using namespace std;
using json = nlohmann::json;

class productDatabase {
public:
  static productDatabase& getInstance(const string& database) {
    if(instancePtr == NULL){
      instancePtr = new productDatabase(database);
    }
    return *instancePtr;
    
  }

  void addProduct(Product & product);
  void updateProduct(const string & id,
    const string & option,
      const string & change);
  void delProduct(const string & id);
  void viewProduct(const string & id);
  bool exists(const string & barcode);
  string getProductIDByBarcode(const string & barcode) const;
  int getProductQuantityByID(const string & id) const;
  Product getProductDetailsByID(const string & barcode) const;
  void commitChangesToDisk();
  json getData() const;

  private: 
    productDatabase(const string& database) {
      _database = database;
      ifstream inputFile(database);
      inputFile >> products_in_memory;
      inputFile.close();
    }
    static productDatabase* instancePtr;
    string _database;
    json products_in_memory;
};

productDatabase* productDatabase::instancePtr = nullptr;

Product productDatabase::getProductDetailsByID(const string & barcode) const {
    for (const auto & product: products_in_memory["products"]) {
      if (product["barcode"] == barcode) {
        return Product(
          product["id"].get<string>(),
          product["name"].get<string>(),
          product["description"].get<string>(),
          product["price"].get<double>(),
          product["quantity"].get<int>(),
          product["category"].get<string>(),
          product["sku"].get<string>(),
          product["barcode"].get<string>(),
          product["expiration_date"].get<string>()
        );
      }
    }
    throw runtime_error("Product with barcode '" + barcode + "' does not exist in the database");
}

int productDatabase::getProductQuantityByID(const string & id) const {
  for (const auto & product: products_in_memory["products"]) {
    if (product["id"] == id) {
      return product["quantity"].get<int>();
    }
  }
  return -1;
}

string productDatabase::getProductIDByBarcode(const string & barcode) const {
  for (const auto & product: products_in_memory["products"]) {
    if (product["barcode"] == barcode) {
      return product["id"].get<string>();
    }
  }
  return "";
}

void productDatabase::addProduct(Product & product) {
  int newID = products_in_memory["products"].size() + 1;
  product.setID(to_string(newID));  
  if (!product.validate()) {
    cout << "Product failed validation. Please check the input data!" << endl;
    return;
  }

  unordered_set<string> existingProduct;
  for (const auto & product: products_in_memory["products"]) {
    existingProduct.insert(product["id"]);
  }

  string productID = product._id;
  if (existingProduct.find(productID) != existingProduct.end()) {
    cout << "Product with ID '" << productID << "' already exists." << endl;
    return;
  }

  json newProduct = product.toJson();
  products_in_memory["products"].push_back(newProduct);
  cout << "Product with ID '" << productID << "' has been added to the database." << endl;

  // ofstream outputFile(_database);
  //outputFile << products_in_memory.dump(4);  //disable live action 
}

void productDatabase::updateProduct(const string & id, const string & option, const string & change) {
  bool foundProduct = false;
  for (auto & prod: products_in_memory["products"]) {
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
    // cout << "Product with ID '" << id << "' has been updated in the database" << endl;
    // ofstream outputFile(_database);
    // outputFile << products_in_memory.dump(4); //disable live action 
  } else {
    cout << "Product with ID '" << id << "' does not exist in the database" << endl;
  }
}

void productDatabase::viewProduct(const string & id) {
  for (const auto & product: products_in_memory["products"]) {
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
  cout << "Product with ID '" << id << "' does not exist in the database" << endl;
}

void productDatabase::delProduct(const string & id) {
  bool foundProduct = false;
  for (auto it = products_in_memory["products"].begin(); it != products_in_memory["products"].end(); ++it) {
    if ((*it)["id"] == id) {
      foundProduct = true;
      it = products_in_memory["products"].erase(it);
      break;
    }
  }

  if (foundProduct) {
    // ofstream outputFile(_database);
    // outputFile << products_in_memory.dump(4);
  } else {
    cout << "Product with ID '" << id << "' does not exist in the database" << endl;
  }
}

bool productDatabase::exists(const string & barcode) {
  for (const auto & product: products_in_memory["products"]) {
    if (product["barcode"] == barcode) {
      return true;
    }
  }
  return false;
}

void productDatabase::commitChangesToDisk() {
  ofstream outputFile(_database);
  outputFile << products_in_memory.dump(4);
  cout << "Data Saved to Hard Disk" << endl;
  outputFile.close();
}

json productDatabase::getData() const{
  return products_in_memory;
}

#endif