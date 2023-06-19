#include "utility/random.hpp"
#include "utility/clock.hpp"
#include "utility/profile.hpp"
#include "two_segments.hpp"
#include "multi_seg_simple.hpp"
#include "multi_seg_benchmark.hpp"
#include "segment_with_name.hpp"

using namespace carpio;

int main(int argc, char** argv) {
    MakeDir("./fig/");

    two_segments_test();
    // intersection_on_segment();
    // MultiSegTestCase1();
    // auto l = GenerateGrid(20);
    // auto l = GenerateParallelSlanted(20);
    // MultiSegTestCase1N2(l);
    // MultiSegTestCase1BenOtt(l);
    main_simple();
    

    // benchmark_test();
} 