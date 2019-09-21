#pragma once

#include <random>
#include <vector>

#include "ssfehash/hash_family.h"

inline bool get_bit(uint8_t *data, int i) {
    return data[i / 8] & (1 << (i % 8));
}

inline void set_bit(uint8_t *data, int i, bool val = true) {
    data[i / 8] = (data[i / 8] & ~(1 << (i % 8))) | (uint8_t(val) << (i % 8));
}

inline void flip_bit(uint8_t *data, int i, bool cond = true) {
    data[i / 8] ^= (int(cond) << (i % 8));
}

namespace HashGroup {

template <typename KEY_TYPE>
bool build_naive_(const std::vector<std::pair<KEY_TYPE, bool> > &kvs,
                 uint8_t *data, size_t data_size, int hash_family) {
    int n = kvs.size();
    int m = data_size * 8;
    HashFamily<KEY_TYPE> h;

    // build the hash matrix
    bool a[n][m + 1];
    memset(a, 0, sizeof(a));
    for (int i = 0; i < n; i++) {
        auto [h1, h2, h3] = h.hash(kvs[i].first, hash_family, data_size * 8);
        a[i][h1] ^= true;
        a[i][h2] ^= true;
        a[i][h3] ^= true;
        a[i][m] = kvs[i].second;
    }

    // do gauess elimnation
    int j = 0;                     // the column with first non-zero entry
    for (int i = 0; i < n; i++) {  // i-th row
        // find a row s.t. a[row][j] = true, then swap it to i-th row
        bool found = false;
        for (; j < m; j++) {
            for (int row = i; row < n; row++) {
                if (a[row][j]) {
                    // swap a[row] and a[i]
                    for (int k = j; k < m + 1; k++) {
                        std::swap(a[i][k], a[row][k]);
                    }
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        // printf("i = %d, j = %d\n", i, j);
        if (!found) return false;

        // elimnate other rows which j-th column elements are true
        for (int k = i + 1; k < n; k++) {  // elimnate k-th row
            if (a[k][j]) {
                // set l < m + 1 to xor the answer
                for (int l = j; l < m + 1; l++) {
                    a[k][l] ^= a[i][l];
                }
            }
        }
    }

    // calculate result
    memset(data, 0, data_size);
    for (int i = n - 1; i >= 0; i--) {
        // find the first non-zero column
        for (j = 0; j < m && a[i][j] == false; j++)
            ;
        assert(j < m);

        set_bit(data, j, a[i][m]);
        for (int k = j + 1; k < m; k++) {
            flip_bit(data, j, a[i][k] & get_bit(data, k));
        }
        // printf("i = %d, x = %d\n", i, int(x_[i]));
    }

    return true;
}

template <typename KEY_TYPE>
int build(const std::vector<std::pair<KEY_TYPE, bool> > &kvs,
                     uint8_t *data, size_t data_size) {
    const int hash_family_index_size = 1;

    // ensure the data size is large enough
    assert((data_size - hash_family_index_size) * 8 >= kvs.size());

    // try to construct with all hash families, and return the first successed
    // one.
    for (int i = 0; i < 100; ++i) {
        if (build_naive_<KEY_TYPE>(kvs, data + hash_family_index_size,
                                  data_size - hash_family_index_size, i)) {
            data[0] = (uint8_t)i;
            return i;
        }
    }

    return -1;
}

template <typename KEY_TYPE>
bool query(KEY_TYPE k, uint8_t *data, int data_size) {
    HashFamily<KEY_TYPE> h;
    auto [h1, h2, h3] = h.hash(k, data[0], (data_size - 1) * 8);
    return get_bit(data + 1, h1) ^ get_bit(data + 1, h2) ^
           get_bit(data + 1, h3);
}

}