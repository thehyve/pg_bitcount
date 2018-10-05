// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#ifndef INT_TO_BIT_AGG_H
#define INT_TO_BIT_AGG_H

//#include <c.h>
#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <utils/varbit.h>

/*
 * setBitAtPosition
 *
 * Given an instance of type 'bit', sets the bit at position 'position'.
 * The purpose of this function is to be used in the pg_int_to_bit_agg aggregate.
 *
 * The bit location is specified left-to-right in a zero-based fashion
 * consistent with the get_bit and set_bit functions.
 */
void setBitAtPosition(VarBit *bits, int32 position);

/*
 * makeIntToBitAggState
 *
 * Initialises a state of type 'bit' of length 'bitlen' for the
 * pg_int_to_bit_agg aggregate.
 */
VarBit* makeIntToBitAggState(FunctionCallInfo fcinfo, int32 bitlen);

#endif // INT_TO_BIT_AGG_H
