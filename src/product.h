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
    string _id;
    string _name;
    string _description;
    double _price;
    int _quantity;
    string _category;
    string _sku;
    string _barcode;
    string _expiration_date;  // Expiration date in "YYYY-MM-DD" format

    Product(const string& id, const string& name, const string& description, double price, 
            int quantity, const string& category, const string& sku, const string& barcode, 
            const string& expiration_date) {
        _id = id;
        _name = name;
        _description = description;
        _price = price;
        _quantity = quantity;
        _category = category;
        _sku = sku;
        _barcode = barcode;
        _expiration_date = expiration_date;
    }

    bool validate() {
        // You may add more validations based on your business rules
        if(_id.empty() || _name.empty() || _price < 0 || _quantity < 0 || 
           _category.empty() || _sku.empty() || _barcode.empty() || _expiration_date.empty()) {
            return false;
        }
        if(_description.empty()){
            _description.assign("NA");
        }
        return true;
    }

    json toJson() const {
        return {
            {"id", _id},
            {"name", _name},
            {"description", _description},
            {"price", _price},
            {"quantity", _quantity},
            {"category", _category},
            {"sku", _sku},
            {"barcode", _barcode},
            {"expiration_date", _expiration_date}
        };
    }
};

#endif
