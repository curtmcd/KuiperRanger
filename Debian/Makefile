# Makefile for Kuiper Ranger Debian Package
# Run from: KuiperRanger/Debian/

# Extract the version from the changelog (e.g., extracts 2.6.15 from "kuiper-ranger (2.6.15-1) ...")
VERSION := $(shell head -n 1 debian_files/changelog 2>/dev/null | sed 's/.*(\(.*\)-.*).*/\1/')
PKG_NAME := kuiper-ranger
BUILD_DIR := $(PKG_NAME)-$(VERSION)
ORIG_TAR := $(PKG_NAME)_$(VERSION).orig.tar.gz

# Binaries strictly required for this build pipeline
PREREQS := git tar debuild dpkg-buildpackage

.PHONY: all check-prereqs tarball extract prep build clean

all: build

check-prereqs:
	@echo "Checking prerequisites..."
	@for req in $(PREREQS); do \
		if ! command -v $$req >/dev/null 2>&1; then \
			echo "Error: Required command '$$req' is not installed."; \
			echo "Please run: sudo apt install devscripts dpkg-dev"; \
			exit 1; \
		fi \
	done

tarball: $(ORIG_TAR)

$(ORIG_TAR): check-prereqs
	@echo "Creating upstream tarball $(ORIG_TAR)..."
	cd .. && git archive --format=tar.gz --prefix=$(BUILD_DIR)/ -o Debian/$(ORIG_TAR) HEAD ':!Debian' ':!msvc'

extract: $(BUILD_DIR)

$(BUILD_DIR): $(ORIG_TAR)
	@echo "Extracting tarball..."
	tar xf $(ORIG_TAR)

prep: $(BUILD_DIR)/debian

$(BUILD_DIR)/debian: $(BUILD_DIR)
	@echo "Injecting Debian template files..."
	cp -r debian_files $@

build: check-prereqs prep
	@echo "Building Debian package..."
	cd $(BUILD_DIR) && debuild -us -uc

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	rm -f $(ORIG_TAR)
	rm -f $(PKG_NAME)_*.build
	rm -f $(PKG_NAME)_*.buildinfo
	rm -f $(PKG_NAME)_*.changes
	rm -f $(PKG_NAME)_*.ddeb
	rm -f $(PKG_NAME)_*.deb
	rm -f $(PKG_NAME)_*.debian.tar.*
	rm -f $(PKG_NAME)_*.dsc
