EXTENSION = pg_bitcount       # the extensions name
DATA = pg_bitcount--0.0.1.sql  # script files to install
REGRESS = bitcount_test
MODULES = pg_bitcount

# postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

