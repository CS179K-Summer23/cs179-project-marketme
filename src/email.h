#ifndef EMAIL_H
#define EMAIL_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "base64.h"
#include "user.h"
#include "report.h"

using namespace std;
// using CHAT GPT template to send email

const string COMMAND_BASE = "curl -X POST -H \"Authorization: Bearer ";
const string API_ENDPOINT = "https://gmail.googleapis.com/gmail/v1/users/me/messages/send";


// Template function to send the email using the Gmail API
void sendEmail(const string& accessToken, const string& rawEmailContentBase64) {
    string email_command = COMMAND_BASE + accessToken +
                     "\" -H \"Content-Type: application/json\" -d '"
                     "{ \"raw\": \"" + rawEmailContentBase64 + "\" }' " + API_ENDPOINT;

    string command = email_command + " >/dev/null 2>/dev/null";

    int result = system(command.c_str());

    if (result == 0) {
        cout << "Email sent successfully." << endl;
    } else {
        cerr << "Failed to send email." << endl;
    }
}

void reportEmail(const string&accessToken, ReportGenerator report) {
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
                     "Content-Type: text/html\r\n" // Specify HTML content type
                         "\r\n"
                         "<html><body>"
                         "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                         "<pre>" + reportContent + "</pre>"
                         "</body></html>";
    
    string base64 = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    string command = COMMAND_BASE + accessToken +
                     "\" -H \"Content-Type: application/json\" -d '"
                     "{ \"raw\": \"" + base64 + "\" }' " + API_ENDPOINT + 
                     " >/dev/null 2>/dev/null";

    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Failed to send the report." << endl;
    }
    else {
        cout << "The daily report has been sent. Please check the company email." << endl;
    }
    
}

void subscribe(const string& accessToken, vector<User>& subscribers){
    string name = "";
    string email = "";
    cout << "Please input your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Please input your email: ";
    getline(cin, email);

    for (const auto& user : subscribers)
    {
        if (user._ename == email)
        {
            cout << "You're already subscribed!" << endl;
            return;
        }
    }

    subscribers.push_back(User(name, email));
    
    string content = "To: " + email + "\r\n"
                     "Subject: Welcome, " + name + "!\r\n"
                     "Content-Type: text/html\r\n" // Specify HTML content type
                         "\r\n"
                         "<html><body>"
                         "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                         "<p>You have successfully subscribed to MarketMe newsletters!\r\n</p>"
                         "</body></html>";
    string base64 = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    string command = COMMAND_BASE + accessToken +
                     "\" -H \"Content-Type: application/json\" -d '"
                     "{ \"raw\": \"" + base64 + "\" }' " + API_ENDPOINT + 
                     " >/dev/null 2>/dev/null";

    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Failed to send email to " + email << "." << endl;
    }
    else {
        cout << "Thanks for subscribing to MarketMe! Please check your email for confirmation." << endl;
    }
    
}

void unsubscribe(const string& accessToken, vector<User>& subscribers){
    string email, content;
    cout << "Please input your email: ";
    cin.ignore();
    getline(cin, email);
    
    for (auto userIter = subscribers.begin(); userIter != subscribers.end(); ++userIter) {
        if (userIter->_email == email) {
            content = "To: " + email + "\r\n"
                      "Subject: We'll miss you, " + userIter->_ename + "!\r\n"
                      "Content-Type: text/html\r\n" // Specify HTML content type
                      "\r\n"
                      "<html><body>"
                      "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/marketme.png\" alt=\"MarketMe Logo\"></p>"
                      "<p>You have successfully unsubscribed from MarketMe newsletters.\r\n</p>"
                      "</body></html>";
            subscribers.erase(userIter); // Remove the user from the vector.
            break;
        }
    }

    string base64 = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());
    string command = COMMAND_BASE + accessToken +
                     "\" -H \"Content-Type: application/json\" -d '"
                     "{ \"raw\": \"" + base64 + "\" }' " + API_ENDPOINT + 
                     " >/dev/null 2>/dev/null";

    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Failed to send email to " + email << "." << endl;
    }
    else {
        cout << "Thanks for subscribing to MarketMe! Please check your email for confirmation." << endl;
    }
}

void newsletter(const string& accessToken, const vector<User>& subscribers){
    
    int count = 0;
    string command = "";

    for (const auto& user : subscribers){
        string content = "To: " + user._email + "\r\n"
                     "Subject: Hi " + user._ename + "! Check out this deal!\r\n"
                     "Content-Type: text/html\r\n" // Specify HTML content type
                     "\r\n"
                     "<html><body>"
                     "<p><img src=\"https://raw.githubusercontent.com/CS179K-Summer23/cs179-project-marketme/email/data/newsletter.png\" alt=\"Newsletter\">\r\n</p>"
                     "</body></html>";

        string base64 = base64_encode(reinterpret_cast<const unsigned char*>(content.c_str()), content.length());

        command = COMMAND_BASE + accessToken +
                     "\" -H \"Content-Type: application/json\" -d '"
                     "{ \"raw\": \"" + base64 + "\" }' " + API_ENDPOINT + 
                     " >/dev/null 2>/dev/null";

        int result = system(command.c_str());

        if (result != 0) {
            cerr << "Failed to send email to " + user._ename << "." << endl;
        }
        else {
            count++;
        }
    }
    
    cout << "Successfully sent " + to_string(count) + " email(s)!" << endl;

}

#endif
