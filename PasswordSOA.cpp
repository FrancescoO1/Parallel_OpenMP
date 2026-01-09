#include "PasswordSOA.h"
#include <fstream>
void PasswordSOA::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        if (line.length() == 8) {
            for (int i = 0; i < 8; ++i) {
                chars[i].push_back(line[i]);
            }
        }
    }
}

char* PasswordSOA::get(int i) {
    static thread_local char buffer[9];
    for (int j = 0; j < 8; ++j) {
        buffer[j] = chars[j][i];
    }
    buffer[8] = '\0';
    return buffer;
}

int PasswordSOA::size() const {
    return chars[0].size(); 
}