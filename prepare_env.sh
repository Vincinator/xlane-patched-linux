#!/bin/bash

# Exit on failure
set -e

# create output folder if it does not exist
mkdir -p asgardbin/

if [ ! -d "net/asgardcore" ]; then
    # get latest asgardmodule sources
    git clone --single-branch --branch dev git@github.com:Distributed-Systems-Programming-Group/asgardgit net/asgard_tmp
    shopt -s dotglob nullglob
    mv net/asgard_tmp/* net/aasgard
    rm -R net/asgard_tmp
else
   echo "Nothing happened - asgardfolder already present. If you want to re-initialize aasgardplease delete net/asasgardlder"
fi
