#!/usr/bin/env bash
PGPASSWORD=${POSTGRES_PASSWORD}
PGUSER=${POSTGRES_USER}
PGDATABASE=${POSTGRES_DB}
PGHOST="registry.gitlab.com__thehyve__pg_bitcount"
make installcheck
