#include <benchmark/benchmark.h>

#include <tuple>

#include "xorsep/group.h"
#include "dev_utils/dev_utils.h"
#include "dev_utils/perf_event_helper.h" 


std::tuple<std::vector<std::pair<uint64_t, bool>>, int> prepare() {
    std::vector<std::pair<uint64_t, bool>> kvs = generate_keyvalues(256 / 1.4);
    
    uint8_t *data = new uint8_t[256 / 8];
    // find a feasible hash function, use that to benchmark the equations solver
    int seed = HashGroup::build<uint64_t, MixFamily2<uint64_t, 8>>(kvs, data, 256 / 8, false);
    assert(seed >= 0);
    delete[] data;

    return std::make_tuple(kvs, seed);
}

template<typename BUILD>
static void benchmark_build_function(benchmark::State& state, BUILD build) {
    auto [kvs, seed] = prepare();
    uint8_t *data = new uint8_t[256 / 8];
    {
        PerfEventBenchamrkWrapper e(state);
        for (auto _ : state) {
            benchmark::DoNotOptimize(build(kvs, data, 256/8, seed));
        }
    }
    state.SetItemsProcessed(int64_t(state.iterations()) *
                            int64_t(kvs.size()));
    delete[] data;
}

static void BM_build_native_(benchmark::State& state) {
    benchmark_build_function(state, HashGroup::build_naive_<uint64_t, MixFamily2<uint64_t, 8>>);
}
BENCHMARK(BM_build_native_);

static void BM_build_bitset_(benchmark::State& state) {
    benchmark_build_function(state, HashGroup::build_bitset_<uint64_t, MixFamily2<uint64_t, 8>>);
}
BENCHMARK(BM_build_bitset_);

static void BM_build_bitset_2_(benchmark::State& state) {
    benchmark_build_function(state, HashGroup::build_bitset_2_<uint64_t, MixFamily2<uint64_t, 8>>);
}
BENCHMARK(BM_build_bitset_2_);