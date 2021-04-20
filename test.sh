#!/usr/bin/env bash
export PGPASSWORD=${POSTGRES_PASSWORD}
export PGUSER=${POSTGRES_USER}
export PGDATABASE=${POSTGRES_DB}
export PGHOST="registry.gitlab.com__thehyve__pg_bitcount"
make installcheck || (cat regression.diffs; exit 1)
