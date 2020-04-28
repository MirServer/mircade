#!/bin/sh

sudo sed --in-place 's~^}$~unix (bind, listen, accept) type=stream addr="@/tmp/.X11-unix/X[0-9]*",\n}~' /var/lib/snapd/apparmor/profiles/snap.mircade.mircade
sudo apparmor_parser -r /var/lib/snapd/apparmor/profiles/snap.mircade.mircade