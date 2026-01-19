#ifndef METRICS_HPP
#define METRICS_HPP

#include <vector>
#include <chrono>
#include <string>

class Timer
{
public:
    void start();
    void stop();
    double elapsedSeconds() const;

private:
    std::chrono::high_resolution_clock::time_point startTime, endTime;
};

// Measures CPU usage (Linux/macOS only)
double getCPUUtilization();

// Computes speedup: seq_time / par_time
double computeSpeedup(double seq_time, double par_time);

// Statistics
double computeMean(const std::vector<int> &data);
double computeVariance(const std::vector<int> &data);

#endif // METRICS_HPP
