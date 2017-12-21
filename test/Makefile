all:
	clang++ -o test -I../ -std=c++11 -Weverything -Werror -Wno-zero-as-null-pointer-constant -Wno-c++98-compat-pedantic -Wno-c++11-long-long -fsanitize=address -g -O0 -DNANOSTL_DEBUG test.cc
