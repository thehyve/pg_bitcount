#!/usr/bin/env bash

PG_BITCOUNT_VERSION=0.0.3

BLUE='\033[0;36m'
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

if test -z "$POSTGRESQL_VERSION"; then
  echo -e "\n${RED}Variable \$POSTGRESQL_VERSION is not set.${NC}\n" >&2; exit 1
else
  echo -e "\n${BLUE}Building postgresql-${POSTGRESQL_VERSION}-pg-bitcount ...${NC}\n"
fi

mkdir -p build

git archive --prefix "pg_bitcount-${PG_BITCOUNT_VERSION}/" -o "build/pg-bitcount_${PG_BITCOUNT_VERSION}.orig.tar.gz" HEAD

pushd build

tar xfz "pg-bitcount_${PG_BITCOUNT_VERSION}.orig.tar.gz"
pushd "pg_bitcount-${PG_BITCOUNT_VERSION}"
cp -r ../../debian .

(sed -e "s/PGVERSION/${POSTGRESQL_VERSION}/g" debian/control.in > debian/control) &&\
debuild -uc -us &&\
echo -e "\n${GREEN}Building postgresql-${POSTGRESQL_VERSION}-pg-bitcount successful.${NC}\n" || {
    echo -e "\n${RED}Building postgresql-${POSTGRESQL_VERSION}-pg-bitcount failed.${NC}\n"
    popd; popd
    exit 1
}

popd
popd
