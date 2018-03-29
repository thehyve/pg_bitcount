create extension pg_bitcount;
select pg_bitcount(B'1'::bit(1));
select pg_bitcount(B'0');
select pg_bitcount(B'101010101');

