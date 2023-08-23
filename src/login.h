#include <map>

class LoginSystem {
private:
    std::map<std::string, std::string> users;  // map of usernames and their passwords

public:
    LoginSystem() {
        // For simplicity, we're hardcoding two users. In a real system, retrieve these from a secure source.
        users["admin"] = "password123";
        users["user"] = "password456";
    }

    bool authenticate(const std::string& username, const std::string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second == password) {
            return true;
        }
        return false;
    }
};

