#!/bin/bash

#
# SCRIPT  print-version.sh
# AUTHOR  Gautier MIQUET <xisabla.dev@gmail.com>
# DATE:   2020-12-02
# REV:    1.0.D (Valid are A, B, D, T and P)
#               (For Alpha, Beta, Dev, Test and Production)
#
# PLATFORM: GNU/Linux
#
# PURPOSE:  Print current project version. Reading it from include/version.h
#
# USAGE:    ./print-version.sh
#

# version.h file
VERSION_HEADER_FILE="./include/version.h"
# Get the given version type (PATCH, MINOR, MAJOR) value
function get_version() {
  grep "#define ACS_VERSION_$1"  "$VERSION_HEADER_FILE" | cut -d ' ' -f3

  return 0
}

# Get the full version
function print_version() {
    echo "$(get_version MAJOR).$(get_version MINOR).$(get_version PATCH)"
}

# Print the version and exit
print_version \
  && exit 0

exit 1
