#ifndef EMAIL_H
#define EMAIL_H

// Include Windows headers before curl.h
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <curl/curl.h>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include "base64.h"
#include "user.h"
#include "report.h"
#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

const string COMMAND_BASE = "curl -X POST -H \"Authorization: Bearer ";
const string API_ENDPOINT = "https://gmail.googleapis.com/gmail/v1/users/me/messages/send";

void saveSubscribers(const vector<User>& subscribers) {
    json j;
    for (const auto& user : subscribers) {
        j.push_back({{"name", user.getName()}, {"email", user.getEmail()}});
    }
    ofstream file("data/subscribers.json");
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    } else {
        cerr << "Could not open data/subscribers.json for writing." << endl;
    }
}

vector<User> loadSubscribers() {
    vector<User> subscribers;
    ifstream file("data/subscribers.json");
    if (file.is_open()) {
        json j;
        file >> j;
        for (const auto& elem : j) {
            subscribers.push_back(User(elem["name"], elem["email"]));
        }
        file.close();
    } else {
        cerr << "Could not open data/subscribers.json for reading." << endl;
    }
    return subscribers;
}

size_t EmailCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Do nothing with the response data
    return size * nmemb;
}

bool isValidEmail(const string& email) {
    // Simple email format validation using regex
    // You can modify this pattern to match your requirements
    const regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4})");
    return regex_match(email, emailPattern);
}

void sendEmailWithLibcurl(const string& accessToken, const string& recipient, const string& payload, int msg) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize libcurl." << endl;
        return;
    }

    string url = API_ENDPOINT;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, EmailCallback);
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        if (msg == 1) {
            cout << "Thanks for subscribing to MarketMe! Please check your email for confirmation." << endl;
        }
        else if (msg == 2) {
            cout << "You have successfully unsubscribed from MarketMe newsletters." << endl;   
        }
        else if (msg == 3) {
            cout << "Receipt has been sent to your email." << endl;   
        }
    } else{
        cerr << "Failed to send email to " << recipient << ". Error: " << curl_easy_strerror(res) << endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void sendEmailWithLibcurlCount(const string& accessToken, const string& recipient, const string& payload, int& count) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize libcurl." << endl;
        return;
    }

    string url = API_ENDPOINT;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, EmailCallback);
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        count++;  // Increment count only for successful emails
    } else {
        cerr << "Failed to send email to " << recipient << ". Error: " << curl_easy_strerror(res) << endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void reportEmail(const string& accessToken, ReportGenerator report) {
    string filename = "report/dailyreport_" + report.getCurrentDate() + ".txt";
    ifstream reportFile(filename);

    if (!reportFile.is_open()) {
        cerr << "Failed to open " << filename << endl;
        return;
    }

    string reportContent((istreambuf_iterator<char>(reportFile)), istreambuf_iterator<char>());
    reportFile.close();

    string content = "To: official.marketme@gmail.com\r\n"
                     "Subject: Daily Report!\r\n"
                     "Content-Type: text/html\r\n"
                     "\r\n"
                     "<html><body>"
                     "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                     "<pre>" + reportContent + "</pre>"
                     "</body></html>";

    CURL *curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize libcurl." << endl;
        return;
    }

    string url = API_ENDPOINT;
    string payload = "{ \"raw\": \"" + base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length()) + "\" }";

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + accessToken).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

  // Set CURLOPT_WRITEFUNCTION to prevent writing the response to stdout
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, EmailCallback);
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        cout << "The daily report has been sent. Please check the company email." << endl;
    } else {
        cerr << "Failed to send the report. Error: " << curl_easy_strerror(res) << endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void subscribe(const string& accessToken){
    vector<User> subscribers = loadSubscribers();
    string name = "";
    string email = "";
    cout << "Please input your name: ";
    cin.ignore();
    getline(cin, name);
    
    // Validate email format
    do {
        cout << "Please input your email: ";
        getline(cin, email);
        if (!isValidEmail(email)) {
            cout << "Invalid email format. Please try again.\n";
        }
    } while (!isValidEmail(email));

    for (const auto& user : subscribers)
    {
        if (user.getEmail() == email)
        {
            cout << "You're already subscribed!" << endl;
            return;
        }
    }

    subscribers.push_back(User(name, email));
    saveSubscribers(subscribers);
    
    string content = "To: " + email + "\r\n"
                     "Subject: Welcome, " + name + "!\r\n"
                     "Content-Type: text/html\r\n" // Specify HTML content type
                         "\r\n"
                         "<html><body>"
                         "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                         "<p>You have successfully subscribed to MarketMe newsletters!\r\n</p>"
                         "</body></html>";
    string payload = "{ \"raw\": \"" + base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length()) + "\" }";

    sendEmailWithLibcurl(accessToken, email, payload, 1);
}

void unsubscribe(const string& accessToken){
    vector<User> subscribers = loadSubscribers();
    string email = "";
    cin.ignore();
    do {
        cout << "Please input your email: ";
        getline(cin, email);
        if (!isValidEmail(email)) {
            cout << "Invalid email format. Please try again.\n";
        }
    } while (!isValidEmail(email));

    auto userIter = std::find_if(subscribers.begin(), subscribers.end(), [&email](const User& user) {
        return user.getEmail() == email;
    });

    if (userIter == subscribers.end()) {
        cerr << "Email not found in subscribers list." << endl;
        return;
    }

    string content = "To: " + email + "\r\n"
                     "Subject: We'll miss you, " + userIter->getName() + "!\r\n"
                     "Content-Type: text/html\r\n"
                     "\r\n"
                     "<html><body>"
                     "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                     "<p>You have successfully unsubscribed from MarketMe newsletters.\r\n</p>"
                     "</body></html>";
    string payload = "{ \"raw\": \"" + base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length()) + "\" }";

    sendEmailWithLibcurl(accessToken, email, payload, 2);
    subscribers.erase(userIter);
    saveSubscribers(subscribers);
}

void newsletter(const string& accessToken, ReportGenerator& reportGen){
    int count = 0;
    map<string, string> soonExpireItems = reportGen.getSoonExpireItems();
    vector<User> subscribers = loadSubscribers();
    for (const auto& user : subscribers){
        string content = "To: " + user.getEmail() + "\r\n"
                        "Subject: Hi " + user.getName() + "! These items are expiring soon!\r\n"
                        "Content-Type: text/html\r\n" // Specify HTML content type
                        "\r\n"
                        "<html><body>"
                        "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                        "<p>Here are the items that are expiring soon:</p>"
                        "<ul>";

        for (const auto& item : soonExpireItems) {
            content += "<li>" + item.first + " - Expiration Date: " + item.second + "</li>";
        }

        content += "</ul>"
                "<p>Use coupon code SPECIAL70 to get 70% off on all soon-to-be expired items!</p>"
                "</body></html>";

        string payload = "{ \"raw\": \"" + base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length()) + "\" }";

        sendEmailWithLibcurlCount(accessToken, user.getEmail(), payload, count);
    }
    
    if (subscribers.empty()){
        cout << "Oops! There are no subscribers in the system yet." << endl;
    }
    else if (count == 0) {
        cout << "Error: Sending emails was unsuccessful." << endl;
    }
    else {    
        cout << "Successfully sent " + to_string(count) + " email(s)!" << endl;
    }
}


string formatDecimal(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

void receipt(const string& accessToken, const string& email, const json& transaction) {
    
    string content = "To: " + email + "\r\n"
                    "Subject: Thanks for shopping with us!\r\n"
                    "Content-Type: text/html\r\n" // Specify HTML content type
                    "\r\n"
                    "<html><body>"
                    "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                    "<p>Here is your receipt:</p>"
                    "<p>Date: " + transaction["date"].get<string>() + "</p>"
                    "<p>Total: $" + formatDecimal(transaction["total"].get<double>()) + "</p>"
                    "<p>Tax: $" + formatDecimal(transaction["tax"].get<double>()) + "</p>"
                    "<p>Discount: $" + formatDecimal(transaction["discount"].get<double>()) + "</p>"
                    "<p>Operator: " + transaction["operator"].get<string>() + "</p>"
                    "<p>Items:</p>"
                    "<ul>";

for (const auto& item : transaction["items"]) {
    content += "<li>" + item["name"].get<string>() + " - Quantity: " + to_string(item["quantity"].get<int>())
                + ", Price per Item: $" + formatDecimal(item["price_per_item"].get<double>()) 
                + ", Total Price: $" + formatDecimal(item["total_price"].get<double>()) + "</li>";
}

    content += "</ul></body></html>";

    string payload = "{ \"raw\": \"" + base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length()) + "\" }";

    sendEmailWithLibcurl(accessToken, email, payload, 3);
}

#endif
