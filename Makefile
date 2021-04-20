EXTENSION = pg_bitcount       # the extensions name
DATA = pg_bitcount--0.0.3.sql  # script files to install
REGRESS = bitcount_test

# postgres build stuff
EXTRA_CLEAN = performance_tester src/performance_test.o
MODULE_big = pg_bitcount
OBJS = src/pg_bitcount.o src/bitcount.o src/int_to_bit_agg.o
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# performance comparison tool
INCLUDE_LIBS := -L$(shell $(PG_CONFIG) --pkglibdir)

performance_tester: src/performance_test.o
	$(CC) $(CFLAGS) -o performance_tester src/performance_test.o $(INCLUDE_LIBS) -lpgcommon -lpgport
