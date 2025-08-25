#ifndef D_SEQUENTIAL_H
#define D_SEQUENTIAL_H

#pragma once
#include <string>

// Funzione globale per la versione sequenziale con modalità aos/soa
std::string decryptDESSequential(
    const std::string& encrypted,
    const std::string& salt,
    const std::string& dict_path,
    double& total_time,
    double& throughput,
    const std::string& mode);

#endif // D_SEQUENTIAL_H