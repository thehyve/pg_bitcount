# pg_bitcount
[![Build Status](https://travis-ci.org/thehyve/pg_bitcount.svg?branch=master)](https://travis-ci.org/thehyve/pg_bitcount/branches)

PostgreSQL extension providing a bitcount function.


## Build

Build the extension.

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
sudo -u postgres psql -c "create role ${USER} with login superuser" 
# Execute checks
make installcheck
```


## Installation

Install the extension into the PostgreSQL library directory:

```bash
make clean && maken && sudo make install
```


## Usage

```sql
# Register the extension in PostgreSQL
create extension pg_bitcount;

# Use the pg_bitcount function
select public.pg_bitcount(127::bit(8)); -- 7
select public.pg_bitcount(B'101010101'); -- 5 
select public.pg_bitcount((17^15)::bigint::bit(128) << 64 | (17^14)::bigint::bit(128)); -- 58
```


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
