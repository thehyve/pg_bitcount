#!/usr/bin/env bash
if test -z "$PG_VER"; then
  echo "Variable \$PG_VER is not set." >&2; exit 1
else
  echo "Building container for ${CI_REGISTRY_IMAGE}:${PG_VER} ..."
fi
if test -z "$CI_PROJECT_DIR"; then
  echo "Variable \$CI_PROJECT_DIR is not set." >&2; exit 1
fi
export BUILDAH_FORMAT=docker
NEWCONTAINER=$(buildah --storage-driver vfs from postgres:${PG_VER})
SCRATCHMNT=$(buildah --storage-driver vfs mount ${NEWCONTAINER})
podman --cgroup-manager=cgroupfs --storage-driver vfs run --systemd=false -v ${SCRATCHMNT}:/mnt:rw -v ${CI_PROJECT_DIR}:/root/project:rw --env DESTDIR=/mnt registry.gitlab.com/thehyve/ci_images/of_pg_dev:${PG_VER} bash -c "cd /root/project && make install"
buildah --storage-driver vfs unmount ${NEWCONTAINER}
buildah --storage-driver vfs commit ${NEWCONTAINER} ${CI_PROJECT_NAME}
buildah --storage-driver vfs tag ${CI_PROJECT_NAME} ${CI_REGISTRY_IMAGE}:${PG_VER}
buildah --storage-driver vfs push --creds ${CI_REGISTRY_USER}:${CI_JOB_TOKEN} ${CI_REGISTRY_IMAGE}:${PG_VER}
