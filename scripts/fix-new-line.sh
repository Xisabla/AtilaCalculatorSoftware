#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage: $0 <file> [file2] ..."
	exit 1
fi

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

for file in "$@"
do
  fix_new_line "$file"
done
