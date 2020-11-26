#!/bin/bash

#
# SCRIPT  fix-new-line.sh
# AUTHOR  Gautier MIQUET <xisabla.dev@gmail.com>
# DATE:   2020-11-25
# REV:    1.0.D (Valid are A, B, D, T and P)
#               (For Alpha, Beta, Dev, Test and Production)
#
# PLATFORM: GNU/Linux
#
# PURPOSE:  Check if the given files ends with a new line
#   Append a new line if it not the case
#
# USAGE:    ./fix-new-line.sh <file>
#           ./fix-new-line.sh <file1> <file2> ... <file_n>
#

# Check if the given file ends with a new line, if not append one
function fix_new_line() {
	if [ ! -f "$1" ]; then
		echo "File \"$1\" not found"
		return 1
	fi

	if [[ -s "$1" && -z "$(tail -c 1 "$1")" ]]
	then
		echo "$1: already ends with a new line"
	else
		echo "" >> "$1"
		echo "$1: new line add at the end"
	fi
	
	return 0
}

# Check for all given files
for file in "$@"
do
  fix_new_line "$file" || exit 1
done

exit 0
