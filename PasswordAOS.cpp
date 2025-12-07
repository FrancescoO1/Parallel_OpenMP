


//CON FILTRAGGIO PW LUNGHE 8
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





/*
//SENZA FILTRAGGIO PW
#include "PasswordAOS.h"
#include <fstream>
#include <cstring>

void PasswordAOS::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        // RIMOSSO IL FILTRO: carica tutte le password indipendentemente dalla lunghezza
        if (!line.empty()) {  // Solo controllo che non sia vuota
            PasswordStruct p;
            // Limita la copia a massimo 8 caratteri per evitare buffer overflow
            size_t len = std::min(line.length(), static_cast<size_t>(8));
            strncpy(p.value, line.c_str(), len);
            p.value[len] = '\0';  // Null-terminate sempre
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
*/