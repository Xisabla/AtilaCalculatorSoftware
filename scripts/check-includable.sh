#!/bin/bash

#
# SCRIPT  check-includable.sh
# AUTHOR  Gautier MIQUET <xisabla.dev@gmail.com>
# DATE:   2020-11-25
# REV:    1.0.D (Valid are A, B, D, T and P)
#               (For Alpha, Beta, Dev, Test and Production)
#
# PLATFORM: GNU/Linux
#
# PURPOSE:  Check if the given files exists in the usual shared libraries include dirs
#
# USAGE:    ./check-includable.sh <file>
#           ./check-includable.sh <file1> <file2> ... <file_n>
#

# Include folders
INCLUDE_DIRS=(
  /usr/include/
  /usr/local/include/
)

# Add include folders sub directories
for dir in "${INCLUDE_DIRS[@]}"; do
  for sub in $(ls -1p "$dir/" | grep '/$'); do
    INCLUDE_DIRS+=("$dir$sub")
  done
done

# Check if the given file is an include folder
function check_includable() {
  for dir in "${INCLUDE_DIRS[@]}"; do
    if [ -f "$dir$1" ]; then
      echo "- $dir$1 found"

      return 0
    fi


  done

  echo "$1 not found"

  return 1
}

# Check for each given files
for lib in "$@"; do
  check_includable "$lib" || exit 1
done

exit 0
