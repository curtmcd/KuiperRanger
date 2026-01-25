# Makefile for Kuiper Ranger
#    Run "./prebuild.sh" before building from clean
#    Run "make -j" to build native app
#    Run "EMSDK=/../emsdk.git make -j" to build native AND Emscripten web app

vpath %.cpp src
vpath %.hpp src

# Build configuration
BUILD_DIR ?= build

# Prefix includes DESTDIR to support distro package build
PREFIX ?= $(DESTDIR)/usr

# Build native always (gcc/SDL2)
PLATFORMS := native

# Build Emscripten if EMSDK is set to an Emscripten root dir
ifdef EMSDK
PLATFORMS += emscripten
endif

# Default: build all platforms
.DEFAULT_GOAL := all

# Source files
SRCS1 =	alien.cpp button.cpp debris.cpp extras.cpp \
	machine.cpp game.cpp ghost.cpp help.cpp highlist.cpp \
	line.cpp linefont.cpp main.cpp missile.cpp mlist.cpp \
	paused.cpp persist.cpp plot.cpp rocks.cpp score.cpp \
	shape.cpp ship.cpp sound.cpp speaker.cpp sprite.cpp \
	text.cpp title.cpp

HDRS1 =	alien.hpp button.hpp debris.hpp extras.hpp \
	machine.hpp game.hpp ghost.hpp help.hpp highlist.hpp \
	linefont.hpp line.hpp missile.hpp mlist.hpp param.hpp \
	paused.hpp persist.hpp plot.hpp point.hpp rand.hpp \
	rocks.hpp score.hpp shape.hpp ship.hpp sound.hpp \
	speaker.hpp sprite.hpp text.hpp title.hpp type.hpp \
	vect.hpp

# Sound test files
SRCS2 = soundtest.cpp sound.cpp
HDRS2 = $(HDRS1)

########################################################################
# Per-platform explicit rules (non-recursive)
########################################################################

# Native (gcc & SDL2)
CXX_native := g++

# To compile for debugging with memory leak checking, switch
# DEBUG_native to the -g line, then run using:
#   LSAN_OPTIONS=suppressions=lsupp.txt ./build/native/bin/kuiper-ranger
#DEBUG_native = -g -fsanitize=address
DEBUG_native = -O2 -fno-strict-aliasing

CXXFLAGS_native := $(DEBUG_native) -D_REENTRANT -Wall -Wextra -Wconversion -Wshadow -Werror
LIBS_native := -lSDL2 -lm -lstdc++

NATIVE_BIN := $(BUILD_DIR)/native/bin
NATIVE_OBJ := $(BUILD_DIR)/native/obj
NATIVE_OBJS1 := $(patsubst %.cpp,$(NATIVE_OBJ)/%.o,$(SRCS1))
NATIVE_OBJS2 := $(patsubst %.cpp,$(NATIVE_OBJ)/%.o,$(SRCS2))

$(NATIVE_BIN)/kuiper-ranger: $(NATIVE_OBJS1)
	@mkdir -p $(NATIVE_BIN)
	$(CXX_native) $(CXXFLAGS_native) -o $@ $^ $(LIBS_native)

$(NATIVE_BIN)/soundtest: $(NATIVE_OBJS2)
	@mkdir -p $(NATIVE_BIN)
	$(CXX_native) $(CXXFLAGS_native) -o $@ $^ $(LIBS_native)

$(NATIVE_OBJ)/%.o: %.cpp
	@mkdir -p $(NATIVE_OBJ)
	$(CXX_native) $(CXXFLAGS_native) -c $< -o $@

# Emscripten
CXX_emscripten := em++
CXXFLAGS_emscripten := -D_REENTRANT -s USE_SDL=2 -O3
LDFLAGS_emscripten := -s USE_SDL=2 --shell-file shell_min.html

EM_BIN := $(BUILD_DIR)/emscripten/bin
EM_OBJ := $(BUILD_DIR)/emscripten/obj
EM_OBJS1 := $(patsubst %.cpp,$(EM_OBJ)/%.o,$(SRCS1))

$(EM_BIN)/kuiper-ranger.html: $(EM_OBJS1) shell_min.html
	@mkdir -p $(EM_BIN)
	$(CXX_emscripten) -o $@ $(LDFLAGS_emscripten) $(EM_OBJS1)

$(EM_OBJ)/%.o: %.cpp
	@mkdir -p $(EM_OBJ)
	$(CXX_emscripten) $(CXXFLAGS_emscripten) -c $< -o $@

# phony platform targets to build only one platform
.PHONY: native emscripten emscripten_check build-all all

# Build native (gcc + SDL2) binaries under $(BUILD_DIR)/native/bin
native: $(NATIVE_BIN)/kuiper-ranger $(NATIVE_BIN)/soundtest

# Build emscripten target under $(BUILD_DIR)/emscripten/bin
# If emsdk_env.sh has not yet been sourced, do that and restart recursively
emscripten:
	if command -v $(CXX_emscripten) >/dev/null 2>&1; then \
		$(MAKE) emscripten_aux; else \
		export EMSDK_QUIET=1 && BASH_SOURCE=$(EMSDK)/emsdk_env.sh \
		. $(EMSDK)/emsdk_env.sh && $(MAKE) emscripten_aux; fi

emscripten_aux: $(EM_BIN)/kuiper-ranger.html

build-all: $(PLATFORMS)

# Default all builds everything (same as build-all)
all: build-all

########################################################################
# Common rules
########################################################################

install:
	install -d $(PREFIX)/games
	install -d $(PREFIX)/share/man/man6
	install -d $(PREFIX)/share/applications
	install -d $(PREFIX)/share/icons/hicolor/16x16/apps
	install -d $(PREFIX)/share/icons/hicolor/32x32/apps
	install -d $(PREFIX)/share/icons/hicolor/48x48/apps
	install -d $(PREFIX)/share/icons/hicolor/64x64/apps
	install -d $(PREFIX)/share/icons/hicolor/128x128/apps

	install -m 755 build/native/bin/kuiper-ranger $(PREFIX)/games/
	install -m 644 kuiper-ranger.man $(PREFIX)/share/man/man6/kuiper-ranger.6
	install -m 644 kuiper-ranger.desktop $(PREFIX)/share/applications/

	install -m 644 icons/16x16.png $(PREFIX)/share/icons/hicolor/16x16/apps/kuiper-ranger.png
	install -m 644 icons/32x32.png $(PREFIX)/share/icons/hicolor/32x32/apps/kuiper-ranger.png
	install -m 644 icons/48x48.png $(PREFIX)/share/icons/hicolor/48x48/apps/kuiper-ranger.png
	install -m 644 icons/64x64.png $(PREFIX)/share/icons/hicolor/64x64/apps/kuiper-ranger.png
	install -m 644 icons/128x128.png $(PREFIX)/share/icons/hicolor/128x128/apps/kuiper-ranger.png

uninstall:
	$(RM) $(PREFIX)/games/kuiper-ranger
	$(RM) $(PREFIX)/share/man/man6/kuiper-ranger.6
	$(RM) $(PREFIX)/share/applications/kuiper-ranger.desktop
	$(RM) $(PREFIX)/share/icons/hicolor/*/apps/kuiper-ranger.png

# Generate ID database for GNU id-utils
ID:
	@if command -v mkid; then mkid .; fi

# Dependency generation
.PHONY: depend
depend: $(SRCS1) $(SRCS2) $(HDRS1)
	@# Generate dependency file mapping each source to both native and emscripten object paths
	@# Use the native compiler to produce header lists, then rewrite the target names.
	$(CXX_native) -MM $(SRCS1) $(SRCS2) | \
		sed -E 's|^([^.]+)\.o:|$(NATIVE_OBJ)/\1.o $(EM_OBJ)/\1.o:|' > .depend

# Include dependencies if they exist
-include .depend

# Clean targets
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) .depend ID
