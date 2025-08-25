
#include "DParallel.h"
#include "PasswordAOS.h"
#include "PasswordSOA.h"
#include <iostream>
#include <omp.h>
#include <atomic>
#include <crypt.h>

std::string decryptDESParallel(const std::string& encrypted, const std::string& salt,
                              const std::string& dict_path, int n_threads,
                              double& total_time, double& throughput,
                              const std::string& mode) {

    PasswordAOS aos;
    PasswordSOA soa;
    int count = 0;

    if (mode == "aos") {
        aos.loadFromFile(dict_path);
        count = aos.size();
    } else if (mode == "soa") {
        soa.loadFromFile(dict_path);
        count = soa.size();
    } else {
        total_time = 0.0;
        throughput = 0.0;
        return "";
    }

    std::string found_password;
    std::atomic<bool> found(false);

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(n_threads) shared(found, found_password, encrypted, salt, aos, soa, count)
    {
        struct crypt_data data;
        data.initialized = 0;
        bool local_found = false;

        if (mode == "aos") {
            #pragma omp for schedule(dynamic, 1000) nowait
            for (int i = 0; i < count; ++i) {  // ✅ Loop semplice senza condizioni extra
                // Check periodico ogni 1024 iterazioni
                if ((i & 0x3FF) == 0) {
                    local_found = found.load(std::memory_order_relaxed);
                }

                if (local_found) continue;  // ✅ Continue è permesso

                char* pw = aos.get(i);
                const char* result = crypt_r(pw, salt.c_str(), &data);

                if (result && encrypted == result) {
                    #pragma omp critical
                    {
                        if (!found.load()) {
                            found_password = std::string(pw);
                            found.store(true, std::memory_order_release);
                            local_found = true;
                        }
                    }
                }
            }
        } else if (mode == "soa") {
            #pragma omp for schedule(dynamic, 1000) nowait
            for (int i = 0; i < count; ++i) {  // ✅ Loop semplice
                if ((i & 0x3FF) == 0) {
                    local_found = found.load(std::memory_order_relaxed);
                }

                if (local_found) continue;  // ✅ Continue è permesso

                char* pw = soa.get(i);
                const char* result = crypt_r(pw, salt.c_str(), &data);

                if (result && encrypted == result) {
                    #pragma omp critical
                    {
                        if (!found.load()) {
                            found_password = std::string(pw);
                            found.store(true, std::memory_order_release);
                            local_found = true;
                        }
                    }
                }
            }
        }
    }

    double end = omp_get_wtime();
    total_time = end - start;
    throughput = count / total_time;

    return found_password;
}