#include "DSequential.h"
#include "PasswordAOS.h"
#include "PasswordSOA.h"
#include <iostream>
#include <crypt.h>
#include <chrono>

std::string decryptDESSequential(const std::string& encrypted, const std::string& salt, const std::string& dict_path, double& total_time, double& throughput, const std::string& mode) {
    struct crypt_data data;
    data.initialized = 0;

    PasswordAOS aos;
    PasswordSOA soa;

    auto start = std::chrono::high_resolution_clock::now();


    if (mode == "aos") {
        aos.loadFromFile(dict_path);
    } else if (mode == "soa") {
        soa.loadFromFile(dict_path);
    } else {
        total_time = 0.0;
        throughput = 0.0;
        return "";
    }


    int count = 0;
    if (mode == "aos") {
        count = aos.size();
        for (int i = 0; i < count; ++i) {
            char* pw = aos.get(i);
            char* result = crypt_r(pw, salt.c_str(), &data);
            if (result && encrypted == result) {
                auto end = std::chrono::high_resolution_clock::now();
                total_time = std::chrono::duration<double>(end - start).count();
                throughput = count / total_time;
                return std::string(pw);
            }
        }
    } else if (mode == "soa") {
        count = soa.size();
        for (int i = 0; i < count; ++i) {
            char* pw = soa.get(i);
            char* result = crypt_r(pw, salt.c_str(), &data);
            if (result && encrypted == result) {
                auto end = std::chrono::high_resolution_clock::now();
                total_time = std::chrono::duration<double>(end - start).count();
                throughput = count / total_time;
                return std::string(pw);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    total_time = std::chrono::duration<double>(end - start).count();
    throughput = count / total_time;
    return "";
}