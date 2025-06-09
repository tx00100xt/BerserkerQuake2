#############################################################################
#
#  Breserker Quake2 Makefile for Linux 
#
# Jun 2025 by tx00100xt <t.x00100x.yandex.ru>
#
# ELF only
#
#############################################################################


# Check OS type.
OSTYPE := $(shell uname -s)

# this nice line comes from the linux kernel makefile
ARCH := $(shell uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc/ -e s/sparc64/sparc/ -e s/arm.*/arm/ -e s/sa110/arm/ -e s/alpha/axp/)


#############################################################################
# OPTIONS
#############################################################################

VERSION=1.45
ENGINE=berserkerq2
GAMELIB=libgame
BUILD_DEBUG_DIR=build_debug
BUILD_RELEASE_DIR=build_release
BINDIR=berserkerq2
BASEDIR=baseq2
CLIENT_DIR=.

#############################################################################
# SETUP
#############################################################################


CC?=gcc
BASE_CFLAGS?=-Wno-missing-braces -Wno-switch
DEBUG_CFLAGS=$(BASE_CFLAGS) -g -ggdb -Wall -pipe

ifeq ($(OSTYPE),Linux)
  RELEASE_CFLAGS=$(BASE_CFLAGS) -O3 -ffast-math -funroll-loops -fomit-frame-pointer -fno-strict-aliasing -fexpensive-optimizations -DNDEBUG
else ifeq ($(OSTYPE),OpenBSD)
  RELEASE_CFLAGS=$(BASE_CFLAGS) -O3 -ffast-math -funroll-loops -fomit-frame-pointer -fno-strict-aliasing -I/usr/local/include -I/usr/X11R6/include
endif

ifeq ($(ARCH),i386)
  RELEASE_CFLAGS+=-falign-loops=2 -falign-jumps=2 -falign-functions=2 -fno-strict-aliasing
endif

ifeq ($(OSTYPE),Linux)
  LDFLAGS_GAME= -lm -Wl,-rpath,$$ORIGIN/lib
else ifeq ($(OSTYPE),OpenBSD)
  LDFLAGS_GAME=-L/usr/local/lib -L/usr/X11R6/lib -lm -Wl,-rpath,$$ORIGIN/lib
endif

ifeq ($(OSTYPE),Linux)
  LDFLAGS_ENGINE=-lm -lGL -lvorbis -lvorbisfile -lz -lminizip -lSDL2 -lpng -ljpeg -Wl,-rpath,$$ORIGIN/lib
else ifeq ($(OSTYPE),OpenBSD)
  LDFLAGS_ENGINE=-L/usr/local/lib -L/usr/X11R6/lib -lminizip -lm -lGL -lvorbis -lvorbisfile -lz -lSDL2 -lpng -ljpeg -Wl,-rpath,$$ORIGIN/lib
endif

ifeq ($(OSTYPE),Linux)
  INSTALL_PREFFIX=/usr
else ifeq ($(OSTYPE),OpenBSD)
  INSTALL_PREFFIX=/usr/local
endif

ifeq ($(OSTYPE),Linux)
  ifeq ($(ARCH),i386)
    INSTALL_LIB=$(INSTALL_PREFFIX)/lib
  else ifeq ($(ARCH),x86_64)
    INSTALL_LIB=$(INSTALL_PREFFIX)/lib
  else ifeq ($(ARCH),amd64)
    INSTALL_LIB=$(INSTALL_PREFFIX)/lib64
  endif
else ifeq ($(OSTYPE),OpenBSD)
  INSTALL_LIB=$(INSTALL_PREFFIX)/lib
endif

INSTALL_BIN=$(INSTALL_PREFFIX)/bin

SHLIBEXT=so
SHLIBCFLAGS=-fPIC
SHLIBLDFLAGS=-shared

DO_CC=$(CC) $(CFLAGS) -o $@ -c $<
DO_SHLIB_CC=$(CC) $(CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

#############################################################################
# BUILD
#############################################################################


.PHONY : targets debug release clean install install.local deinstall deinstall.local


TARGETS+=$(BUILDDIR)/$(BINDIR)/$(ENGINE)
TARGETS+=$(BUILDDIR)/$(BINDIR)/$(BASEDIR)/$(GAMELIB).$(SHLIBEXT)

all:
	@echo 
	@echo "Possible targets:"
	@echo
	@echo ">> make release          build the binary for a release."
	@echo ">> make debug            build the binary for debuging mode."
	@echo ">> make clean            remove building dir."
	@echo 
	@echo ">> For release version:"
	@echo 
	@echo ">> make install          will install to system dir."
	@echo ">> make install.local    will install to your berserkerq2 home dir."
	@echo ">> make deinstall        will uninstall to system dir."
	@echo ">> make deinstall.local  will uninstall to your berserkerq2 home dir."
	@echo 

debug:
	@-mkdir -p $(BUILD_DEBUG_DIR) \
		$(BUILD_DEBUG_DIR)/$(BINDIR) \
		$(BUILD_DEBUG_DIR)/$(BINDIR)/$(BASEDIR)
	
	$(MAKE) targets BUILDDIR=$(BUILD_DEBUG_DIR) CFLAGS+="$(DEBUG_CFLAGS)"

release:
	@-mkdir -p $(BUILD_RELEASE_DIR) \
		$(BUILD_RELEASE_DIR)/$(BINDIR) \
		$(BUILD_RELEASE_DIR)/$(BINDIR)/$(BASEDIR)
	
	$(MAKE) targets BUILDDIR=$(BUILD_RELEASE_DIR) CFLAGS+="$(RELEASE_CFLAGS)"

targets: $(TARGETS)

#############################################################################
# CLIENT/SERVER
#############################################################################

BERSERKERQ2_OBJS = \
	$(BUILDDIR)/main.o \
	$(BUILDDIR)/unpak.o


$(BUILDDIR)/$(BINDIR)/$(ENGINE) : $(BERSERKERQ2_OBJS)
	@echo
	@echo "==================== Linking $@ ===================="
	@echo
	$(CC) $(CFLAGS) -o $@ $(BERSERKERQ2_OBJS) $(LDFLAGS_ENGINE)

$(BUILDDIR)/main.o :     $(CLIENT_DIR)/main.c
	$(DO_CC)
	
$(BUILDDIR)/unpak.o :    $(CLIENT_DIR)/unpak.c
	$(DO_CC)
	

#############################################################################
# GAME LIB
#############################################################################

BERSERKERQ2_LIBS_OBJS = \
	$(BUILDDIR)/game.o


$(BUILDDIR)/$(BINDIR)/$(BASEDIR)/$(GAMELIB).$(SHLIBEXT) : $(BERSERKERQ2_LIBS_OBJS)
	@echo
	@echo "==================== Linking $@ ===================="
	@echo
	$(CC) $(CFLAGS) -o $@ $(BERSERKERQ2_LIBS_OBJS) $(SHLIBLDFLAGS) $(LDFLAGS_GAME)

$(BUILDDIR)/game.o :     	$(CLIENT_DIR)/game.c
	$(DO_SHLIB_CC)


#############################################################################
# MISC
#############################################################################

clean:
	@echo cleaning objects
	@rm -rf \
	$(BUILD_DEBUG_DIR) \
	$(BUILD_RELEASE_DIR)
	@echo .
	@echo .... Done
	
install:
	@echo Copying files to your system dir ...
	@cp -vr $(BUILD_RELEASE_DIR)/$(BINDIR)/$(ENGINE) $(INSTALL_BIN)
	@mkdir -p $(INSTALL_LIB)/$(ENGINE)
	@cp -vr $(BUILD_RELEASE_DIR)/$(BINDIR)/$(BASEDIR)/$(GAMELIB).$(SHLIBEXT) $(INSTALL_LIB)/$(ENGINE)
	@echo .

install.local:
	@echo Copying files to your local game dir ...
	@cp -vr $(BUILD_RELEASE_DIR)/$(BINDIR)/$(ENGINE) ../
	@cp -vr $(BUILD_RELEASE_DIR)/$(BINDIR)/$(BASEDIR)/$(GAMELIB).$(SHLIBEXT) ../$(BASEDIR)
	@echo .
	@echo ... Done

deinstall:
	@echo Removing files to your system dir ...
	@rm $(INSTALL_BIN)/$(ENGINE)
	@rm $(INSTALL_LIB)/$(ENGINE)/$(GAMELIB).$(SHLIBEXT)
	@echo .

deinstall.local:
	@echo Removing files to your local game dir ...
	@rm ../$(ENGINE)
	@rm ../$(BASEDIR)/$(GAMELIB).$(SHLIBEXT)
	@echo .
	@echo ... Done
