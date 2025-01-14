#include <gtest/gtest.h>

#include "xorsep/hash_family.h"
#include "xxhash.h"

template <class HASH_FAMILY>
void test_hash_family() {
    HASH_FAMILY h;

    int a = 123;
    uint32_t b = 123;
    int64_t c = 123;
    int d = 432;

    EXPECT_EQ(h.hash3(a, 42), h.hash3(b, 42));
    EXPECT_EQ(h.hash1(a), h.hash1(b));

    EXPECT_EQ(h.hash3(b, 42), h.hash3(c, 42));
    EXPECT_EQ(h.hash1(b), h.hash1(c));

    EXPECT_NE(h.hash3(c, 42), h.hash3(d, 42));
    EXPECT_NE(h.hash1(c), h.hash1(d));
}

TEST(HashFamilies, Basic) {
    // TODO: Fix the error when the key type is long double
    test_hash_family<Murmur3Family<uint64_t> >();
    // test_hash_family<CRC32Family<long double> >();
    test_hash_family<CRC32Family<uint64_t> >();
    // test_hash_family<XXH32Family<long double> >();
    test_hash_family<XXH32Family<uint64_t> >();

    test_hash_family<MixFamily2<uint64_t, 10> >();
    test_hash_family<MixFamily2<uint64_t, 8> >();
    test_hash_family<MixFamily3<uint64_t, 10> >();
}

TEST(xxHash, Basic) {
    uint64_t a = 233333333333333u;
    uint64_t b = 123;
    XXH32_hash_t h1 = XXH32(&a, sizeof(a), 0);
    XXH32_hash_t h2 = XXH32(&b, sizeof(b), 0);
    //printf("%d %d\n", h1, h2);
    EXPECT_NE(h1, h2);
}