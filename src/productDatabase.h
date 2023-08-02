#ifndef PRODUCTDATABASE_H
#define PRODUCTDATABASE_H

#include <iostream>
#include <fstream>
#include "product.h"

class productDatabase{
public:
    productDatabase(){}
    void addProduct(const Product& product, const string& database);
    void delProduct(const string& name);
};


void productDatabase::addProduct(const Product& product, const string& database){
    string inventoryPath = database;
    
    //opens file of the products
    ifstream inputFile(inventoryPath);

    //puts json structure from file to json object
    json inventory;
    inputFile >> inventory;
    inputFile.close();

    //check to see if the product already exists in the inventory
    unordered_set<string> existingProduct;

    for (const auto& product : inventory["products"]) {
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

#endif