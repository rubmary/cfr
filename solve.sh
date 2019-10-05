#!/bin/bash

function solve_game() {
	echo "Making dirs"
	mkdir -p graphics/
	mkdir -p regret/$2
	mkdir -p results/$2

	echo "Running cfr"
	./targets/cfr $@
	echo "Running gebr"
	./targets/gebr $@
	echo "Creating graphics"
	python src/graph.py $2
}

GAME=$1
FOLDER=$1
shift 1
for var in "$@"
do
	FOLDER="${FOLDER}_${var}"
done
echo "FOLDER: ${FOLDER}"
echo "Compiling programs..."
make
solve_game $GAME $FOLDER $@
