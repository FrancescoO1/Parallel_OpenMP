#ifndef MEDIA_H
#define MEDIA_H

#pragma once
#include <vector>
#include <utility>

// Funzione per calcolare media e deviazione standard
std::pair<double, double> calculateStats(const std::vector<double>& values);

#endif // MEDIA_H