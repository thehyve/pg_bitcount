// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#ifndef BITCOUNT_H
#define BITCOUNT_H

#include <c.h>

typedef int32 (*bitcount_t)(uint8 *begin, uint8 *end);

int32 bitcount_dictionary(uint8 *begin, uint8 *end);

int32 bitcount_popcount(uint8 *begin, uint8 *end);

int32 bitcount_psnip(uint8 *begin, uint8 *end);

int32 bitcount_wegner(uint8 *data, uint8 *end);

int32 bitcount_64bit(uint8 *data, uint8 *end);

int32 bitcount(uint8 *begin, uint8 *end);

#endif // BITCOUNT_H
