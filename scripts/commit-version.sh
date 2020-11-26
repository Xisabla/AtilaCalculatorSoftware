#!/bin/bash

#
# SCRIPT  commit-version.sh
# AUTHOR  Gautier MIQUET <xisabla.dev@gmail.com>
# DATE:   2020-11-26
# REV:    1.0.D (Valid are A, B, D, T and P)
#               (For Alpha, Beta, Dev, Test and Production)
#
# PLATFORM: GNU/Linux
#
# PURPOSE:  Prepare a git commit with a version change
#     The main idea of this script is to be ran after lot of code changes that
#     should cause a version change
#     The script will reset the lower levels version to 0 and increment the target
#     level
#
# USAGE:    ./commit-version.sh
#           ./commit-version.sh --major
#           ./commit-version.sh --minor
#           ./commit-version.sh --patch
#
# NOTE:     Will make a patch change by default if no argument given
#

VERSION_HEADER_FILE="./include/version.h"

function reset_version() {
    sed -i "s/#define ACS_VERSION_$1 [0-9]*/#define ACS_VERSION_$1 0/g" "$VERSION_HEADER_FILE" \
      && return 0

    return 1
}

function set_version() {
  sed -i "s/#define ACS_VERSION_$1 [0-9]*/#define ACS_VERSION_$1 $2/g" "$VERSION_HEADER_FILE" \
      && return 0

    return 1
}

function get_version() {
  grep "#define ACS_VERSION_$1"  "$VERSION_HEADER_FILE" | cut -d ' ' -f3

  return 0
}

function increment_version() {
  current=$(get_version "$1")

  set_version "$1" $((current+1)) \
    && return 0

  return 1
}

function print_version() {
    echo "$(get_version MAJOR).$(get_version MINOR).$(get_version PATCH)"
}

old_version=$(print_version)

case $1 in
--major)
  increment_version MAJOR
  reset_version MINOR
  reset_version PATCH
  ;;
--minor)
  increment_version MINOR
  reset_version PATCH
  ;;
--patch | *)
  increment_version PATCH
  ;;
esac

message="Change version $old_version --> $(print_version)"

git add . \
  && git commit -S -m "$message" \
  && exit 0

exit 1
