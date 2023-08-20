#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class Filter{
public:
    Filter(const string& productsPath){
        _path = productsPath;
        loadFromFile();
    }

    virtual vector<json> apply() const = 0;

protected:   
    string _path;
    json _data;

    void loadFromFile() {
        ifstream pPath(_path);
        if (!pPath.is_open()) {
            cerr << "Error opening file: " << _path << endl;
            return;
        }
        pPath >> _data;
        pPath.close();
    }
};



class PriceRangeFilter : public Filter{
public:
    PriceRangeFilter(const string& productsPath, const double& minPrice, const double& maxPrice) : Filter(productsPath){
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
    ExpirationDateFilter(const string& productsPath, const string& expirationDate)
        : Filter(productsPath), _expirationDate(expirationDate) {
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
    CategoryFilter(const string& productsPath, const string& name) : Filter(productsPath){
        _name = name;
    }

    vector<json> apply() const override{
        vector<json> filteredData;

        for (const auto& product : _data["products"]) {
            string name = product["category"];
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
    NameFilter(const string& productsPath) : Filter(productsPath){

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
    QuantityFilter(const string&  productsPath, const int& minQuantity, const int& maxQuantity) : Filter(productsPath){
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