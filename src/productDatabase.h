#ifndef PRODUCTDATABASE_H
#define PRODUCTDATABASE_H

#include <iostream>
#include <fstream>
#include "product.h"

class productDatabase {
  public: productDatabase(const string & database) {
    _database = database;
  }

  void addProduct(const Product & product);
  void delProduct(const string & name);

  private: string _database;
};

void productDatabase::addProduct(const Product & product) {
  string inventoryPath = _database;

  //opens file of the products
  ifstream inputFile(inventoryPath);

  //puts json structure from file to json object
  json inventory;
  inputFile >> inventory;
  inputFile.close();

  //check to see if the product already exists in the inventory
  unordered_set < string > existingProduct;

  for (const auto & product: inventory["products"]) {
    existingProduct.insert(product["name"]);
  }

  string productName = product._name;
  if (existingProduct.find(productName) != existingProduct.end()) {
    cout << "Product '" << productName << "' already exists." << endl;
    return;
  }

  //turns new product info to json
  json newProduct = product.toJson();

  //pushes new product into inventory json
  inventory["products"].push_back(newProduct);

  cout << "Product '" << productName << "' has been added to the inventory." << endl;

  //makes json pretty
  ofstream outputFile(inventoryPath);
  outputFile << inventory.dump(4);
}

void productDatabase::updateProduct(const Product & product) {
  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  bool foundProduct = false;

  for (auto & prod: inventory["products"]) {
    if (prod["name"] == product._name) {
      foundProduct = true;
      prod["name"] = product._name;
      prod["price"] = product._price;
      prod["description"] = product._description;
      prod["count"] = product._count;
      break;
    }
  }

  if (foundProduct) {
    cout << "Product '" << product._name << "' has been updated in the inventory" << endl;
    ofstream outputFile(inventoryPath);
    outputFile << inventory.dump(4);
  } else {
    cout << "Product '" << product._name << "' does not exist in the inventory" << endl;
  }
}

void productDatabase::viewProduct(const string & name) {
  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  for (const auto & product: inventory["products"]) {
    if (product["name"] == name) {
      cout << "Name: " << product["name"] << endl;
      cout << "Price: " << product["price"] << endl;
      cout << "Description: " << product["description"] << endl;
      cout << "Count: " << product["count"] << endl;
      return;
    }
  }

  cout << "Product '" << name << "' does not exist in the inventory" << endl;
}

void productDatabase::delProduct(const string & name) {
  string inventoryPath = _database;

  ifstream inputFile(inventoryPath);

  json inventory;
  inputFile >> inventory;
  inputFile.close();

  bool foundProduct = false;

  for (auto & product: inventory["products"]) {
    if (product["name"] == name) {
      foundProduct = true;
      inventory["products"].erase(product);
      break;
    }
  }

  if (foundProduct) {
    cout << "Product '" << name << "' has been removed from the inventory" << endl;
    ofstream outputFile(inventoryPath);
    outputFile << inventory.dump(4);
  } else {
    cout << "Product '" << name << "' does not exists in the inventory" << endl;
  }

}

#endif
