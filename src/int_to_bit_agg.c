// -*- tab-width:4 ; indent-tabs-mode:nil -*-

#include "int_to_bit_agg.h"

// Based on the bitsetbit function in varbit.c:
// https://github.com/postgres/postgres/blob/REL_10_0/src/backend/utils/adt/varbit.c#L1839
void
setBitAtPosition(VarBit *bits, int32 position) {
    bits8 *r;
    int byteNo;
    int bitNo;

    Assert(bits != NULL);

    // Check if the position is within the length of the bitstring.
    if (position < 0 || position >= VARBITLEN(bits)) {
        ereport(ERROR,
                (errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR), errmsg(
                        "bit index %d out of valid range (0..%d)", position,
                        VARBITLEN(bits))));
    }

    // Set bit at position
    r = VARBITS(bits);
    byteNo = position / BITS_PER_BYTE;
    bitNo = BITS_PER_BYTE - 1 - (position % BITS_PER_BYTE);
    r[byteNo] |= (1 << bitNo);
}

// The structure of this function is based on the makeStringAggState function in varlena.c:
// https://github.com/postgres/postgres/blob/REL_10_0/src/backend/utils/adt/varlena.c#L4664
VarBit*
makeIntToBitAggState(FunctionCallInfo fcinfo, int32 bitlen) {
    VarBit* state;
    int rlen;
    MemoryContext aggcontext;
    MemoryContext oldcontext;

    if (!AggCheckCallContext(fcinfo, &aggcontext)) {
        /* cannot be called directly because of internal-type argument */
        elog(ERROR, "int_to_bit_agg_transfn called in non-aggregate context");
    }

    /*
     * Create state in aggregate context.  It'll stay there across subsequent
     * calls.
     */
    oldcontext = MemoryContextSwitchTo(aggcontext);

    // Initialise an empty bitstring if none is provided.
    rlen = VARBITTOTALLEN(bitlen);
    state = (VarBit*) palloc0(rlen);
    SET_VARSIZE(state, rlen);
    VARBITLEN (state) = bitlen;

    MemoryContextSwitchTo(oldcontext);

    return state;
}
