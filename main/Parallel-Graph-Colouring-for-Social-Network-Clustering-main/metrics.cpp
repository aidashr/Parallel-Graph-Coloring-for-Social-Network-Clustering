#include "metrics.hpp"
#include <fstream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <sys/resource.h>
#include <numeric>
#include <cmath>

void Timer::start()
{
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    endTime = std::chrono::high_resolution_clock::now();
}

double Timer::elapsedSeconds() const
{
    std::chrono::duration<double> diff = endTime - startTime;
    return diff.count();
}

double getCPUUtilization()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    double user = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6;
    double sys = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6;
    return user + sys;
}

double computeSpeedup(double seq_time, double par_time)
{
    return (par_time > 0.0) ? (seq_time / par_time) : 0.0;
}

double computeMean(const std::vector<int> &data)
{
    if (data.empty())
        return 0.0;
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double computeVariance(const std::vector<int> &data)
{
    if (data.empty())
        return 0.0;
    double mean = computeMean(data);
    double sq_sum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);
    return (sq_sum / data.size()) - (mean * mean);
}
