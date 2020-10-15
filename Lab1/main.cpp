#include <iostream>
#include "Allocator.h"

int main() {
	Allocator alloc;

	std::cout << "\n";

	char* tmp = (char*) alloc.mem_alloc(10);

	alloc.mem_alloc(10);
	char* tmp1 = (char*) alloc.mem_alloc(21);
	alloc.mem_alloc(10);

	std::cout << std::endl;

	char* tmp2 = (char*)alloc.mem_realloc(tmp, 15);

	std::cout << std::endl;

	alloc.mem_free(tmp);
	alloc.mem_free(tmp1);


	system("pause");
}