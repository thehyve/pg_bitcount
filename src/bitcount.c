// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#include "bitcount.h"
#include "builtin.h"


/**
 * The number of bits per byte. For a number i (i >=0, i < 256), the
 * value uint8_bits[i] equals the number of bits set in i.
 */
static const uint8 uint8_bits[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3,
    4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4,
    4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4,
    5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5,
    4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3,
    4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5,
    5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

/**
 * \brief Use a dictionary of bit counts for all bytes.
 * Similar to http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetTable.
 */
inline int32 bitcount_dictionary(uint8 *data, uint8 *end) {
    int32 count = 0;
    uint8 *p = data;
    while (p < end) {
        count += uint8_bits[*p];
        ++p;
    }
    return count;
}

/**
 * Use the builtin compiler function __builtin_popcount if available.
 * builtin.h from Portable Snippets is used to ensure portable code,
 * see https://github.com/nemequ/portable-snippets/tree/master/builtin.
 */
inline int32 bitcount_popcount(uint8 *data, uint8 *end) {
    int32 count = 0;
    uint8 *p = data;
    while (p < end) {
        count += psnip_builtin_popcount(*p);
        ++p;
    }
    return count;
}

/**
 * Use the builtin compiler function __builtin_popcountll if available.
 * builtin.h from Portable Snippets is used to ensure portable code,
 * see https://github.com/nemequ/portable-snippets/tree/master/builtin.
 */
inline int32 bitcount_popcountll(uint8 *data, uint8 *end) {
    int32 count = 0;
    uint8 *p = data;
    uint64 word;
    uint8 i;
    while (p < end) {
        word = 0;
        for (i=0; i < 8 && p < end; i++) {
            word &= (*p) << i*8;
            ++p;
        }
        count += psnip_builtin_popcountll(word);
    }
    return count;
}

PSNIP_BUILTIN__POPCOUNT_DEFINE_PORTABLE(popcount8, uint8)

/**
 * Use the fallback popcount implementation from Portable Snippets,
 * see see https://github.com/nemequ/portable-snippets/tree/master/builtin..
 */
inline int32 bitcount_psnip(uint8 *data, uint8 *end) {
    int32 count = 0;
    uint8 *p = data;
    while (p < end) {
        count += psnip_builtin_popcount8(*p);
        ++p;
    }
    return count;
}

/**
 * Counting bits set, Brian Kernighan's way,
 * taken from the list by Sean Eron Anderson,
 * see http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan.
 */
inline int32 bitcount_wegner(uint8 *data, uint8 *end) {
    int32 count = 0; // c accumulates the total bits set in v
    uint8 *p = data;
    uint8 v;
    while (p < end) {
        v = *p;
        for (; v; count++)
        {
          v &= v - 1; // clear the least significant bit set
        }
        ++p;
    }
    return count;
}

/**
 * Counting bits set in 14-bit words using 64-bit instructions,
 * taken from the list by Sean Eron Anderson,,
 * see http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSet64
 */
inline int32 bitcount_64bit(uint8 *data, uint8 *end) {
    uint32 count = 0;
    uint8 *p = data;
    uint8 v;
    while (p < end) {
        v = *p;
        count += (v * 0x200040008001ULL & 0x111111111111111ULL) % 0xf;
        ++p;
    }
    return count;
}

/**
 * \brief Counts the number of bits set in the bit set, which is represented as an array of uint8.
 * \param data a pointer to the first element of the array.
 * \param end a pointer to the first location after the last element of the array.
 * \example
 *     const uint8 bytes[2] = {8, 127};
 *     int32 number_of_bits = bitcount(&(bytes[0]), &(bytes[2]));
 * \return the number of bits set in the array.
 */
inline int32 bitcount(uint8 *data, uint8 *end) {
    return bitcount_dictionary(data, end);
}
