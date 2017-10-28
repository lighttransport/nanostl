all:
	clang++ -Weverything -Werror -Wno-c++11-long-long -fsanitize=address -g -O0 -DNANOSTL_DEBUG test.cc
