#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string encrypt(const std::string& msg, int key) {
    std::string encrypted_msg = msg;
    for (char &c : encrypted_msg) {
        c += key;
    }
    return encrypted_msg;
}

#endif // ENCRYPT_H
