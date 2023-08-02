#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class Product {
public: 
    string _name;
    double _price;
    string _description;
    int _count;

    Product(const string& name, double price, const string& description, int count){
        _name = name;
        _price = price;
        _description = description;
        _count = count;
    }

    json toJson() const {
        return {
            {"name", _name},
            {"price", _price},
            {"description", _description},
            {"count", _count}
        };
    }
};

void addProduct(const Product& product, const string& database){
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

#endif //PRODUCT_H  