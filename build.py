#!/usr/bin/env python3

from subprocess import call
import argparse
import os
import multiprocessing
import configparser
import fileinput
import subprocess
from shutil import copyfile

from os import environ

try:
    import pexpect
    remote_support = True
except ImportError:
    remote_support = False
from subprocess import check_output

show_help = False

def prepare_kernel_config(template):
    copyfile(template, ".config")

def build_module():
    prepare_kernel_config("asgard-module.config")
    rc = subprocess.call(['./make_asgard_module.sh'])
    print(rc)

def build_kernel():
    prepare_kernel_config("asgard-kernel.config")
    rc = subprocess.call(['./make_deb_pkg.sh'])
    print(rc)

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('--build', choices=['module', 'kernel'],
                        help='Compile asgard on build server and print output')

    args = parser.parse_args()

    if args.build is not None:
        if args.build == 'module':
            build_module()
        if args.build == 'kernel':
            build_kernel()

    if show_help:
        parser.print_help()

if __name__ == '__main__':
    main()