create extension pg_bitcount;

-- Check some small numbers
select pg_bitcount(B'0');
select pg_bitcount(B'1'::bit(1));
select pg_bitcount(B'101010101');

-- Check exhaustively for the numbers 0 to 2^20.
select bool_and(public.pg_bitcount(i::bit(64)) = length(replace(i::bit(64)::text, '0', ''))) as check
from (select generate_series(0, (2^20)::bigint) as i) data;

-- Check a 128 bit value
select
  data::text,
  public.pg_bitcount(bits) as bitcount,
  length(replace(bits::text, '0', '')) as bitcount_reference,
  public.pg_bitcount(bits) = length(replace(bits::text, '0', '')) as check
from (select (17^15)::bigint::bit(128) << 64 | (17^14)::bigint::bit(128) as bits) data;

