#include <iostream>
#include <vector>
#include <chrono>
#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class Filter{
public:
    Filter(const json& data){
        _data = data;
    }

    virtual vector<json> apply() const = 0;

protected:   
    json _data;
};



class PriceRangeFilter : public Filter{
public:
    PriceRangeFilter(const json& data, const double& minPrice, const double& maxPrice) : Filter(data){
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
    ExpirationDateFilter(const json& data, const string& expirationDate)
        : Filter(data), _expirationDate(expirationDate) {
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