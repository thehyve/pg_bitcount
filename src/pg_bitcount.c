// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <utils/varbit.h>
#include "int_to_bit_agg.h"
#include "bitcount.h"

PG_MODULE_MAGIC;

/*
 * pg_bitcount - Counts the number of bits set in a bit string.
 *
 * Syntax: pg_bitcount(arg bit) RETURNS int
 */
PG_FUNCTION_INFO_V1(pg_bitcount);
Datum
pg_bitcount(PG_FUNCTION_ARGS)
{
    VarBit *arg = PG_GETARG_VARBIT_P(0);
    bits8 *begin = VARBITS(arg);
    bits8 *end = VARBITEND(arg);
    int32 count = bitcount(begin, end);
    PG_RETURN_INT32(count);
}

/*
 * pg_int_to_bit_agg - Aggregates integers in a bit string.
 * For each aggregate integer, the bit at that position is set.
 * The second parameter specifies the length of the bit string.
 *
 * Syntax: pg_int_to_bit_agg(value int, length int) RETURNS bit
 */
PG_FUNCTION_INFO_V1(pg_int_to_bit_agg_transfn);
Datum
pg_int_to_bit_agg_transfn(PG_FUNCTION_ARGS)
{
    VarBit *state;
    int32 value;

    if (PG_ARGISNULL(0)) {
        state = makeIntToBitAggState(fcinfo, PG_GETARG_INT32(2));
    } else {
        state = PG_GETARG_VARBIT_P(0);
    }

    if (!PG_ARGISNULL(1)) {
        value = PG_GETARG_INT32(1);
        setBitAtPosition(state, value);
    }

    PG_RETURN_VARBIT_P(state);
}
