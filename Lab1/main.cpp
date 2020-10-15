#include <iostream>
#include "Allocator.h"

int main() {
	Allocator alloc;

	char* tmp = (char*) alloc.mem_alloc(10);

	std::cout << alloc.mem_alloc(10) << std::endl;
	std::cout << alloc.mem_alloc(21) << std::endl;
	std::cout << alloc.mem_alloc(10) << std::endl;

	char* tmp1 = (char*)alloc.mem_realloc(tmp, 15);

	alloc.mem_free(tmp1);


	system("pause");
}