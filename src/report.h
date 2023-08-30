#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>

#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class ReportGenerator {
public:
    ReportGenerator();
    void generateReport(int option, const std::string &startDate = "", const std::string &endDate = "");
    string getCurrentDate();
    bool askToDeleteExpiredItems();
    void deleteExpiredItems();

private:
    double calculateTotal(const json &transactions);
    vector<pair<string, int>> getTopSellingItems(const json &transactions);
    map<string, int> getLowStockItems();
    map<string, string> getSoonExpireItems();
    int getTotalTransactions(const json &transactions);
    double getAverageBasketSize(const json &transactions);
    int getDiscountUsage(const json &transactions);
    string getReportType(int option, const std::string &startDate = "", const std::string &endDate = "");
};

ReportGenerator::ReportGenerator() {}

string ReportGenerator::getCurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return buf;
}

double ReportGenerator::calculateTotal(const json &transactions) {
    double total = 0.0;
    for (const auto &transaction: transactions) {
        total += transaction["total"].get<double>();
    }
    return total;
}

string ReportGenerator::getReportType(int option, const std::string &startDate, const std::string &endDate) {
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

vector<pair<string, int>> ReportGenerator::getTopSellingItems(const json &transactions) {
    map<string, int> productCounts;
    for (const auto &transaction: transactions) {
        for (const auto &item: transaction["items"]) {
            productCounts[item["name"].get<string>()] += item["quantity"].get<int>();
        }
    }
    vector<pair<string, int>> productCountsVec(productCounts.begin(), productCounts.end());
    sort(productCountsVec.begin(), productCountsVec.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
        return a.second > b.second;
    });
    if (productCountsVec.size() > 10) {
        productCountsVec.resize(10);
    }
    return productCountsVec;
}

map<string, int> ReportGenerator::getLowStockItems() {
    map<string, int> lowStockItems;
    ifstream productsFile("data/products.json");
    if (!productsFile.is_open()) {
        cerr << "Failed to open products.json!" << endl;
        return lowStockItems;
    }
    json productsData;
    productsFile >> productsData;
    productsFile.close();
    for (const auto &product: productsData["products"]) {
        int quantity = product["quantity"].get<int>();
        if (quantity < 5) {
            lowStockItems[product["name"].get<string>()] = quantity;
        }
    }
    return lowStockItems;
}

map<string, string> ReportGenerator::getSoonExpireItems() {
    map<string, string> soonExpireItems;
    ifstream productsFile("data/products.json");
    if (!productsFile.is_open()) {
        cerr << "Failed to open products.json!" << endl;
        return soonExpireItems;
    }
    json productsData;
    productsFile >> productsData;
    productsFile.close();
    string currentDate = getCurrentDate();
    tm tm1{}, tm2{};
    istringstream currentDateStream(currentDate);
    if (!(currentDateStream >> get_time(&tm1, "%Y-%m-%d"))) {
        cerr << "Failed to parse current date!" <<endl;
        return soonExpireItems;
    }
    for (const auto &product : productsData["products"]) {
        if (product.find("expiration_date") != product.end()) {
            string expirationDate = product["expiration_date"].get<string>();
            istringstream expirationDateStream(expirationDate);
            time_t currentTime = mktime(&tm1);
            time_t expireTime = mktime(&tm2);
            double secondsInAMonth = 30 * 24 * 60 * 60;
            if (difftime(expireTime, currentTime) <= secondsInAMonth) {
                soonExpireItems[product["name"].get<std::string>()] = expirationDate;
            }
        }
    }
    return soonExpireItems;
}

int ReportGenerator::getTotalTransactions(const json &transactions) {
    return transactions.size();
}

double ReportGenerator::getAverageBasketSize(const json &transactions) {
    int totalItems = 0;
    for (const auto &transaction: transactions) {
        totalItems += transaction["items"].size();
    }
    return static_cast<double>(totalItems) / transactions.size();
}

int ReportGenerator::getDiscountUsage(const json &transactions) {
    int count = 0;
    for (const auto &transaction: transactions) {
        if (transaction["discount"].get<double>() > 0) {
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
    ifstream productsFile("data/products.json");
    if (!productsFile.is_open()) {
        cerr << "Failed to open products.json!" << endl;
        return;
    }

    json productsData;
    productsFile >> productsData;
    productsFile.close();

    string currentDate = getCurrentDate();
    json updatedProducts = json::array();

    for (const auto &product : productsData["products"]) {
        if (product.find("expiration_date") != product.end()) {
            string expirationDate = product["expiration_date"].get<string>();
            struct tm tm1, tm2;
            strptime(currentDate.c_str(), "%Y-%m-%d", &tm1);
            strptime(expirationDate.c_str(), "%Y-%m-%d", &tm2);
            time_t currentTime = mktime(&tm1);
            time_t expireTime = mktime(&tm2);

            if (difftime(currentTime, expireTime) < 0) {
                updatedProducts.push_back(product);
            }
        } else {
            updatedProducts.push_back(product);
        }
    }

    ofstream outFile("data/products.json");
    if (!outFile.is_open()) {
        cerr << "Failed to open products.json for writing!" << endl;
        return;
    }
    outFile << updatedProducts.dump(4);
    outFile.close();

    cout << "Expired items deleted successfully.\n";
}

void ReportGenerator::generateReport(int option, const std::string &startDate, const std::string &endDate) {
    ifstream transactionsFile("data/transactions.json");
    if (!transactionsFile.is_open()) {
        cerr << "Failed to open transactions.json!" << endl;
        return;
    }
    json transactions;
    transactionsFile >> transactions;
    transactionsFile.close();
    string reportType = getReportType(option, startDate, endDate);
    double total = calculateTotal(transactions);
    auto topSelling = getTopSellingItems(transactions);
    auto lowStock = getLowStockItems();
    auto soonExpire = getSoonExpireItems();
    int totalTransactions = getTotalTransactions(transactions);
    double averageBasket = getAverageBasketSize(transactions);
    int discountUsage = getDiscountUsage(transactions);
    ofstream reportFile("reports/" + reportType + "_report.txt");
    reportFile << "Total Sales: $" << total << "\n";
    reportFile << "Top Selling Items:\n";
    for (const auto &item: topSelling) {
        reportFile << item.first << ": " << item.second << "\n";
    }
    reportFile << "Low Stock Items:\n";
    for (const auto &item: lowStock) {
        reportFile << item.first << ": " << item.second << "\n";
    }
    reportFile << "Items Soon to Expire:\n";
    for (const auto &item: soonExpire) {
        reportFile << item.first << ": " << item.second << "\n";
    }
    reportFile << "Total Transactions: " << totalTransactions << "\n";
    reportFile << "Average Basket Size: " << averageBasket << "\n";
    reportFile << "Discount Usage: " << discountUsage << "\n";
    reportFile.close();
    cout << "Report generated successfully.\n";
}

#endif // REPORTGENERATOR_H
