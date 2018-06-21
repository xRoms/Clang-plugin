#!/bin/bash
INPUT=$1
OUTPUT=$2
if [[ -n "$INPUT" ]]; then
	if ! [[ -n "$OUTPUT" ]]; then
		clang++ -c -Xclang -load -Xclang ./PrintFunctionNames.so -Xclang -add-plugin -Xclang print-fns $INPUT
	else 
		clang++ -c -Xclang -load -Xclang ./PrintFunctionNames.so -Xclang -add-plugin -Xclang print-fns -Xclang -plugin-arg-print-fns -Xclang $OUTPUT $INPUT
	fi
	clang++ temp.cpp
else
	echo "Usage: start.sh <input file> <output (if needed)>"
fi
