#!/bin/bash

function solve_game() {
	echo "Making dirs"
	mkdir -p graphics/$1
	mkdir -p regret/$1
	mkdir -p results/$1

	echo "Running cfr"
	./cfr $1
	echo "Running gebr"
	./gebr $1
	echo "Creating graphics"
	python src/graph.py $1
}

echo "Compiling programs"
make cfr
make gebr
solve_game "KuhnPoker"
echo "Cleaning"
make clean