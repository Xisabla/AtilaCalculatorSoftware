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
