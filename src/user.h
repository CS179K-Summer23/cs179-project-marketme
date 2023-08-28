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
    string _ename;
    string _email;

    User(const string & name, const string & email) {
      _ename = name;
      _email = email;
    }

    string getName() const {
      return _ename;
    }

    string getEmail() const {
      return _email;
    }
  
    json toJson() const {
      return {
        {
          "name",
          _ename
        },
        {
          "email",
          _email
        }
    };
  }
};

#endif