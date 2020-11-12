#include <iostream>
#include "Allocator.h"

using namespace std;

int main() {
	
	Allocator allocator(15000);

	void* test1 = allocator.mem_alloc(300);
	void* test2 = allocator.mem_alloc(3000);
	allocator.mem_free(test1);

	allocator.mem_dump();


	system("pause");
}