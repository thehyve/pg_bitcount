-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "create extension pg_bitcount" to load this file. \quit
create function public.pg_bitcount(bit)
returns integer
as '$libdir/pg_bitcount', 'pg_bitcount'
language C immutable strict;

create function public.pg_int_to_bit_agg_transfn(bit, int, int)
returns bit
as '$libdir/pg_bitcount', 'pg_int_to_bit_agg_transfn'
language C;

create aggregate public.pg_int_to_bit_agg(int, int) (
    sfunc = pg_int_to_bit_agg_transfn,
    stype = bit
);
