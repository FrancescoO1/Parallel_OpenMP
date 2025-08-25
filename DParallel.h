#ifndef DPARALLEL_H
#define DPARALLEL_H

#pragma once
#include <string>

// Funzione globale per il brute-force parallelo con modalità aos/soa
std::string decryptDESParallel(
    const std::string& encrypted,
    const std::string& salt,
    const std::string& dict_path,
    int n_threads,
    double& total_time,
    double& throughput,
    const std::string& mode);

#endif //DPARALLEL_H