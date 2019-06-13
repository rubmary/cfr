#!/bin/bash

echo "Making dirs"
mkdir -p graphics/$1
mkdir -p regret/$1
mkdir -p results/$1

echo "Compiling programs"
make cfr
make gebr


echo "Running cfr"
./cfr $1
echo "Running gebr"
./gebr $1
echo "Creating graphics"
pytho src/graphs.py $1

echo "Cleaning"
make clean