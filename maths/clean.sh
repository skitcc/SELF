#!/bin/bash

dir=$(dirname "$(readlink -f "$0")")
cd "$dir" || exit 3

rm -f ./*.exe ./*.o ./*.gcov ./*.gcda ./*.gcno ./*.c ./*.h

while IFS= read -r to_delete; do
    rm -rf "$to_delete"
done < <(find . -type f -name "*.bin")

while IFS= read -r to_delete; do
    rm -rf "$to_delete"
done < <(find . -type f -name "*.tmp")
