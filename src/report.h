// ReportGenerator.h

#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <iostream>

#include <fstream>

#include <string>

#include <map>

#include <vector>

#include <algorithm>

#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class ReportGenerator {
  public: ReportGenerator();
  void generateReport(int option,
    const std::string & startDate = "",
      const std::string & endDate = "");

  private: string getCurrentDate();
  double calculateTotal(const json & transactions);
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

std::string ReportGenerator::getReportType(int option,const std::string & startDate,const std::string & endDate) {
    switch (option) {
    case 1:
        return "daily";
    case 2:
        return "monthly";
    case 3:
        return "yearly";
    case 4:
        return "all-time";
    case 5: // New added
        return "custom_" + startDate + "_";
    case 6: // New added
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

  //Convert for sorting
  vector < pair < string, int >> productCountsVec(productCounts.begin(), productCounts.end());

  //Sort
  sort(productCountsVec.begin(), productCountsVec.end(), [](const pair < string, int > & a,
    const pair < string, int > & b) {
    return a.second > b.second;
  });

  // Only contain the top 10
  if (productCountsVec.size() > 10) {
    productCountsVec.resize(10);
  }

  return productCountsVec;
}

map < string, int > ReportGenerator::getLowStockItems() {
  map < string, int > lowStockItems;

  // json reading
  ifstream productsFile("data/products.json");
  if (!productsFile.is_open()) {
    cerr << "Failed to open products.json!" << endl;
    return lowStockItems;
  }
  json productsData;
  productsFile >> productsData;
  productsFile.close();

  // find low stock items
  for (const auto & product: productsData["products"]) {
    int quantity = product["quantity"].get < int > ();
    if (quantity < 5) {
      lowStockItems[product["name"].get < string > ()] = quantity;
    }
  }

  return lowStockItems;
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

  // Generate the appropriate filename and title based on the selected report option
  std::string reportType = getReportType(option,startDate,endDate);
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

  reportFile << "\nTotal Transactions: " << getTotalTransactions(filteredTransactions) << "\n";
  reportFile << "Average Basket Size: " << getAverageBasketSize(filteredTransactions) << "\n";
  reportFile << "Discounts Applied: " << getDiscountUsage(filteredTransactions) << "\n";
  reportFile << "\nReport generated at: " << currentTime << "\n";
  reportFile << "\nNote: This file was generated automatically by the system." << "\n";
  reportFile.close();
  std::cout << title << " has been generated. Please find it under " << filename << "\n";

}

#endif // REPORTGENERATOR_H