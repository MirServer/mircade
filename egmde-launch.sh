#!/bin/sh

bindir=$(dirname $0)

if [ "${bindir}" != "" ]; then bindir="${bindir}/"; fi

mkdir -p ${XDG_RUNTIME_DIR}

for cmd in $(find ${SNAP}/bin -name setup-*)
do
    $cmd
done

exec ${bindir}egmde "$@"