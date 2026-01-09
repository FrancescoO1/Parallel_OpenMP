#include "PasswordAOS.h"
#include <fstream>
#include <cstring>
void PasswordAOS::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        if (line.length() == 8) {
            PasswordStruct p;
            strncpy(p.value, line.c_str(), 8);
            p.value[8] = '\0';
            data.push_back(p);
        }
    }
}

char* PasswordAOS::get(int i) {
    return data[i].value;
}

int PasswordAOS::size() const {
    return data.size();
}
