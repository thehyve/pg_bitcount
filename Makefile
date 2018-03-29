EXTENSION = pg_bitcount       # the extensions name
DATA = pg_bitcount--0.0.1.sql  # script files to install
REGRESS = bitcount_test

# postgres build stuff
MODULE_big = pg_bitcount
OBJS = src/pg_bitcount.o src/bitcount.o
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# performance comparison tool
performance_tester: src/performance_test.o
	$(CC) $(CFLAGS) -o performance_tester src/performance_test.o

.PHONY : clean
clean :
	rm -f performance_tester src/*.o src/*.so

