all: main.o nanoalgorithm.o
	clang++ -o test $< -pthread

main.o: main.cc
	clang++ -c -o main.o -I../../include -std=c++17 -nostdinc++ -DNANOSTL_PSTL main.cc

# Enable stdlib required at the moment
nanoalgorithm.o: ../../src/nanoalgorithm.cc
	clang++ -c -o nanoalgorithm.o -I../../include -std=c++17 -DNANOSTL_PSTL ../../src/nanoalgorithm.cc


.PHONY: clean

clean:
	rm -rf nanoalgorithm.o main.o test
