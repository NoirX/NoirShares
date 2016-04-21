
Debian
====================
This directory contains files used to package noirsharesd/noirshares-qt
for Debian-based Linux systems. If you compile noirsharesd/noirshares-qt yourself, there are some useful files here.

## noirshares: URI support ##


noirshares-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install noirshares-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your noirshares-qt binary to `/usr/bin`
and the `../../share/pixmaps/noirshares128.png` to `/usr/share/pixmaps`

noirshares-qt.protocol (KDE)

