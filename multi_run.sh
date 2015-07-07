#!/bin/bash
CONVERTER="./json2html.exe"
if [[ $1 == "-p" || $1 == "-m" ]]; then
	for json in data/*.json; do
		out="${json%.*}.html"
		$CONVERTER -i $json -o $out $1
	done
else
	echo "$1 is not a valid flag. Quitting..."
	exit
fi
	