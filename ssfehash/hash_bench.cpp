#include <benchmark/benchmark.h>

#include "ssfehash/hash_family.h"
#include "dev_utils/dev_utils.h"

// Note:
// A general technique used there is that the key, which is for the
// next round, is generated by the hash result of this round. So that, the CPU
// cannot utilize the out of order execution (speculation) there, to reflect
// real scenarios that the memory address is calculated from hash functions.

// generate a shuffled sequence consists of numbers from 0 to 255 (HASH_FAMILY_NUM)
std::vector<int> shuffled_hash_family_indexes() {
    std::vector<int> families;
    for (int i = 0; i < HASH_FAMILY_NUM; i++) {
        families.push_back(i);
    }
    std::mt19937 g(7);
    std::shuffle(families.begin(), families.end(), g);
    return families;
}

template <class HASH>
static void BM_hash3(benchmark::State& state) {
    auto families = shuffled_hash_family_indexes();
    auto h = HASH();
    int i = 0;
    for (auto _ : state) {
        auto [h1, h2, h3] = h.hash3(42414242424u, families[i]);
        i = (h1 ^ h2 ^ h3) % HASH_FAMILY_NUM;
        benchmark::DoNotOptimize(i);
    }
}

template <class HASH>
static void BM_hash3_mod(benchmark::State& state) {
    auto families = shuffled_hash_family_indexes();
    auto h = HASH();
    int i = 0;
    int mod_n = 247;
    for (auto _ : state) {
        auto [h1, h2, h3] = h.hash3(42414242424u, families[i]);
        i = (h1 % mod_n ^ h2 % mod_n^+ h3 % mod_n) % HASH_FAMILY_NUM;
        benchmark::DoNotOptimize(i);
    }
}

template <class HASH>
static void BM_hash1(benchmark::State& state) {
    auto h = HASH();
    int i = 23423431;
    for (auto _ : state) {
        i = h.hash1(i);
        benchmark::DoNotOptimize(i);
    }
}

template <class HASH>
static void BM_hash1_mod(benchmark::State& state) {
    auto h = HASH();
    int mod_n = 2333;
    for (auto _ : state) {
        mod_n = h.hash1(mod_n) % mod_n + 23;
        benchmark::DoNotOptimize(mod_n);
    }
}

BENCHMARK_TEMPLATE(BM_hash3, CRC32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash3, Murmur3Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash3, XXH32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash3, MixFamily2<uint64_t, 8>);
BENCHMARK_TEMPLATE(BM_hash3, MixFamily3<uint64_t, 8>);

BENCHMARK_TEMPLATE(BM_hash3_mod, CRC32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash3_mod, Murmur3Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash3_mod, XXH32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash3_mod, MixFamily2<uint64_t, 10>);
BENCHMARK_TEMPLATE(BM_hash3_mod, MixFamily3<uint64_t, 10>);

BENCHMARK_TEMPLATE(BM_hash1, CRC32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash1, Murmur3Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash1, XXH32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash1, MixFamily2<uint64_t, 8>);
BENCHMARK_TEMPLATE(BM_hash1, MixFamily3<uint64_t, 8>);

BENCHMARK_TEMPLATE(BM_hash1_mod, CRC32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash1_mod, Murmur3Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash1_mod, XXH32Family<uint64_t>);
BENCHMARK_TEMPLATE(BM_hash1, MixFamily2<uint64_t, 10>);
BENCHMARK_TEMPLATE(BM_hash1, MixFamily3<uint64_t, 10>);