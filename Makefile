all:
	clang++ -fsanitize=address -g -O0 -DNANOSTL_DEBUG test.cc
