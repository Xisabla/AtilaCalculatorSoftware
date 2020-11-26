#
# SCRIPT  prepared-documentation.sh
# AUTHOR  Gautier MIQUET <xisabla.dev@gmail.com>
# DATE:   2020-11-26
# REV:    1.0.D (Valid are A, B, D, T and P)
#               (For Alpha, Beta, Dev, Test and Production)
#
# PLATFORM: GNU/Linux
#
# PURPOSE:  Remove all files that do not take place  on documentation branch
#
# USAGE:    ./prepared-documentation.sh
#

# Elements to remove
REMOVE=(
  build
  cmake
  data
  include
  qt
  src
  .clang-format
  CMakeLists.txt
  Doxyfile
  Makefile
)

# Remove all unneeded files/directories
for elem in "${REMOVE[@]}"
do
  echo "rm -rf $elem"
done

exit 0
