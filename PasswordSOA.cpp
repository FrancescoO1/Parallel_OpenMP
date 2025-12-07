



//CON FILTRAGGIO PW LUNGHE 8 ho 800k o 2.5M pw da testare
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
    return chars[0].size(); // tutte le colonne hanno stessa dimensione
}



//SENZA FILTRAGGIO PW, ho 14M o 32M di pw da testare
/* ora se ho pw di wuesto tipop faccio:
rockyou.txt contiene:
"123"      → Caricata come "123\0\0\0\0\0"
"rayna123" → Caricata come "rayna123"
"verylongpassword" → Caricata come "verylon"
"password123456"   → Caricata come "password" (troncata)
*/

/*

#include "PasswordSOA.h"
#include <fstream>
#include <algorithm>

void PasswordSOA::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        // RIMOSSO IL FILTRO: carica tutte le password indipendentemente dalla lunghezza
        if (!line.empty()) {  // Solo controllo che non sia vuota
            size_t len = std::min(line.length(), static_cast<size_t>(8));

            // Aggiungi caratteri fino alla lunghezza effettiva
            for (size_t i = 0; i < len; ++i) {
                chars[i].push_back(line[i]);
            }

            // Riempi con '\0' per le posizioni rimanenti se la password è più corta di 8
            for (size_t i = len; i < 8; ++i) {
                chars[i].push_back('\0');
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
    return chars[0].size(); // tutte le colonne hanno stessa dimensione
}
*/