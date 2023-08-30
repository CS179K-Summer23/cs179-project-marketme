#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include "productDatabase.h"
#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class Filter {
public:
    Filter(productDatabase& database) : _database(database) {
        loadFromDatabase();
    }

    virtual vector<json> apply() const = 0;

protected:
    productDatabase& _database;
    json _data;
    
    void loadFromDatabase() {
        _data = _database.getData(); 
    }
};



class PriceRangeFilter : public Filter{
public:
    PriceRangeFilter(productDatabase& database, const double& minPrice, const double& maxPrice) : Filter(database){
        _minPrice = minPrice;
        _maxPrice = maxPrice;
    }

    vector<json> apply() const override{
        vector<json> filteredData;

        // Apply price range filtering logic using _data, _minPrice, and _maxPrice
        for (const auto& product : _data["products"]) {
            double price = product["price"];
            if (price >= _minPrice && price <= _maxPrice) {
                filteredData.push_back(product);
            }
        }

        return filteredData;
    }
private:
    double _minPrice;
    double _maxPrice;
};

class ExpirationDateFilter : public Filter {
public:
    ExpirationDateFilter(productDatabase& database, const string& expirationDate)
        : Filter(database), _expirationDate(expirationDate) {
    }

    vector<json> apply() const override {
        vector<json> filteredData;

        // Convert the expiration date string to a time_point
        auto targetTime = convertToDatePoint(_expirationDate);

        // Apply expiration date filtering logic using _data and targetTime
        for (const auto& product : _data["product"]) {
            string itemExpiration = product["expiration"];
            auto itemTime = convertToDatePoint(itemExpiration);

            if (itemTime <= targetTime) {
                filteredData.push_back(product);
            }
        }

        return filteredData;
    }

private:
    string _expirationDate;

    // Convert a date string to a time_point
    static chrono::system_clock::time_point convertToDatePoint(const string& dateStr) {
        // Assuming the dateStr is in the format "YYYY-MM-DD"
        struct std::tm tm = {};
        sscanf(dateStr.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;

        time_t time = mktime(&tm);
        return chrono::system_clock::from_time_t(time);
    }
};

class CategoryFilter : public Filter{
public:
    CategoryFilter(productDatabase& database, const string& name) : Filter(database){
        _name = name;
        transform(_name.begin(), _name.end(), _name.begin(), ::tolower);
    }

    vector<json> apply() const override{
        vector<json> filteredData;

        for (const auto& product : _data["products"]) {
            string name = product["category"];
            name.erase(0, name.find_first_not_of(" \t")); 
            name.erase(name.find_last_not_of(" \t") + 1); 

            transform(name.begin(), name.end(), name.begin(), ::tolower);
            // cout << "category name: " << "'" << name << "'" << endl;
            // cout << "look for name: " << "'" << _name << "'" << endl;
            if (_name == name) {
                filteredData.push_back(product);
            }
        }

        return filteredData;
    }
private:
    string _name;
};

class NameFilter : public Filter {
public:
    NameFilter(productDatabase& database) : Filter(database){

    }

    vector<json> apply() const override {
        vector<json> filteredData;

        for (const auto& product : _data["products"]) {
            filteredData.push_back(product);
        }

        sort(filteredData.begin(), filteredData.end(), [](const json& a, const json& b) {
            string nameA = a["name"];
            string nameB = b["name"];
            return nameA < nameB;
        });


        return filteredData;
    }
};

class QuantityFilter : public Filter {
public:
    QuantityFilter(productDatabase& database, const int& minQuantity, const int& maxQuantity) : Filter(database){
        _minQuantity = minQuantity;
        _maxQuantity = maxQuantity;
    }

    vector<json> apply() const override {
        vector<json> filteredData;

        // Apply price range filtering logic using _data, _minQuantity, and _maxQuantity
        for (const auto& product : _data["products"]) {
            double price = product["quantity"];
            if (price >= _minQuantity && price <= _maxQuantity) {
                filteredData.push_back(product);
            }
        }

        return filteredData;
    }
private:
    int _minQuantity;
    int _maxQuantity;
};

class PrefixFilter : public Filter {
public:
    PrefixFilter(productDatabase& database, const string& prefix) : Filter(database){
        _prefix = prefix;
        transform(_prefix.begin(), _prefix.end(), _prefix.begin(), ::tolower);
    }

    vector<json> apply() const override {
        vector<json> filteredData;

        for (const auto& product : _data["products"]) {
           string productName = product["name"];
           transform(productName.begin(), productName.end(), productName.begin(), ::tolower);

            if (productName.compare(0, _prefix.length(), _prefix) == 0) {
                filteredData.push_back(product);
            }
        }

        return filteredData;
    }

private:
    string _prefix;
};

class ExpirationDateRangeFilter : public Filter {
public:
    ExpirationDateRangeFilter (productDatabase& database, const string& startDate, const string& endDate)
        : Filter(database), _startDate(startDate), _endDate(endDate) {
    }

    vector<json> apply() const override {
        vector<json> filteredData;


        for (const auto& product : _data["products"]) {
            string expirationDate = product["expiration_date"];

            if (expirationDate >= _startDate && expirationDate <= _endDate) {
                filteredData.push_back(product);
            }
        }

        return filteredData;
    }
private:
    string _startDate;
    string _endDate;
};

class ExpiredProductsFilter : public Filter {
public:
    ExpiredProductsFilter(productDatabase& database) : Filter(database){

    }

    vector<json> apply() const override {
        vector<json> filteredData;
        auto currentTime = std::chrono::system_clock::now();

         for (const auto& product : _data["products"]) {
            tm tm = {};
            istringstream ss(product["expiration_date"].get<string>());
            ss >> std::get_time(&tm, "%Y-%m-%d");

            auto expirationTime = std::chrono::system_clock::from_time_t(std::mktime(&tm));
            if (expirationTime < currentTime) {
                filteredData.push_back(product);
            }
        }
        return filteredData;
    }
private:
};

#endif