#include "utility/random.hpp"
#include "utility/clock.hpp"
#include "utility/profile.hpp"
#include "multi_seg_benchmark.hpp"

using namespace carpio;

int main(int argc, char** argv) {
    MakeDir("./fig/");

    benchmark_test();
} 