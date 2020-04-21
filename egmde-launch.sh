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

# We need to do this for the user that launches supertux, so
# it can't be done on installation
if [ ! -f "${HOME}/.local/share/supertux2/config" ]
then
  # Initialize the configuration
  mkdir -p "${HOME}/.local/share/supertux2"
  echo "(supertux-config (video (fullscreen #t)))" > "${HOME}/.local/share/supertux2/config"
fi

exec ${bindir}egmde "$@"