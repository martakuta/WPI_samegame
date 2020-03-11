#!/bin/bash

gcc -o gen generatorka.c
gcc -o kacper kacper.c
gcc -o marta marta.c

for((i=1;i<=10000;i++))
do
	echo $i
	echo $i | ./gen > test
	
	./kacper < test > kacper.out
	./marta < test > marta.out
	
	diff kacper.out marta.out || break
done
