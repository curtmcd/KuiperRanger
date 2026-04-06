# Kuiper Ranger Debian Package - Complete Setup Guide

## Overview

A complete Debian package configuration has been set up for Kuiper Ranger, enabling easy distribution and installation on Debian-based Linux systems (Ubuntu, Debian, Linux Mint, etc.).

*WARNING:* If it's not obvious from the redundancy, AI was used in creating this document.

## Debian Package Files (`debian/` directory)

```
debian/
├── changelog              # Package version history (Debian format)
├── compat                 # Debhelper compatibility version (13)
├── control                # Package metadata and dependencies
├── copyright              # License information
├── kuiper-ranger.desktop  # Desktop launcher entry
├── postinst               # Post-installation hook script (executable)
└── rules                  # Build and installation rules
```

### 1. **debian/control**
   - Package metadata (name, version, dependencies)
   - Specifies SDL2 as a runtime dependency
   - Includes proper Debian package descriptors
   - Follows Debian Policy Manual standards

### 2. **debian/rules**
   - Build rules for Debian package creation
   - Compiles native binary using the existing Makefile
   - Installs binary to `/usr/games/kuiper-ranger`
   - Installs man page to `/usr/share/man/man6/kuiper-ranger.6`
   - Installs desktop entry to `/usr/share/applications/`
   - Installs icons (5 sizes) to `/usr/share/icons/hicolor/`

### 3. **debian/changelog**
   - Version history in Debian format
   - Current version: 2.6-1
   - Documents package features in changelog

### 4. **debian/compat**
   - Debhelper compatibility version (13)
   - Ensures consistent building across systems

### 5. **debian/copyright**
   - License information (GPL-2.0+)
   - Copyright attribution

### 6. **debian/kuiper-ranger.desktop**
   - Desktop launcher entry
   - Allows launching from application menus
   - References the game icon
   - Proper categorization as a Game application

### 7. **debian/postinst**
   - Post-installation script
   - Automatically creates `/var/games/kuiper-ranger/` directory
   - Creates `scores` file with proper permissions (666)
   - Allows all users to read and write high scores
   - Runs without requiring sudo

## Features Included

✅ **Desktop Integration**
   - Desktop entry file for menu integration
   - Application will appear in system menu
   - Multiple icon sizes (16, 32, 48, 64, 128 pixels)
   - Proper FreeDesktop icon installation

✅ **High Score File Management**
   - Automatic creation of `/var/games/kuiper-ranger/`
   - Automatic creation of `scores` file
   - World-writable permissions for shared high scores
   - No manual setup required by users

✅ **Proper Installation Paths**
   - Binary: `/usr/games/kuiper-ranger`
   - Man page: `/usr/share/man/man6/kuiper-ranger.6`
   - Desktop entry: `/usr/share/applications/kuiper-ranger.desktop`
   - Icons: `/usr/share/icons/hicolor/{size}x{size}/apps/kuiper-ranger.png`

✅ **Standards Compliance**
   - Follows Debian Policy Manual (v4.6.0)
   - Uses debhelper for automation
   - Proper dependency specification including SDL2 library
   - Cross-architecture support (amd64, i386, ARM, ARM64, etc.)

## Building the Package

This directory contains the Debian package configuration files for Kuiper Ranger.

### Prerequisites

Install build dependencies:

```bash
sudo apt-get update
sudo apt-get install build-essential g++ libsdl2-dev
sudo apt-get install debhelper-compat devscripts git-buildpackage
```

### Build

See `BUILD_PACKAGE.md`.

```bash
cd KuiperRanger/Debian/kuiper-ranger-2.6/
# (Assuming the debian/ config files are ready)

# Method 1: Using debuild (recommended)
debuild -us -uc

# Method 2: Using dpkg-buildpackage
dpkg-buildpackage -us -uc

# Method 3: Using git-buildpackage
gbp buildpackage --git-no-sign
```

The resulting `.deb` file will be created in the parent directory
`KuiperRanger/Debian/kuiper-ranger_2.6-1_*.deb`.

### Build Output

Where * is the architecture (e.g. amd64):

```
kuiper-ranger_2.6-1_*.deb          # The main package file
kuiper-ranger_2.6-1.dsc            # Source package descriptor
kuiper-ranger_2.6-1.tar.xz         # Source tarball
kuiper-ranger_2.6-1_*.buildinfo    # Build information
kuiper-ranger_2.6-1_*.changes      # Changes file
```

Once built, verify with:

```bash
# Check package contents
dpkg -c kuiper-ranger_2.6-1_amd64.deb

# Check package dependencies
dpkg-deb -f kuiper-ranger_2.6-1_amd64.deb

# Lint the package
lintian kuiper-ranger_2.6-1_amd64.deb
```

## Installation

### What Happens During Installation

1. **debuild** compiles the binary using the existing Makefile
2. **Install step** places files in their standard locations:
   - Binary → `/usr/games/kuiper-ranger`
   - Man page → `/usr/share/man/man6/`
   - Desktop entry → `/usr/share/applications/`
   - Icons → `/usr/share/icons/hicolor/*/`
3. **postinst** runs automatically:
   - Creates `/var/games/kuiper-ranger/` directory
   - Creates `scores` file if missing
   - Sets permissions to 666 (world-writable)

### Install the Package

```bash
sudo dpkg -i kuiper-ranger_2.6-1_amd64.deb
```

Or using apt:

```bash
sudo apt install kuiper-ranger_2.6-1_amd64.deb
```

### Post-Installation

The package automatically:

1. Installs the binary to `/usr/games/kuiper-ranger`
2. Creates `/var/games/kuiper-ranger/` directory
3. Creates the scores file with world-writable permissions
4. Installs desktop entry to `/usr/share/applications/`
5. Installs 5 icon sizes to `/usr/share/icons/hicolor/`
6. Installs man page to `/usr/share/man/man6/kuiper-ranger.6`

If the system-wide scores file does not exists or is not readable/writable,
scores are stored in the user's home directory instead (see README.md for details).

No additional manual setup is required.

#### Post-Installation Verification ✓

After `sudo dpkg -i kuiper-ranger_2.6-1_*.deb`:

- [x] Binary exists: `which kuiper-ranger`
- [x] High score dir exists: `ls -la /var/games/kuiper-ranger/`
- [x] Scores file created: `ls -la /var/games/kuiper-ranger/scores`
- [x] Desktop entry available: `grep -r kuiper-ranger /usr/share/applications/`
- [x] Icons installed: `ls -la /usr/share/icons/hicolor/*/apps/kuiper-ranger.png`
- [x] Man page available: `man kuiper-ranger`
- [x] Game launches: `/usr/games/kuiper-ranger`

### Launch the Game

#### Command Line
```bash
kuiper-ranger
kuiper-ranger -f          # Full screen mode
kuiper-ranger -w 1024     # Custom window width
kuiper-ranger -h          # Show help
```

#### Desktop Menu
- Look for "Kuiper Ranger" in your application menu
- Click to launch from your desktop environment
- Icons will appear in your application launcher

## File Locations After Installation

### Executable
```
/usr/games/kuiper-ranger
```

### High Score File
```
/var/games/kuiper-ranger/scores        # System-wide (created by package)
~/.local/share/Fishlet/kuiper-ranger/scores    # User-specific (fallback)
```

### Documentation
```
/usr/share/man/man6/kuiper-ranger.6
```

### Desktop Integration
```
/usr/share/applications/kuiper-ranger.desktop
```

### Icons
```
/usr/share/icons/hicolor/16x16/apps/kuiper-ranger.png
/usr/share/icons/hicolor/32x32/apps/kuiper-ranger.png
/usr/share/icons/hicolor/48x48/apps/kuiper-ranger.png
/usr/share/icons/hicolor/64x64/apps/kuiper-ranger.png
/usr/share/icons/hicolor/128x128/apps/kuiper-ranger.png
```

## Package Information

| Field | Value |
|-------|-------|
| **Package Name** | kuiper-ranger |
| **Version** | 2.6-1 |
| **Architecture** | any (amd64, i386, ARM, ARM64, etc.) |
| **Section** | games |
| **Priority** | optional |
| **Maintainer** | Kuiper Ranger Maintainers |
| **License:** | GPL-2.0+ |
| **Size:** | <200 kB (varies) |
| **Build-Depends** | debhelper-compat (=13), g++, libsdl2-dev |
| **Runtime-Depends** | libsdl2-2.0-0, libc6 |
| **Standards-Version** | 4.6.0 |

## Uninstallation and Other Package Operations

To remove the package:

```bash
sudo apt remove kuiper-ranger
```

To also remove configuration/high scores:

```bash
sudo apt purge kuiper-ranger
```

To check what was installed:
```bash
dpkg -L kuiper-ranger
```

To reinstall the package:

```bash
sudo apt install --reinstall kuiper-ranger_2.6-1_amd64.deb
```

To fix broken installation:
```bash
sudo apt --fix-broken install
```

To view the high scores file:
```bash
cat /var/games/kuiper-ranger/scores
# Or user-specific:
cat ~/.local/share/Fishlet/kuiper-ranger/scores
```

## Desktop Integration

### Desktop Entry

The package includes a `.desktop` file that provides:

- Application name and description
- Icon association
- Category classification (Game)
- Proper launcher integration

### Desktop Environments

The package works with:
- GNOME / GNOME Shell
- KDE / Plasma
- XFCE
- LXDE
- Budgie
- Cinnamon
- MATE
- Other standard XDG-compliant environments

## Troubleshooting

### Build Issues

**Error: Command not found**
```bash
sudo apt-get install debhelper-compat devscripts build-essential
```

**Error: SDL2 development files not found**
```bash
sudo apt-get install libsdl2-dev
```

**Error: Permission denied**
```bash
chmod +x debian/postinst debian/rules
```

### Installation Issues

**Error: Dependency problems**
```bash
sudo apt --fix-broken install
```

**High score file not accessible**
```bash
# Check permissions
ls -la /var/games/kuiper-ranger/

# Fix if needed
sudo chmod 666 /var/games/kuiper-ranger/scores
```

**Desktop entry not appearing**
```bash
update-desktop-database
```

**Icons not showing**
```bash
gtk-update-icon-cache
```

### Runtime Issues

For detailed help: `man kuiper-ranger` (after installation)

**Command not found after installation**
```bash
# Ensure /usr/games is in PATH
echo $PATH

# Check if found in PATH
which kuiper-ranger

# Try full path
/usr/games/kuiper-ranger

# Check dependencies
ldd /usr/games/kuiper-ranger
```

## Distribution

### For Repository Maintainers

The package is ready for distribution to:
- Personal Package Archives (PPA)
- Debian repositories
- Linux distribution packaging

Key files for distribution:
- `kuiper-ranger_2.6-1.tar.xz` - Source code
- `kuiper-ranger_2.6-1.dsc` - Source package info
- `kuiper-ranger_2.6-1_amd64.deb` - Binary package

### For End Users

Users can install via:

1. **Standard package manager** (if in official repos)
   ```bash
   sudo apt install kuiper-ranger
   ```

2. **Downloaded .deb file**
   ```bash
   sudo apt install ./kuiper-ranger_2.6-1_amd64.deb
   ```

3. **From your PPA**
   ```bash
   sudo add-apt-repository ppa:your-name/kuiper-ranger
   sudo apt update
   sudo apt install kuiper-ranger
   ```

## Standards Compliance

The package follows:
- **Debian Policy Manual** (v4.6.0)
- **Debian Free Software Guidelines** (DFSG)
- **FreeDesktop.org** specifications
- **LSB** (Linux Standard Base) conventions

## Additional Notes

- The package is architecture-independent (builds for all architectures)
- The game binary runs as a regular user with no setuid or sudo required
- Follows Debian security best practices
- Compatible with modern Debian/Ubuntu systems
- Easy to uninstall without leaving artifacts

## Support Files

Additional documentation:
- `README.md` - Game user information
- `kuiper-ranger.man` - Game manual page
- `build-deb.sh` - Automated build script

## Next Steps

1. Install build dependencies
2. Run `./build-deb.sh` to build the package
3. Test installation: `sudo dpkg -i ../kuiper-ranger_2.6-1_*.deb`
4. Launch from menu or command line: `kuiper-ranger`
5. Share the `.deb` file with others for easy installation!
