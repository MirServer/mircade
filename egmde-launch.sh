#!/bin/sh

bindir=$(dirname $0)

if [ "${bindir}" != "" ]; then bindir="${bindir}/"; fi

mkdir -p ${XDG_RUNTIME_DIR}

# We need to do this for the user that launches scummvm, so
# it can't be done on installation
if [ ! -f "${XDG_CONFIG_HOME}/starfighter/conf" ]
then
  # Initialize the configuration
  mkdir -p "${XDG_CONFIG_HOME}/starfighter"
  echo "1 1 1 0" > "${XDG_CONFIG_HOME}/starfighter/conf"
fi

exec ${bindir}egmde "$@"