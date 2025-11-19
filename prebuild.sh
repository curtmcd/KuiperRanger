#!/bin/sh

# Prep for a clean build

make clean
make ID
make depend

echo "*** Ready to run make ***"
