#! /usr/bin/bash

tmpa=$(mktemp)
tmpb=$(mktemp)

sed 's/|/ /' $1 | awk '{print $2"\011"$4}' > "$tmpa"
sed 's/|/ /' $2 | awk '{print $4}' > "$tmpb"

paste -d$'\t' "$tmpa" "$tmpb"

rm ${tmpa}
rm ${tmpb}
