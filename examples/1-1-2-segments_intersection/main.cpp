#include "utility/random.hpp"
#include "utility/clock.hpp"
#include "utility/profile.hpp"
#include "two_segments.hpp"
#include "multi_segments.hpp"
#include "segment_with_name.hpp"

using namespace carpio;

int main(int argc, char** argv) {
    MakeDir("./fig/");

    two_segments_test();
    // intersection_on_segment();
    // MultiSegTestCase1();
    auto l = GenerateRandomSegments(20, 0, 100, 0, 100);
    MultiSegTestCase1N2(l);
    MultiSegTestCase1BenOtt(l);

    benchmark_test();
}