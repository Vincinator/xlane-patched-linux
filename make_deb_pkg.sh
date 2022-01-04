#!/bin/bash

# Exit on failure
set -e

# Store short git commit hash. Will be used as Asgard kernel version name
ASGARD_KERNEL_VERSION="$(git rev-parse --short HEAD)"

# create output folder if it does not exist
mkdir -p asgard-bin/

# Copy Predefined asgard kernel config
# cp asgard-kernel.config .config

# Compile Kernel and create .deb packages
make bindeb-pkg KDEB_PKGVERSION=$(make kernelversion) ARCH=x86 -j 54

# # Copy install script to output folder
# cp install_asgard_kernel.sh asgard-bin/
# cp install_on_remotes.sh asgard-bin/

# Copy .deb packages to output folder
mv ../*.deb asgard-bin/

exit 0

