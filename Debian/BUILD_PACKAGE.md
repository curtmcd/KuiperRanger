# Debian Packaging

This file describes the process for creating a Debian Package for Kuiper Ranger.

## Prerequisites

	sudo apt update
	sudo apt install devscripts dpkg-dev dh-make pbuilder

## Create the upstream tarball

	cd KuiperRanger
	ls -ld .git
	git archive --format=tar.gz --prefix=kuiper-ranger-2.6/ -o Debian/kuiper-ranger_2.6.orig.tar.gz HEAD

## Extract the tarball to kuiper-ranger-2.6/ in the Debian area

	cd Debian
	tar xf kuiper-ranger_2.6.orig.tar.gz
	cd kuiper-ranger-2.6

## Create the debian/* template files

	dh_make --single --copyright gpl3 --email gamer@fishlet.com 

Then edit the files under debian/. Retain the files:
* changelog
* control
* copyright
* kuiper-ranger.desktop
* kuiper-ranger-docs.docs (contents just README.md)
* kuiper-ranger.manpages (contents just kuiper-ranger.man)
* postinst
* postrm
* rules
* source/format
* upstream/metadata

Refer to examples in ./debian_example/. It should be possible to pretty much use this example directory as debian/ with the exception of the changelog.

	diff -r debian ../debian_example

To submit an initial release to the repository, open an ITP issue with Debian. Put the bug number of the ITP in the changelog.

It would be a good idea to ARCHIVE THE FULLY BUILT Debian DIRECTORY for anything submitted to Debian.

## Do the build

The -us option is passed to dpkg-buildpackage and means not to sign the source package. The -uc option means not to sign the .changes file.

	cd kuiper-ranger-2.6
	debuild -us -uc

The output package will be placed in the parent directory (`../kuiper-ranger_2.6-1_*.deb`).

# Compiling for ARM and RISC-V

## Prerequisites

	sudo apt update
	sudo apt install qemu-user-static binfmt-support
	sudo apt install debian-archive-keyring debian-ports-archive-keyring

## Configure

Create /root/.pbuilderrc

	cp .pbuilderrc /tmp
	sudo -s
	cd /root
	mv /tmp/.pbuilderrc /root/

## Download ARM debian release and build KR package

`ARCH=arm64` tells .pbuilderrc to target 64-bit ARM

`DIST=bookworm` tells .pbuilderrc to use the current stable release

	sudo ARCH=arm64 DIST=bookworm pbuilder --create
	cd kuiper-ranger-2.6/debian
	sudo ARCH=arm64 DIST=bookworm pdebuild

Outputs go in /var/cache/pbuilder/result/

## Download RISC-V debian release and build KR package

	sudo ARCH=riscv64 DIST=sid pbuilder --create
	sudo ARCH=riscv64 DIST=sid pdebuild

Outputs go in /var/cache/pbuilder/result/

# Creating a Debian repository on local machine

This will create a debian repo that is just a directory on the local machine. The repo can be added to APT the same as a remote repo can be added.

## Prerequisites

	sudo apt update
	sudo apt install apt-utils

## Create skeleton of repository

	cd $HOME/Linux/Debian_Repo
	mkdir -p conf
	mkdir -p pool/main
	mkdir -p dists/stable/main/binary-amd64
	mkdir -p dists/stable/main/binary-arm64
	mkdir -p dists/stable/main/binary-riscv64
	cp ~/debian-build/*.deb pool/main/
	# Also copy the arm64/riscv64 debs from /var/cache/pbuilder/result/
	cp /var/cache/pbuilder/result/*.deb pool/main/

## Scan packages for each supported architecture

amd64

	dpkg-scanpackages --arch amd64 pool/main > dists/stable/main/binary-amd64/Packages
	cat dists/stable/main/binary-amd64/Packages | gzip -9 > dists/stable/main/binary-amd64/Packages.gz

arm64

	dpkg-scanpackages --arch arm64 pool/main > dists/stable/main/binary-arm64/Packages
	cat dists/stable/main/binary-arm64/Packages | gzip -9 > dists/stable/main/binary-arm64/Packages.gz

riscv64

	dpkg-scanpackages --arch riscv64 pool/main > dists/stable/main/binary-riscv64/Packages
	cat dists/stable/main/binary-riscv64/Packages | gzip -9 > dists/stable/main/binary-riscv64/Packages.gz

## Create release.conf

	APT::FTPArchive::Release::Origin "My Local Repo";
	APT::FTPArchive::Release::Label "Kuiper Ranger Repo";
	APT::FTPArchive::Release::Suite "stable";
	APT::FTPArchive::Release::Codename "stable";
	APT::FTPArchive::Release::Architectures "amd64 arm64 riscv64";
	APT::FTPArchive::Release::Components "main";
	APT::FTPArchive::Release::Description "Local test repository for Kuiper Ranger";

## Create dists/stable/Release:

	apt-ftparchive release -c release.conf dists/stable > dists/stable/Release 

Inputs:

	Origin: My Local Repo
	Label: Kuiper Ranger Repo
	Suite: stable
	Codename: stable
	Architectures: amd64 arm64 riscv64
	Components: main
	Description: Local test repository for Kuiper Ranger

Add local repository to sources list. Using [trusted=yes] instead of signing with GPG.

	echo "deb [trusted=yes] file:$HOME/Linux/Debian_Repo stable main" | sudo tee /etc/apt/sources.list.d/my-local-repo.list

Now the package can be installed using apt:

	sudo apt update
	sudo apt install kuiper-ranger

# Creating a Debian PPA repository accessible from the Internet

TODO!
