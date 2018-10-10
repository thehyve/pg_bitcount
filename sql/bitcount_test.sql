create extension pg_bitcount;

\df pg_bitcount

-- Check some small numbers
select pg_bitcount(B'0');
select pg_bitcount(B'1'::bit(1));
select pg_bitcount(B'101010101');

-- Check exhaustively for the numbers 0 to 2^20.
select bool_and(pg_bitcount(i::bit(64)) = length(replace(i::bit(64)::text, '0', ''))) as check
from (select generate_series(0, (2^20)::bigint) as i) data;

-- Check a 128 bit value
select
  data::text,
  pg_bitcount(bits) as bitcount,
  length(replace(bits::text, '0', '')) as bitcount_reference,
  pg_bitcount(bits) = length(replace(bits::text, '0', '')) as check
from (select (17^15)::bigint::bit(128) << 64 | (17^14)::bigint::bit(128) as bits) data;

\df pg_int_to_bit_agg

-- Aggregate numbers to a bit string
select pg_int_to_bit_agg(i::int, 24)::text as bits
from (select generate_series(2, 8) as i) data;

select pg_int_to_bit_agg(i::int, 24)::text as bits
from (select generate_series(0, 3) as i) data;

-- Aggregating over an empty set requires coalescing 
select coalesce(pg_int_to_bit_agg(i::int, 10), 0::bit(10))::text as bits
from (select 0 as i) data where i > 0;

-- Count bits of aggregate numbers
select pg_bitcount(pg_int_to_bit_agg(i::int, 24))
from (select generate_series(2, 8) as i) data;

-- Error on exceeding bit length 
select pg_int_to_bit_agg(i::int, 24)::text
from (select generate_series(20, 32) as i) data;

select pg_int_to_bit_agg(i::int, 24)::text
from (select generate_series(-5, 5) as i) data;

-- Check aggregate with get_bit. Expect bits 3 and 4 to be set.
select i, get_bit(pg_int_to_bit_agg(j::int, 16), i) as result
from (select generate_series(3, 4) as j) selection,
(select generate_series(0, 15) as i) data
group by i order by i;

-- Check aggregation with null values in the data
create temporary table bitset_testdata (value int);
insert into bitset_testdata values
  (1),
  (2),
  (null),
  (7);

-- Expect the first bit not to be set
select public.pg_int_to_bit_agg(value, 10)::text from bitset_testdata;

-- Expect true
select get_bit(public.pg_int_to_bit_agg(value, 10), 0) = 0 as check from bitset_testdata;
