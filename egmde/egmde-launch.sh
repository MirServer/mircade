#!/bin/sh

bindir=$(dirname $0)

if [ "${bindir}" != "" ]; then bindir="${bindir}/"; fi

mkdir -p ${XDG_RUNTIME_DIR}

for cmd in $(find ${SNAP}/bin -name setup-*)
do
    $cmd
done

export MIR_SERVER_WALLPAPER_TOP=0x333333
export MIR_SERVER_WALLPAPER_BOTTOM=0xe95420
exec ${bindir}egmde "$@"