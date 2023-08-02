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

    bool validate() const {
        if(_name.empty() || _price < 0 || _count < 0){
            return false;
        }
        if(_description.empty()){
            _description = "NA";
        }
        return true;
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

#endif
