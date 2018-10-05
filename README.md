# pg_bitcount
[![Build Status](https://travis-ci.org/thehyve/pg_bitcount.svg?branch=master)](https://travis-ci.org/thehyve/pg_bitcount/branches)

PostgreSQL extension providing a bitcount function and an aggregate function that aggregates integers in a bit string.


## Build

### Dependencies

The PostgreSQL extension build tool needs to be installed.
```bash
# For Ubuntu/Debian:
sudo apt install postgresql-server-dev-all
```

### Build the extension

```bash
make
```


### Tests
```bash
# Run performance tests (comparing four different implementations)
make performance_tester && ./performance_tester
```

### Run database tests
```bash
# Create database role for testing
sudo -u postgres psql -p 5433 -c "create role ${USER} with login superuser" 
# Execute checks
PGPORT=5433 make installcheck
```


## Installation

Install the extension into the PostgreSQL library directory:

```bash
make clean && make && sudo make install
```


## Usage

```sql
-- Register the extension in PostgreSQL
create extension pg_bitcount;

-- Use the pg_bitcount function
select public.pg_bitcount(127::bit(8)); -- 7
select public.pg_bitcount(B'101010101'); -- 5 
select public.pg_bitcount((17^15)::bigint::bit(128) << 64 | (17^14)::bigint::bit(128)); -- 58

-- Use the pg_int_to_bit_agg aggregate using a bit string of size 24
select public.pg_bitcount(public.pg_int_to_bit_agg(i::int, 24)) from (select generate_series(2, 8) as i) data; -- 7
```

## List of functions

 Schema |    Name           | Result data type | Argument data types |  Type  
--------|-------------------|------------------|---------------------|--------
 public | pg_bitcount       | integer          | bit                 | normal
 public | pg_int_to_bit_agg | bit              | integer, integer    | agg


## Acknowledgements

The website [Bit Twiddling Hacks](http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive)
by Sean Eron Anderson provides a valuable resource for bit counting algorithms.
From our comparison, the table lookup methods appears to be the fastest
(function `bitcount_dictionary` in [bitcount.c](src/bitcount.c).

We use `pop_count` from the collection of [Portable Snippets](https://github.com/nemequ/portable-snippets) of C code
to avoid dependency on a particular compiler.

 
## License

Copyright &copy; 2018 &nbsp; The Hyve

The pg_count extension is licensed under the MIT License. See the file [LICENSE](LICENSE).
