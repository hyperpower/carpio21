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
    main_simple();
} 