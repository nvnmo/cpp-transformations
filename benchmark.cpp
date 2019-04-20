#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include "transform.hpp"

#define MAX_SIZE 10000000

double test_map_func(double x)
{
    return x * x;
}

bool test_filter_func(double x)
{
    return (x - sqrt(x)) < 1e-3;
}

int main()
{
    std::vector<double> test_vec;
    // fill up the test container
    for (long i = 0; i < MAX_SIZE; ++i)
        test_vec.push_back((10000 + rand()) / 1000);

    std::cout << "Benchmarking map transform...." << std::endl;
    auto start_map = std::chrono::high_resolution_clock::now();
    transform::map<double, decltype(test_vec)>(test_vec, test_map_func);
    auto end_map = std::chrono::high_resolution_clock::now();
    std::cout << "Done!" << std::endl;

    std::cout << "Benchmarking reduce transform...." << std::endl;
    auto start_reduce = std::chrono::high_resolution_clock::now();
    transform::reduce<double, decltype(test_vec)>(test_vec, test_map_func, 0);
    auto end_reduce = std::chrono::high_resolution_clock::now();
    std::cout << "Done!" << std::endl;

    std::cout << "Benchmarking filter transform...." << std::endl;
    auto start_filter = std::chrono::high_resolution_clock::now();
    transform::filter<double, decltype(test_vec)>(test_vec, test_filter_func);
    auto end_filter = std::chrono::high_resolution_clock::now();
    std::cout << "Done!" << std::endl;

    std::cout << "Results" << std::endl
              << "Map   : " << std::chrono::duration_cast<std::chrono::milliseconds>(end_map - start_map).count() << "ms\n"
              << "Reduce: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_reduce - start_reduce).count() << "ms\n"
              << "Filter: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_filter - start_filter).count() << "ms\n";
    return 0;
}