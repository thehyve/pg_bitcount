#!/usr/bin/env bash
export BUILDAH_FORMAT=docker
NEWCONTAINER=$(buildah --storage-driver vfs from postgres:${PG_VER})
SCRATCHMNT=$(buildah --storage-driver vfs mount ${NEWCONTAINER})
podman --cgroup-manager=cgroupfs --storage-driver vfs run --systemd=false -v ${SCRATCHMNT}:/mnt:rw -v ${CI_PROJECT_DIR}:/root/project:rw --env DESTDIR=/mnt registry.gitlab.com/thehyve/ci_images/of_pg_dev:${PG_VER} bash -c "cd /root/project && make install"
buildah --storage-driver vfs unmount ${NEWCONTAINER}
buildah --storage-driver vfs commit ${NEWCONTAINER} ${CI_PROJECT_NAME}
buildah --storage-driver vfs tag ${CI_PROJECT_NAME} ${CI_REGISTRY_IMAGE}:${PG_VER}
buildah --storage-driver vfs push --creds ${CI_REGISTRY_USER}:${CI_JOB_TOKEN} ${CI_REGISTRY_IMAGE}:${PG_VER}
