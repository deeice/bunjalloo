Overview
=========

If you have downloaded this in a source tar.gz, then run

    ./configure
    make

If you are using a version from git, then you will need to install waf from
http://code.google.com/p/waf/downloads/list - version 1.5.8 should be used.

If the configuration step fails, and you know you have python installed, then
try running:

   python waf configure

To see all the configuration options, use the "--help" flag.

   ./configure --help
   python waf --help

Prerequisites
=============

First I'll assume you are using Ubuntu 8.04+, or some similar modern Linux
distro. Otherwise you may have to modify these steps a little.

Download and install devkitARM, libnds, dswifi and libfat. These can be
found at the website http://sourceforge.net/projects/devkitpro/

As an alternative you can use a script in the tools directory to install
the files. Set the variable DEVKITPRO to point to the /path/to/devkitpro and
DEVKITARM to point to $DEVKITPRO/devkitARM. With bash for example:

    export DEVKITPRO=$HOME/devkitpro
    export DEVKITARM=$DEVKITPRO/devkitARM

[ TIP: place these lines in your ~/.bashrc file ]

Now install the latest devkitARM, libnds and friends. The script in tools
checks for the latest versions of the devkitPro packages, downloads and
installs them to the $DEVKITPRO directory.

    tools/update-devkitpro.sh

Additionally and optionally download the appropriate DLDI driver for your card.
See chishm's DLDI website for details http://chishm.drunkencoders.com/DLDI/

This is not required if your card automatically patches homebrew, which most
do.

Building dependencies
=====================

The following tools are needed to download and build the 3rd party dependencies:

    sudo apt-get install patch subversion

To build the library dependencies for the DS version, you can use the
download-libs.sh script in the tools directory. This will download a few tar
files, unpack them, build the contents and install the files to
$DEVKITPRO/libnds. You might run something like this:

    tools/download-libs.sh

This should install libpng, libgif, libjpeg, zlib, unzip and matrixSSL into
the $DEVKITPRO/libnds directory.

Alternatively there are pre-compiled libraries available from
http://code.google.com/p/quirkysoft/downloads/list - copy them to
$DEVKITPRO/libnds/ or use the included install.sh script.

Follow download-libs.sh's final instructions to install "unzip" and "matrixSSL"
for Linux.

The following standard development libraries are also needed:

    sudo apt-get install libpng-dev libgif-dev libjpeg-dev \
                         zlib1g-dev libsdl1.2-dev libgl1-mesa-dev sox

Google's C++ test framework is required to compile and run the unit tests. This
is optional (but recommended) and can be gotten from here:

    http://code.google.com/p/googletest/

Patches to these build instructions are welcome in case I've missed off any
other required libraries or tools.

Almost there!
=============

That's the tough bit out the way, hopefully. Now to compile. You can
use the usual "./configure && make" style build:

    ./configure
    make

Or if you are familiar with the waf build system, the equivalent commands are
valid:

    /path/to/waf configure
    /path/to/waf build

If this works, you should now have bunjalloo.nds and bunjalloo executable in
the build directory. First see if the Linux version works:

    cd bunjalloo
    ../_build_/sdl/bunjalloo

If that worked, then you are ready to try out the DS build.

Installing
==========

The build system uses the '--prefix' flag to make the installation of the
required files onto your DLDI supported device easier. In these steps
/media/$disk is the mount point of your media card, you will have to replace it
with the real location:

    ./waf --prefix=/media/$disk configure
    ./waf install
    # optional DLDI step, probably not needed:
    dlditool /path/to/your.dldi /media/$disk/bunjalloo.nds

Once you have done that, and assuming you don't alter things in the data
directory, future builds can be updated by simply running:

    cp -v _build_/default/bunjalloo-patched.nds /media/$disk/bunjalloo.nds

See this page too for more details.
http://code.google.com/p/quirkysoft/wiki/Compiling

Good luck, Commander.
--- MESSAGE ENDS
