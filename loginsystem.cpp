#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <bcrypt/BCrypt.hpp>

using namespace std;
using json = nlohmann::json;

bool registerUser(const string& user, const string& pass, json& db) {
    if (db.find(user) != db.end()) return false;
    string hashPass = BCrypt::generateHash(pass);
    db[user]["hashedPassword"] = hashPass;
    return true;
}

bool loginUser(const string& user, const string& pass, json& db) {
    if (db.find(user) == db.end()) return false;
    string storedPass = db[user]["hashedPassword"];
    return BCrypt::validatePassword(pass, storedPass);
}

int main() {
    json db;
    ifstream dbFile("db.json");
    if (dbFile.is_open()) {
        dbFile >> db;
        dbFile.close();
    }

    while (true) {
        cout << "1. Register\n2. Login\n3. View Users\n4. Exit\nYour choice: ";
        int choice;
        cin >> choice;
        string user, pass;

        switch (choice) {
            case 1:
                cout << "Username: ";
                cin >> user;
                cout << "Password: ";
                cin >> pass;
                registerUser(user, pass, db) ? 
                    cout << "Welcome, " << user << "!\n" : 
                    cout << "Oops, try another username.\n";
                break;
            
            case 2:
                cout << "Username: ";
                cin >> user;
                cout << "Password: ";
                cin >> pass;
                loginUser(user, pass, db) ?
                    cout << "Logged in.\n" :
                    cout << "Failed to login.\n";
                break;

            case 3:
                for (auto& [name, _] : db.items()) cout << name << endl;
                break;

            case 4:
                ofstream dbFileOut("db.json");
                dbFileOut << db.dump(4);
                dbFileOut.close();
                return 0;

            default:
                cout << "Hmm. Try again." << endl;
                break;
        }
    }
}
