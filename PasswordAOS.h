#ifndef PASSWORDAOS_H
#define PASSWORDAOS_H

#pragma once
#include <vector>
#include <string>

struct PasswordStruct {
    char value[9]; // 8 caratteri + '\0'
};

class PasswordAOS {
private:
    std::vector<PasswordStruct> data;

public:
    void loadFromFile(const std::string& path);
    char* get(int i);
    int size() const;
};


#endif //PASSWORDAOS_H
