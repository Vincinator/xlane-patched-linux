#!/bin/bash

# Exit on failure
set -e

# create output folder if it does not exist
mkdir -p asgard-bin/

if [ ! -d "net/asgard/core" ]; then
    # get latest asgard module sources
    git clone --single-branch --branch dev git@github.com:Distributed-Systems-Programming-Group/sassy.git net/asgard_tmp
    shopt -s dotglob nullglob
    mv net/asgard_tmp/* net/asgard/
else
    cd net/asgard && git pull && cd ../.. && pwd
fi

# clean up
make clean

# compile asgard module
make M=net/asgard/ modules

# copy binary
cp net/asgard/core/asgard.ko asgard-bin/

# clean before exit
sudo rm -R net/asgard

# get Kconfig back
git reset --hard


# ... to be continued ...

exit 0

