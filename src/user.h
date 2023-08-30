#ifndef USER_H
#define USER_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include "../libraries/json.hpp"

using namespace std;
using json = nlohmann::json;

class User {
public: 
    enum Role {
        Manager,
        Cashier,
        Undefined //for backup
    };

private:
    string _ename;
    string _email;
    Role _role;

    // Static method to validate registration key for Manager role
    static bool validateManagerKey(const string &key) {
        const string correctKey = "TEST1";  
        return key == correctKey;
    }

public: 
    User(const string & name, const string & email, Role role = Undefined, const string & regKey = "") 
        : _ename(name), _email(email), _role(role) 
    {
        // If user wants to be a manager, validate the key
        if (_role == Manager && !validateManagerKey(regKey)) {
            _role = Undefined;
            cerr << "Invalid registration key provided. Role set to Undefined." << endl;
        }
    }

    string getName() const {
        return _ename;
    }

    string getEmail() const {
        return _email;
    }

    Role getRole() const {
        return _role;
    }

    string roleToString() const {
        switch(_role) {
            case Manager: return "Manager";
            case Cashier: return "Cashier";
            default: return "Undefined";
        }
    }

    json toJson() const {
        return {
            {"name", _ename},
            {"email", _email},
            {"role", roleToString()}
        };
    }
};

#endif
