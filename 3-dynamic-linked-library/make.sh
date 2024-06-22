#!/bin/bash
set -e
set -u
if [[ "$OSTYPE" == "darwin"* ]]; then
    clang++ -shared -o libsomelibrary.so some-library.cpp -fPIC
    clang++ -o main main.cpp -L. -lsomelibrary
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    exit 1
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
    exit 1
else
    exit 1
fi
