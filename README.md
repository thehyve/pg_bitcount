# pg_bitcount
[![Build Status](https://travis-ci.org/thehyve/pg_bitcount.svg?branch=master)](https://travis-ci.org/thehyve/pg_bitcount/branches)

PostgreSQL extension providing a bitcount function and an aggregate function that aggregates integers in a bit string.


## Installation

### Using Docker

Docker images of PostgreSQL with the `pg_bitcount` extension available
can be found at [Docker Hub](https://hub.docker.com/r/thehyve/pg_bitcount).
To start a Docker container with PostgreSQL 11 and expose
the database server port at port `9432`:
```bash
docker run --detach --publish 9432:5432 thehyve/pg_bitcount:11
```

### From Debian packages

Install the extension for PostgreSQL 11 in Debian or Ubuntu:
```bash
wget https://github.com/thehyve/pg_bitcount/releases/download/0.0.3-2/postgresql-11-pg-bitcount_0.0.3-2_amd64.deb && \
sudo dpkg -i postgresql-11-pg-bitcount_0.0.3-2_amd64.deb
```

### From source

#### Dependencies

The PostgreSQL extension build tool needs to be installed.
```bash
# For Ubuntu/Debian:
sudo apt install postgresql-server-dev-all
```

#### Build

Install the extension into the PostgreSQL library directory:

```bash
wget https://github.com/thehyve/pg_bitcount/archive/0.0.3-2.zip && \
unzip 0.0.3-2.zip && \
cd pg_bitcount-0.0.3-2 && \
make && sudo make install
```


## Usage

```sql
-- Register the extension in PostgreSQL
create extension pg_bitcount version '0.0.3';

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


## Development

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
sudo -u postgres psql -p 5434 -c "create role ${USER} with login superuser" 
# Execute checks
PGPORT=5434 make installcheck
```


## Acknowledgements

The website [Bit Twiddling Hacks](http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive)
by Sean Eron Anderson provides a valuable resource for bit counting algorithms.
From our comparison, the table lookup methods appears to be the fastest
(function `bitcount_dictionary` in [bitcount.c](src/bitcount.c).

We use `pop_count` from the collection of [Portable Snippets](https://github.com/nemequ/portable-snippets) of C code
to avoid dependency on a particular compiler.

 
## License

Copyright &copy; 2018&ndash;2021 &nbsp; The Hyve

The pg_count extension is licensed under the MIT License. See the file [LICENSE](LICENSE).
