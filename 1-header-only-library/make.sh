#!/bin/bash
set -e
set -u
if [[ "$OSTYPE" == "darwin"* ]]; then
    clang++ -o main main.cpp
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    exit 1
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]]; then
    exit 1
else
    exit 1
fi
