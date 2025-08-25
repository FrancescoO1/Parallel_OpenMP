#include "DSequential.h"
#include "DParallel.h"
#include "Media.h"
#include <iostream>
#include <fstream>
#include <crypt.h>
#include <vector>
#include <iomanip>

int main() {
    const int NUM_RUNS = 10;

    std::string password = "rayna123"; //scelgo la pw da 8 caratteri da hashare e rompere
    std::string salt = "of";
    std::string dict_path = "rockyou.txt";
    int max_threads = 64;

    std::string hash = std::string(crypt(password.c_str(), salt.c_str()));

    // Header iniziale
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  BENCHMARK AOS vs SOA - PASSWORD DECRYPTION" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "🔍 Password: " << password << " | Salt: " << salt << std::endl;
    std::cout << "🔗 Hash DES: " << hash << std::endl;
    std::cout << "🔁 Run per test: " << NUM_RUNS << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Vettori per raccogliere i tempi
    std::vector<double> times_seq_aos(NUM_RUNS);
    std::vector<double> times_seq_soa(NUM_RUNS);

    // Test sequenziali (esecuzione silenziosa)
    std::cout << "\n📊 Eseguendo test sequenziali e paralleli..." << std::flush;

    for (int run = 0; run < NUM_RUNS; ++run) {
        double t_seq_aos, thr_seq_aos;
        decryptDESSequential(hash, salt, dict_path, t_seq_aos, thr_seq_aos, "aos");
        times_seq_aos[run] = t_seq_aos;

        double t_seq_soa, thr_seq_soa;
        decryptDESSequential(hash, salt, dict_path, t_seq_soa, thr_seq_soa, "soa");
        times_seq_soa[run] = t_seq_soa;

        std::cout << "." << std::flush;
    }
    std::cout << " Completato!" << std::endl;

    // Calcolo statistiche sequenziali
    auto [mean_seq_aos, stdev_seq_aos] = calculateStats(times_seq_aos);
    auto [mean_seq_soa, stdev_seq_soa] = calculateStats(times_seq_soa);

    // Calcolo throughput medio
    double temp_time, temp_thr;
    decryptDESSequential(hash, salt, dict_path, temp_time, temp_thr, "aos");
    double throughput_seq_aos = temp_thr * temp_time / mean_seq_aos;

    decryptDESSequential(hash, salt, dict_path, temp_time, temp_thr, "soa");
    double throughput_seq_soa = temp_thr * temp_time / mean_seq_soa;

    std::cout << "\n📈 RISULTATI SEQUENZIALI:" << std::endl;
    std::cout << "┌─────────┬─────────────────┬──────────────────┐" << std::endl;
    std::cout << "│  TIPO   │      TEMPO      │    THROUGHPUT    │" << std::endl;
    std::cout << "├─────────┼─────────────────┼──────────────────┤" << std::endl;
    std::cout << "│   AOS   │ " << std::fixed << std::setw(6) << std::setprecision(3) << mean_seq_aos
              << " ± " << std::setw(5) << stdev_seq_aos << "s  │ " << std::setw(8) << std::setprecision(0)
              << throughput_seq_aos << " pwd/s│" << std::endl;
    std::cout << "│   SOA   │ " << std::fixed << std::setw(6) << std::setprecision(3) << mean_seq_soa
              << " ± " << std::setw(5) << stdev_seq_soa << "s  │ " << std::setw(8) << std::setprecision(0)
              << throughput_seq_soa << " pwd/s│" << std::endl;
    std::cout << "└─────────┴─────────────────┴──────────────────┘" << std::endl;

    // Apertura file CSV (rimuovo efficiency_csv e scalability_csv)
    std::ofstream csv("risultati_scaling.csv");
    std::ofstream summary("riepilogo_medie.csv");
    std::ofstream bar_chart_csv("bar_chart_tempi.csv");
    std::ofstream line_chart_csv("line_chart_speedup.csv");

    std::ofstream throughput_csv("throughput_chart.csv");
    std::ofstream variability_csv("variability_chart.csv");
    std::ofstream overhead_csv("overhead_chart.csv");

    // Headers per tutti i CSV
    csv << "Thread,Tipo,Run,Tempo,Throughput,Speedup\n";
    summary << "Thread,Tempo_AOS_Media,Tempo_AOS_StdDev,Throughput_AOS,Speedup_AOS,"
            << "Tempo_SOA_Media,Tempo_SOA_StdDev,Throughput_SOA,Speedup_SOA\n";
    bar_chart_csv << "Thread,AOS_Tempo,SOA_Tempo,AOS_StdDev,SOA_StdDev\n";
    line_chart_csv << "Thread,AOS_Speedup,SOA_Speedup,Speedup_Teorico\n";

    throughput_csv << "Thread,AOS_Throughput,SOA_Throughput,AOS_StdDev,SOA_StdDev\n";
    variability_csv << "Thread,AOS_CV,SOA_CV\n";
    overhead_csv << "Thread,AOS_Overhead,SOA_Overhead\n";

    // Salva risultati sequenziali in tutti i CSV
    for (int run = 0; run < NUM_RUNS; ++run) {
        double thr_aos = throughput_seq_aos * mean_seq_aos / times_seq_aos[run];
        double thr_soa = throughput_seq_soa * mean_seq_soa / times_seq_soa[run];
        csv << "1,AOS," << (run + 1) << "," << times_seq_aos[run] << "," << thr_aos << ",1.0\n";
        csv << "1,SOA," << (run + 1) << "," << times_seq_soa[run] << "," << thr_soa << ",1.0\n";
    }

    // Dati sequenziali per riepilogo e grafici
    summary << "1," << mean_seq_aos << "," << stdev_seq_aos << "," << throughput_seq_aos << ",1.0,"
            << mean_seq_soa << "," << stdev_seq_soa << "," << throughput_seq_soa << ",1.0\n";
    bar_chart_csv << "1," << mean_seq_aos << "," << mean_seq_soa << ","
                  << stdev_seq_aos << "," << stdev_seq_soa << "\n";
    line_chart_csv << "1,1.0,1.0,1.0\n";

    // Test paralleli
    std::cout << "\n📊 RISULTATI PARALLELI:" << std::endl;
    std::cout << "┌─────────┬─────────┬─────────────────┬──────────────────┬─────────────┐" << std::endl;
    std::cout << "│ THREAD  │  TIPO   │      TEMPO      │    THROUGHPUT    │   SPEEDUP   │" << std::endl;
    std::cout << "├─────────┼─────────┼─────────────────┼──────────────────┼─────────────┤" << std::endl;

    for (int threads = 2; threads <= max_threads; threads *= 2) {
        std::vector<double> times_par_aos(NUM_RUNS);
        std::vector<double> times_par_soa(NUM_RUNS);
        std::vector<double> throughputs_par_aos(NUM_RUNS);
        std::vector<double> throughputs_par_soa(NUM_RUNS);

        std::cout << "│    " << std::setw(2) << threads << "   │ Eseguendo test..." << std::flush;

        // Run paralleli (esecuzione silenziosa)
        for (int run = 0; run < NUM_RUNS; ++run) {
            double t_aos, thr_aos;
            decryptDESParallel(hash, salt, dict_path, threads, t_aos, thr_aos, "aos");
            times_par_aos[run] = t_aos;
            throughputs_par_aos[run] = thr_aos;

            double t_soa, thr_soa;
            decryptDESParallel(hash, salt, dict_path, threads, t_soa, thr_soa, "soa");
            times_par_soa[run] = t_soa;
            throughputs_par_soa[run] = thr_soa;
        }

        // Cancella la riga di "Eseguendo test..."
        std::cout << "\r";

        // Calcolo statistiche parallele
        auto [mean_par_aos, stdev_par_aos] = calculateStats(times_par_aos);
        auto [mean_par_soa, stdev_par_soa] = calculateStats(times_par_soa);
        auto [mean_thr_aos, stdev_thr_aos] = calculateStats(throughputs_par_aos);
        auto [mean_thr_soa, stdev_thr_soa] = calculateStats(throughputs_par_soa);

        // Calcolo speedup
        double speedup_aos = mean_seq_aos / mean_par_aos;
        double speedup_soa = mean_seq_soa / mean_par_soa;
        double speedup_teorico = static_cast<double>(threads);

        // Calcoli per grafici rimanenti (rimuovo tempo_ratio e throughput_ratio)
        double cv_aos = (stdev_par_aos / mean_par_aos) * 100;
        double cv_soa = (stdev_par_soa / mean_par_soa) * 100;
        double overhead_aos = ((mean_par_aos * threads) - mean_seq_aos) / mean_seq_aos * 100;
        double overhead_soa = ((mean_par_soa * threads) - mean_seq_soa) / mean_seq_soa * 100;

        // Stampa risultati AOS
        std::cout << "│    " << std::setw(2) << threads << "   │   AOS   │ "
                  << std::fixed << std::setw(6) << std::setprecision(3) << mean_par_aos
                  << " ± " << std::setw(5) << stdev_par_aos << "s │ "
                  << std::setw(8) << std::setprecision(0) << mean_thr_aos << " pwd/s │ "
                  << std::setw(6) << std::setprecision(2) << speedup_aos << "x    │" << std::endl;

        // Stampa risultati SOA
        std::cout << "│         │   SOA   │ "
                  << std::fixed << std::setw(6) << std::setprecision(3) << mean_par_soa
                  << " ± " << std::setw(5) << stdev_par_soa << "s │ "
                  << std::setw(8) << std::setprecision(0) << mean_thr_soa << " pwd/s │ "
                  << std::setw(6) << std::setprecision(2) << speedup_soa << "x    │" << std::endl;

        if (threads < max_threads) {
            std::cout << "├─────────┼─────────┼─────────────────┼──────────────────┼─────────────┤" << std::endl;
        }

        // Salva nel riepilogo
        summary << threads << "," << mean_par_aos << "," << stdev_par_aos << "," << mean_thr_aos << ","
                << speedup_aos << "," << mean_par_soa << "," << stdev_par_soa
                << "," << mean_thr_soa << "," << speedup_soa << "\n";

        // Salva dati per grafici esistenti
        bar_chart_csv << threads << "," << mean_par_aos << "," << mean_par_soa << ","
                      << stdev_par_aos << "," << stdev_par_soa << "\n";
        line_chart_csv << threads << "," << speedup_aos << "," << speedup_soa << ","
                       << speedup_teorico << "\n";

        // Salva dati per grafici rimanenti (RIMUOVO scalability_csv)
        throughput_csv << threads << "," << mean_thr_aos << "," << mean_thr_soa << ","
                       << stdev_thr_aos << "," << stdev_thr_soa << "\n";
        variability_csv << threads << "," << cv_aos << "," << cv_soa << "\n";
        overhead_csv << threads << "," << overhead_aos << "," << overhead_soa << "\n";

        // Salva tutti i run nel CSV dettagliato
        for (int run = 0; run < NUM_RUNS; ++run) {
            double speedup_aos_run = mean_seq_aos / times_par_aos[run];
            double speedup_soa_run = mean_seq_soa / times_par_soa[run];

            csv << threads << ",AOS," << (run + 1) << "," << times_par_aos[run] << ","
                << throughputs_par_aos[run] << "," << speedup_aos_run << "\n";
            csv << threads << ",SOA," << (run + 1) << "," << times_par_soa[run] << ","
                << throughputs_par_soa[run] << "," << speedup_soa_run << "\n";
        }
    }

    std::cout << "└─────────┴─────────┴─────────────────┴──────────────────┴─────────────┘" << std::endl;

    // Chiusura di tutti i file (rimuovo scalability_csv)
    csv.close();
    summary.close();
    bar_chart_csv.close();
    line_chart_csv.close();
    throughput_csv.close();
    variability_csv.close();
    overhead_csv.close();

    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "✅ BENCHMARK COMPLETATO" << std::endl;
    std::cout << "📁 risultati_scaling.csv - Dati dettagliati di tutti i run" << std::endl;
    std::cout << "📁 riepilogo_medie.csv - Statistiche riassuntive" << std::endl;
    std::cout << "📊 bar_chart_tempi.csv - Dati per Bar Chart (Tempi)" << std::endl;
    std::cout << "📈 line_chart_speedup.csv - Dati per Line Chart (Speedup)" << std::endl;
    std::cout << "📊 throughput_chart.csv - Throughput assoluto" << std::endl;
    std::cout << "📊 variability_chart.csv - Variabilità misurazioni" << std::endl;
    std::cout << "📊 overhead_chart.csv - Overhead parallelizzazione" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    return 0;
}