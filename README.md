### Radio Edit

The game uses CMake as build system, so its building process is pretty typical.

Dependences (for Debian-based distros):

	sudo apt install liballegro5-dev liballegro-ttf5-dev liballegro-image5-dev liballegro-audio5-dev liballegro-acodec5-dev

Compilation:

	mkdir build; cd build
	cmake ..
	make

Running (from top directory):

	build/src/radioedit

Installation (as root):

	make install

For packaging information, read lib/README.txt

Written by Sebastian Krzyszkowiak <dos@dosowisko.net>
