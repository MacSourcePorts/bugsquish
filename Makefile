# Makefile for bugsquish

# by Bill Kendrick
# bill@newbreedsoftware.com
# http://www.newbreedsoftware.com/

# March 19, 2000 - December 14, 2000


# User-definable stuff:

BIN_PREFIX=/usr/local/bin/
DATA_PREFIX=/usr/share/bugsquish/


# Defaults for Linux:

TARGET=bugsquish
TARGET_DEF=LINUX


CFLAGS=-Wall -O2 $(SDL_CFLAGS) -DDATA_PREFIX=\"$(DATA_PREFIX)\" \
        -D$(NOSOUNDFLAG) -D$(TARGET_DEF)


# Other definitions:

MIXER=-lSDL_mixer
IMAGE=-lSDL_image
NOSOUNDFLAG=__SOUND
SDL_LIB=$(SDL_LDFLAGS) $(MIXER) $(IMAGE)
SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

installbin = install -g root -o root -m 755 
installdat = install -g root -o root -m 644


# Make commands:

all:	$(TARGET)

install:
	mkdir -p $(DATA_PREFIX)
	cp -r data/* $(DATA_PREFIX)
	chown -R root.root $(DATA_PREFIX)
	chmod -R a+rX,g-w,o-w $(DATA_PREFIX)
	cp bugsquish $(BIN_PREFIX)
	chown root.root $(BIN_PREFIX)/bugsquish
	chmod a+rx,g-w,o-w $(BIN_PREFIX)/bugsquish


emtest:
	make bugsquish TARGET_DEF=EMBEDDED DATA_PREFIX=e/

embedded:
	make bugsquish TARGET_DEF=EMBEDDED \
		DATA_PREFIX=/opt/QtPalmtop/share/bugsquish/ \
		SDL_LIB="/usr/local/arm/lib/libSDL_mixer.a /usr/local/arm/lib/libSDL.a -L/usr/local/arm/lib/ -lpthread -L/opt/Qtopia/sharp/lib -lqpe -lqte" \
                CC=/usr/local/arm/bin/arm-linux-gcc
	/usr/local/arm/bin/arm-linux-strip bugsquish


nosound:
	make bugsquish MIXER= NOSOUNDFLAG=NOSOUND

win32:
	make TARGET_DEF=WIN32 TARGET=bugsquish.exe DATA_PREFIX=data/
	cp /usr/local/cross-tools/i386-mingw32/lib/SDL*.dll .
	chmod 644 SDL*.dll

clean:
	-rm bugsquish bugsquish.exe
	-rm *.o
	-rm SDL*.dll


# Main executable:

$(TARGET):	bugsquish.o
	$(CC) $(CFLAGS) bugsquish.o -o $(TARGET) $(SDL_LIB)


# Main object:

bugsquish.o:	bugsquish.c \
		data/images/flyswatter.xbm \
		data/images/flyswatter-mask.xbm
