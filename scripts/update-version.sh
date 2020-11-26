#!/bin/bash

#
# SCRIPT  update-version.sh
# AUTHOR  Gautier MIQUET <xisabla.dev@gmail.com>
# DATE:   2020-11-26
# REV:    1.0.D (Valid are A, B, D, T and P)
#               (For Alpha, Beta, Dev, Test and Production)
#
# PLATFORM: GNU/Linux
#
# PURPOSE:  Read version from include/version.h and update version in given files
#
# USAGE:    ./update-version.sh <file> [file] ... [file]
#

# version.h file
VERSION_HEADER_FILE="./include/version.h"

# Get the given version type (PATCH, MINOR, MAJOR) value
function get_version() {
  grep "#define ACS_VERSION_$1"  "$VERSION_HEADER_FILE" | cut -d ' ' -f3

  return 0
}

# Apply all update patterns on the given files with the given version values
function update_file() {
    sed -i "s/# AtilaCalculatorSoftware v[ 0-9\.]*/# AtilaCalculatorSoftware v$2.$3.$4/g" "$1" \
      && sed -i "s/PROJECT_NUMBER[ ]*=[ 0-9\.]*/PROJECT_NUMBER         = $2.$3.$4/g" "$1" \
      && echo "Updated $1" \
      && return 0

    return 1
}

# Get versions
major=$(get_version MAJOR)
minor=$(get_version MINOR)
patch=$(get_version PATCH)

# Apply changes on given files
for file in "$@"
do
  update_file "$file" "$major" "$minor" "$patch" || exit 1
done

exit 0