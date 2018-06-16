#!/bin/bash
clang -c -Xclang -load -Xclang ./PrintFunctionNames.so -Xclang -add-plugin -Xclang print-fns test.cpp
