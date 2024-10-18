#!/bin/bash

dir=$(dirname "$(readlink -f "$0")")
cd "$dir" || exit 3

for file in *.c; do
    if ! gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wfloat-equal -Wfloat-conversion -Wvla -g -c "$file"; then
        exit 2
    fi
done

if ! gcc ./*.o -o "app.exe" -lm; then
    exit 2
fi
exit 0
