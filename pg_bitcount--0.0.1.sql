-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "create extension pg_bitcount" to load this file. \quit
create function pg_bitcount(bit)
returns integer
as '$libdir/pg_bitcount'
language C immutable strict;

