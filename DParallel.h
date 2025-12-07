#ifndef DPARALLEL_H
#define DPARALLEL_H

#pragma once
#include <string>

std::string decryptDESParallel(
    const std::string& encrypted,
    const std::string& salt,
    const std::string& dict_path,
    int n_threads,
    double& total_time,
    double& throughput,
    const std::string& mode);

#endif