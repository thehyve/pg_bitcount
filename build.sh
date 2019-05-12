#!/usr/bin/env bash
NEWCONTAINER=$(buildah --storage-driver vfs from postgres:${PG_VER})
SCRATCHMNT=$(buildah --storage-driver vfs mount ${NEWCONTAINER})
echo ${PWD}
echo ${CI_PROJECT_DIR}
