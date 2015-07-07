#!/bin/bash
CONVERTER="./json2html.exe"
for json in data/*.json; do
	out="${json%.*}.html"
	$CONVERTER -i $json -o $out
done
