// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bitcount.c"

typedef struct
{
    const char* name;
    bitcount_t impl_function;
} test_implementation_t;

static test_implementation_t create_implementation(const char* name, bitcount_t impl_function) {
    test_implementation_t result;
    result.name = name;
    result.impl_function = impl_function;
    return result;
}

static void run_test(test_implementation_t implementation, uint8 *begin, uint8 *end, uint32 expected) {
    const uint32 iterations = 100000;
    clock_t start;
    clock_t duration;
    int32 msec;
    int32 c;

    printf("Run test with '%s' (Expected bit count: %d) ...\n", implementation.name, expected);
    start = clock();
    for (c=0; c < iterations; c++) {
        int32 count = implementation.impl_function(begin, end);
        if (count != expected) {
            fprintf(stderr, "Incorrect bitcount: %d, expected: %d\n", count, expected);
            exit(1);
        }
    }
    duration = clock() - start;
    msec = duration * 1000 / CLOCKS_PER_SEC;
    printf("Test completed with '%s': %d iterations took %d ms.\n", implementation.name, iterations, msec);
}

static void test1(test_implementation_t* implementations, int32 n) {
    const int32 size = 1024;
    bits8 bytes[size];
    uint8 *begin;
    uint8 *end;
    int32 i;

    for (i=0; i < size; i++) {
        bytes[i] = (bits8)1;
    }

    printf("=== Test 1 ===\n");

    for (i=0; i < n; i++) {
        begin = &(bytes[0]);
        end = &(bytes[size]);
        run_test(implementations[i], begin, end, size);
    }
}

static void test2(test_implementation_t* implementations, int32 n) {
    const int32 size = 1024;
    bits8 bytes[size];
    uint8 *begin;
    uint8 *end;
    int32 i;

    for (i=0; i < size; i++) {
        bytes[i] = (bits8)255;
    }

    printf("=== Test 2 ===\n");

    for (i=0; i < n; i++) {
        begin = &(bytes[0]);
        end = &(bytes[size]);
        run_test(implementations[i], begin, end, size*8);
    }
}

static void test3(test_implementation_t* implementations, int32 n) {
    const int32 size = 1024;
    bits8 bytes[size];
    uint8 *begin;
    uint8 *end;
    int32 i;

    for (i=0; i < size; i++) {
        bytes[i] = (bits8)0;
    }

    printf("=== Test 3 ===\n");

    for (i=0; i < n; i++) {
        begin = &(bytes[0]);
        end = &(bytes[size]);
        run_test(implementations[i], begin, end, 0);
    }
}

#define N 4

int main (int argc, char *argv[]) {
    test_implementation_t implementations[N] = {
        create_implementation("dictionary", &bitcount_dictionary),
        create_implementation("popcount", &bitcount_popcount),
        create_implementation("psnip", &bitcount_psnip),
        create_implementation("wegner", &bitcount_wegner)
    };

#if PSNIP_BUILTIN_GNU_HAS_BUILTIN(__builtin_popcount, 3, 4)
    printf("popcount is available.\n");
#else
    printf("popcount is not available!\n");
#endif

    test1(implementations, N);
    test2(implementations, N);
    test3(implementations, N);

    // Success
    return 0;
}
