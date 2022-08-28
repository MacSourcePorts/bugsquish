#!/bin/sh

# mkipk.sh
# generates an ipkg for embedded Bugsquish

# Bill Kendrick
# bill@newbreedsoftware.com

# 2002.Apr.13 - 2002.Apr.20


VER=0.0.6


PACKAGE=bugsquish
TMPDIR=tmp
CONTROL=$TMPDIR/CONTROL/control
ARCH=arm
RM=rm

echo "SETTING UP"
mkdir $TMPDIR
mkdir $TMPDIR/CONTROL


echo
echo "MAKING SURE BINARY EXISTS"
make clean
make embedded

echo 
echo "CREATING CONTROL FILE"

echo "Package: $PACKAGE" > $CONTROL
echo "Priority: optional" >> $CONTROL
echo "Version: $VER" >> $CONTROL
echo "Section: games" >> $CONTROL
echo "Architecture: $ARCH" >> $CONTROL
echo "Maintainer: Bill Kendrick (bill@newbreedsoftware.com)" >> $CONTROL
echo "Description: Squish the bugs before they suck all your blood." >> $CONTROL

echo
echo "COPYING DATA FILES"

mkdir -p $TMPDIR/opt/QtPalmtop/share/bugsquish
mkdir $TMPDIR/opt/QtPalmtop/share/bugsquish/sounds/
mkdir $TMPDIR/opt/QtPalmtop/share/bugsquish/music/
mkdir $TMPDIR/opt/QtPalmtop/share/bugsquish/images/
cp -R data/sounds/* $TMPDIR/opt/QtPalmtop/share/bugsquish/sounds/
cp -R data/music/* $TMPDIR/opt/QtPalmtop/share/bugsquish/music/
cp -R data/embedded/images/* $TMPDIR/opt/QtPalmtop/share/bugsquish/images/

echo
echo "CREATING BINARIES"

mkdir -p $TMPDIR/opt/QtPalmtop/bin/
echo "bugsquish" > $TMPDIR/opt/QtPalmtop/bin/bugsquish.sh
chmod 755 $TMPDIR/opt/QtPalmtop/bin/bugsquish.sh
cp bugsquish $TMPDIR/opt/QtPalmtop/bin/


echo "CREATING ICON AND DESKTOP FILE"

mkdir -p $TMPDIR/opt/QtPalmtop/pics/
cp data/images/bug1a.png $TMPDIR/opt/QtPalmtop/pics/bugsquish.png

mkdir -p $TMPDIR/opt/QtPalmtop/apps/Games/
DESKTOP=$TMPDIR/opt/QtPalmtop/apps/Games/bugsquish.desktop
echo "[Desktop Entry]" > $DESKTOP
echo "Comment=Bug squishing game" >> $DESKTOP
echo "Exec=bugsquish.sh" >> $DESKTOP
echo "Icon=bugsquish" >> $DESKTOP
echo "Type=Application" >> $DESKTOP
echo "Name=Bugsquish" >> $DESKTOP


echo
echo "CREATING IPK..."

ipkg-build $TMPDIR

echo
echo "CLEANING UP"

$RM -r $TMPDIR

echo

