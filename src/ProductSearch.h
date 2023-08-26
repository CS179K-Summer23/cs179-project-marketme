#ifndef PRODUCT_SEARCH_H
#define PRODUCT_SEARCH_H

#include "product.h"

#include "productDatabase.h"

#include "mainmenuhelpers.h"

#include <iostream>

#include <limits>

#include <vector>

#include <algorithm>

#include <ctime>

#include <cmath>

#include <algorithm>


using namespace std;
using json = nlohmann::json;

class ProductSearch {
  private: 
  json _data;
  productDatabase& _database;

  void loadFromDatabase() {
    _data = _database.getData(); 
  }

  public:
  ProductSearch(productDatabase& database) : _database(database){
    loadFromDatabase();
  }

  vector < json > searchById(const std::string & id) {
    vector < json > results;
    for (const auto & product: _data["products"]) {
      if (product["id"] == id) {
        results.push_back(product);
      }
    }
    return results;
  }

  std::string toLower(const std::string & str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }

  // dynamic programming from CS141, used gpt for debugging; hence, we do not take credit for this
  // Each cell  [i][j] in the matrix represents the cost to transform the first   [i] characters of string  A to the first  [j] characters of string B. 
  // The value in each cell is computed by taking the minimum of three operations: insertion, deletion, and substitution. 
  // By iteratively filling in the matrix, the bottom-right cell eventually gives the total minimum edit distance between the two strings.
  int edit_distance(const std::string & a,
    const std::string & b) {
    int m = a.size(), n = b.size();
    std::vector < std::vector < int >> dp(m + 1, std::vector < int > (n + 1));  

    for (int i = 0; i <= m; i++) {
      for (int j = 0; j <= n; j++) {
        if (i == 0) {
          dp[i][j] = j;
        } else if (j == 0) {
          dp[i][j] = i;
        } else if (a[i - 1] == b[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1];
        } else {
          dp[i][j] = 1 + std::min({
            dp[i - 1][j],
            dp[i][j - 1],
            dp[i - 1][j - 1]
          });
        }
      }
    }

    return dp[m][n];
  }



  vector < json > searchByName(const std::string & name) {
    vector < json > results;
    std::string loweredQuery = toLower(name);
    for (const auto & product: _data["products"]) {
      std::string loweredProductName = toLower(product["name"]);
      if (edit_distance(loweredQuery, loweredProductName) <= 1) {
        results.push_back(product);
      }
    }
    return results;
  }

  vector < json > searchByBarcode(const std::string & barcode) {
    vector < json > results;
    for (const auto & product: _data["products"]) {
      if (product["barcode"] == barcode) {
        results.push_back(product);
      }
    }
    return results;
  }
};

#endif // PRODUCT_SEARCH_H