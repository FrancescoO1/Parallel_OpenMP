#ifndef PASSWORDSOA_H
#define PASSWORDSOA_H

#pragma once
#include <vector>
#include <string>

class PasswordSOA {
private:
    std::vector<char> chars[8];

public:
    void loadFromFile(const std::string& path);
    char* get(int i);
    int size() const;
};


#endif
