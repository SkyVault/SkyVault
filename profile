#!/bin/bash

cd src/
valgrind --tool=callgrind ./bin/release/SkyVault

echo "kcachegrind to visualize, or callgrind callgrind.out.* | dot -Tsvg -o output.svg"
