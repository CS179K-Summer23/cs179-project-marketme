#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <iostream>

#include <fstream>

#include <string>

#include <map>

#include <vector>

#include <algorithm>

#include <ctime>

#include "productDatabase.h"

#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class ReportGenerator {
  public: ReportGenerator();
  void generateReport(int option,
    const std::string & startDate = "",
      const std::string & endDate = "");
  string getCurrentDate();
  bool askToDeleteExpiredItems();
  void deleteExpiredItems();
  map < string,
  string > getSoonExpireItems();

  private: double calculateTotal(const json & transactions);
  vector < pair < string,
  int >> getTopSellingItems(const json & transactions);
  map < string,
  int > getLowStockItems();
  int getTotalTransactions(const json & transactions);
  double getAverageBasketSize(const json & transactions);
  int getDiscountUsage(const json & transactions);
  string getReportType(int option,
    const std::string & startDate = "",
      const std::string & endDate = "");
};

ReportGenerator::ReportGenerator() {}

string ReportGenerator::getCurrentDate() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = * localtime( & now);
  strftime(buf, sizeof(buf), "%Y-%m-%d", & tstruct);
  return buf;
}

double ReportGenerator::calculateTotal(const json & transactions) {
  double total = 0.0;
  for (const auto & transaction: transactions) {
    total += transaction["total"].get < double > ();
  }
  return total;
}

string ReportGenerator::getReportType(int option,
  const std::string & startDate,
    const std::string & endDate) {
  switch (option) {
  case 1:
    return "daily";
  case 2:
    return "monthly";
  case 3:
    return "yearly";
  case 4:
    return "all-time";
  case 5:
    return "custom_" + startDate + "_";
  case 6:
    return "custom_" + startDate + "_to_" + endDate + "_";
  default:
    return "unknown";
  }
}

vector < pair < string, int >> ReportGenerator::getTopSellingItems(const json & transactions) {
  map < string, int > productCounts;
  for (const auto & transaction: transactions) {
    for (const auto & item: transaction["items"]) {
      productCounts[item["name"].get < string > ()] += item["quantity"].get < int > ();
    }
  }
  vector < pair < string, int >> productCountsVec(productCounts.begin(), productCounts.end());
  sort(productCountsVec.begin(), productCountsVec.end(), [](const pair < string, int > & a,
    const pair < string, int > & b) {
    return a.second > b.second;
  });
  if (productCountsVec.size() > 10) {
    productCountsVec.resize(10);
  }
  return productCountsVec;
}

map < string, int > ReportGenerator::getLowStockItems() {
  map < string, int > lowStockItems;
  productDatabase& manage = productDatabase::getInstance("data/products.json");
//   ifstream productsFile("data/products.json");
//   if (!productsFile.is_open()) {
//     cerr << "Failed to open products.json!" << endl;
//     return lowStockItems;
//   }
  json productsData = manage.getData();
//   productsFile >> productsData;
//   productsFile.close();
  for (const auto & product: productsData["products"]) {
    int quantity = product["quantity"].get < int > ();
    if (quantity < 5) {
      lowStockItems[product["name"].get < string > ()] = quantity;
    }
  }
  return lowStockItems;
}

map < string, string > ReportGenerator::getSoonExpireItems() {
  map < string, string > soonExpireItems;
  productDatabase & manage = productDatabase::getInstance("data/products.json");
  json productsData = manage.getData();
  string currentDate = getCurrentDate();
  tm tm1 {}, tm2 {};
  istringstream currentDateStream(currentDate);
  if (!(currentDateStream >> get_time( & tm1, "%Y-%m-%d"))) {
    cerr << "Failed to parse current date!" << endl;
    return soonExpireItems;
  }
  for (const auto & product: productsData["products"]) {
    if (product.find("expiration_date") != product.end()) {
      string expirationDate = product["expiration_date"].get < string > ();
      istringstream expirationDateStream(expirationDate);
      if (!(expirationDateStream >> get_time( & tm2, "%Y-%m-%d"))) {
        cerr << "Failed to parse expiration date for product " << product["name"].get < string > () << endl;
        continue;
      }

      time_t currentTime = mktime( & tm1);
      time_t expireTime = mktime( & tm2);
      double secondsInAMonth = 30 * 24 * 60 * 60;
      if (difftime(expireTime, currentTime) <= secondsInAMonth) {
        soonExpireItems[product["name"].get < std::string > ()] = expirationDate;
      }
    }
  }
  return soonExpireItems;
}

int ReportGenerator::getTotalTransactions(const json & transactions) {
  return transactions.size();
}

double ReportGenerator::getAverageBasketSize(const json & transactions) {
  int totalItems = 0;
  for (const auto & transaction: transactions) {
    totalItems += transaction["items"].size();
  }
  return static_cast < double > (totalItems) / transactions.size();
}

int ReportGenerator::getDiscountUsage(const json & transactions) {
  int count = 0;
  for (const auto & transaction: transactions) {
    if (transaction["discount"].get < double > () > 0) {
      count++;
    }
  }
  return count;
}

bool ReportGenerator::askToDeleteExpiredItems() {
  char choice;
  cout << "Do you want to delete all expired items? (y/n): ";
  cin >> choice;

  if (choice == 'y' || choice == 'Y') {
    return true;
  }
  return false;
}

void ReportGenerator::deleteExpiredItems() {
    productDatabase& manage = productDatabase::getInstance("data/products.json");
//   ifstream productsFile("data/products.json");
//   if (!productsFile.is_open()) {
//     cerr << "Failed to open products.json!" << endl;
//     return;
//   }

  json productsData = manage.getData();
//   productsFile >> productsData;
//   productsFile.close();

  string currentDate = getCurrentDate();
  json updatedProducts = json::array();

  tm tm1 {};
  istringstream current_date_stream(currentDate);
  current_date_stream >> get_time( & tm1, "%Y-%m-%d");
  chrono::system_clock::time_point currentTimePoint = chrono::system_clock::from_time_t(std::mktime( & tm1));

  for (const auto & product: productsData["products"]) {
    if (product.find("expiration_date") != product.end()) {
      string expirationDate = product["expiration_date"].get < string > ();
      tm tm2 {};
      istringstream expiration_date_stream(expirationDate);
      expiration_date_stream >> get_time( & tm2, "%Y-%m-%d");
      chrono::system_clock::time_point expireTimePoint = chrono::system_clock::from_time_t(std::mktime( & tm2));
      chrono::duration < double > timeDifference = currentTimePoint - expireTimePoint;
      if (timeDifference.count() > 0) {
        manage.delProduct(product["id"]);
      }
    } 
  }

//   ofstream outFile("data/products.json");
//   if (!outFile.is_open()) {
//     cerr << "Failed to open products.json for writing!" << endl;
//     return;
//   }
//   outFile << updatedProducts.dump(4);
//   outFile.close();

  cout << "Expired items deleted successfully.\n";
}

void ReportGenerator::generateReport(int option,
  const std::string & startDate,
    const std::string & endDate) {
  ifstream transactionsFile("data/transactions.json");
  if (!transactionsFile.is_open()) {
    cerr << "Failed to open transactions.json!" << endl;
    return;
  }
  json allTransactions;
  transactionsFile >> allTransactions;
  transactionsFile.close();

  // Filter transactions based on the selected option
  json filteredTransactions = json::array();
  string currentDate = getCurrentDate();
  for (const auto & transaction: allTransactions) {
    std::string transactionDate = transaction["date"].get < std::string > ();
    if (option == 1 && transactionDate == currentDate) { // Daily
      filteredTransactions.push_back(transaction);
    } else if (option == 2 && transactionDate.substr(0, 7) == currentDate.substr(0, 7)) { // Monthly
      filteredTransactions.push_back(transaction);
    } else if (option == 3 && transactionDate.substr(0, 4) == currentDate.substr(0, 4)) { // Yearly
      filteredTransactions.push_back(transaction);
    } else if (option == 4) { // All time
      filteredTransactions.push_back(transaction);
    } else if (option == 5 && transactionDate == startDate) { // Custom Date // New added
      filteredTransactions.push_back(transaction);
    } else if (option == 6 && transactionDate >= startDate && transactionDate <= endDate) { // Custom Date Range // New added
      filteredTransactions.push_back(transaction);
    }
  }

  // Calculate the total sales
  double totalSales = calculateTotal(filteredTransactions);

  // Retrieve top selling items and low stock items
  vector < pair < string, int >> topItems = getTopSellingItems(filteredTransactions);
  map < string, int > lowStock = getLowStockItems();
  auto soonExpire = getSoonExpireItems();

  // Generate the appropriate filename and title based on the selected report option
  std::string reportType = getReportType(option, startDate, endDate);
  std::string currentTime = getCurrentDate() + " " + std::to_string(time(0) % 86400); // HH:MM:SS format
  std::string filename = "report/" + reportType + "report_" + getCurrentDate() + ".txt";
  std::string title = reportType + " report";

  // Display the report on the console
  cout << "\n---- " << title << " ----\n";
  cout << "\nTotal Sales: $" << totalSales << "\n";
  cout << "\nTop Selling Items:\n";
  for (const auto & item: topItems) {
    cout << item.first << ": " << item.second << " sold\n";
  }
  cout << "\nLow Stock Items:\n";
  if (lowStock.empty()) {
    cout << "No items are running low on stock.\n";
  } else {
    for (const auto & item: lowStock) {
      cout << item.first << ": " << item.second << " in stock\n";
    }
  }

  if (!soonExpire.empty()) {
    cout << "Items Soon to Expire:\n";
    for (const auto & item: soonExpire) {
      cout << item.first << ": " << item.second << "\n";
    }
    //if(askToDeleteExpiredItems() == true){
        //deleteExpiredItems();
    //}
  } else {
    cout << "Nothing is expiring soon.\n";
  }

  cout << "\nTotal Transactions: " << getTotalTransactions(filteredTransactions) << "\n";
  cout << "Average Basket Size: " << getAverageBasketSize(filteredTransactions) << "\n";
  cout << "Discounts Applied: " << getDiscountUsage(filteredTransactions) << "\n";

  // Save the report to a file
  ofstream reportFile(filename);
  reportFile << "\n---- " << title << " ----\n";
  reportFile << "\nTotal Sales: $" << totalSales << "\n";
  reportFile << "\nTop Selling Items:\n";
  for (const auto & item: topItems) {
    reportFile << item.first << ": " << item.second << " sold\n";
  }
  reportFile << "\nLow Stock Items:\n";
  if (lowStock.empty()) {
    reportFile << "No items are running low on stock.\n";
  } else {
    for (const auto & item: lowStock) {
      reportFile << item.first << ": " << item.second << " in stock\n";
    }
  }

  if (!soonExpire.empty()) {
    reportFile << "Items Soon to Expire:\n";
    for (const auto & item: soonExpire) {
      reportFile << item.first << ": " << item.second << "\n";
    }
  } else {
    reportFile << "Nothing is expiring soon.\n";
  }

  reportFile << "\nTotal Transactions: " << getTotalTransactions(filteredTransactions) << "\n";
  reportFile << "Average Basket Size: " << getAverageBasketSize(filteredTransactions) << "\n";
  reportFile << "Discounts Applied: " << getDiscountUsage(filteredTransactions) << "\n";
  reportFile << "\nReport generated at: " << currentTime << "\n";
  reportFile << "\nNote: This file was generated automatically by the system." << "\n";
  reportFile.close();
  std::cout << title << " has been generated. Please find it under " << filename << "\n";
    
}

#endif // REPORTGENERATOR_H
