#!/bin/bash

if [ ! -d debian_example ]; then
    echo >&2 "Must run in the Debian directory!"
    exit 1
fi

echo "===== WARNING ====="
echo "This removes the ENTIRE build, including the .deb package,"
echo "the .orig.tar.gz file, the edited debian/ files, etc!"

echo -n "Proceed? [yes/no] "

read ans
if [ "$ans" != yes ]; then
    echo "Aborted"
    exit 1
fi

# The only thing it does not remove is the final .deb.

for i in \
    kuiper-ranger-[0-9]* \
    kuiper-ranger_[0-9]* \
    *.ddeb *.dsc *.build *.buildinfo *.changes \
    *.deb *.tar.xz *.tar.gz
do
    echo "Removing: $i"
    rm -rf "$i"
done
