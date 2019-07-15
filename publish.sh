#!/usr/bin/env bash
skopeo copy --dest-creds ${dh_username}:${dh_userpass} docker://${CI_REGISTRY_IMAGE}:${PG_VER} docker://${TRGT_RG}/${RG_USER}/${CI_PROJECT_NAME}:${PG_VER}
