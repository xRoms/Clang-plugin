#!/bin/bash
echo "first"
echo "without counters:"
clang++ ./timetest/1.cpp 
time ./a.out
echo "with counters:"
./start.sh ./timetest/1.cpp ./timetest/result1.txt
time ./a.out
echo "second"
echo "without counters:"
clang++ ./timetest/2.cpp 
time ./a.out
echo "with counters:"
./start.sh ./timetest/2.cpp ./timetest/result2.txt
time ./a.out
echo "third"
echo "without counters:"
clang++ ./timetest/3.cpp 
time ./a.out
echo "with counters:"
./start.sh ./timetest/3.cpp ./timetest/result3.txt
time ./a.out
echo "fourth"
echo "without counters:"
clang++ ./timetest/4.cpp 
time ./a.out
echo "with counters:"
./start.sh ./timetest/4.cpp ./timetest/result4.txt
time ./a.out