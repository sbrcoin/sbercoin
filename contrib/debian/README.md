
Debian
====================
This directory contains files used to package sbercoind/sbercoin-qt
for Debian-based Linux systems. If you compile sbercoind/sbercoin-qt yourself, there are some useful files here.

## sbercoin: URI support ##


sbercoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install sbercoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your sbercoinqt binary to `/usr/bin`
and the `../../share/pixmaps/sbercoin128.png` to `/usr/share/pixmaps`

sbercoin-qt.protocol (KDE)

