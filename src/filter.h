#include <iostream>
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
    PriceRangeFilter(const json& data, double minPrice, double maxPrice) : Filter(data){
        _minPrice = minPrice;
        _maxPrice = maxPrice;
    }

    vector<json> apply() const override{
        vector<json> filteredData;

        // Apply price range filtering logic using _data, _minPrice, and _maxPrice
        for (const auto& item : _data["products"]) {
            double price = item["price"];
            if (price >= _minPrice && price <= _maxPrice) {
                filteredData.push_back(item);
            }
        }

        return filteredData;
    }
private:
    double _minPrice;
    double _maxPrice;
};