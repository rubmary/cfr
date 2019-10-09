#!/bin/bash

function solve_game() {
	echo "Running cfr"
	./targets/cfr $@
	echo "Running gebr"
	./targets/gebr $@
}

function make_folders() {
	echo "Making dirs"
	mkdir -p graphics/$1
	mkdir -p regret/$2
	mkdir -p results/$2
}

GAME=$1
FOLDER=$1
shift 1
FIRST=true
for var in "$@"
do
	if "$FIRST" = true; then
		FOLDER="${FOLDER}/${var}"
	else
		FOLDER="${FOLDER}_${var}"
	fi
	FIRST=false
done
echo "GAME: ${GAME}"
echo "FOLDER: ${FOLDER}"
make_folders $GAME $FOLDER
echo "Compiling programs..."
make
solve_game $GAME $@
echo "Creating graphics"
python src/graph.py $FOLDER
