// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <utils/varbit.h>
#include "bitcount.h"

PG_MODULE_MAGIC;

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
