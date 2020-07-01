#!/bin/sh

bindir=$(dirname $0)

if [ "${bindir}" != "" ]; then bindir="${bindir}/"; fi

mkdir -p ${XDG_RUNTIME_DIR}

for cmd in $(find ${SNAP}/bin -name setup-\*)
do
    $cmd
done

export MIR_SERVER_WALLPAPER_TOP=0x7f7fff
export MIR_SERVER_WALLPAPER_BOTTOM=0x3f7f3f
exec ${bindir}egmde "$@"