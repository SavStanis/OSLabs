#include <iostream>
#include "Allocator.h"

using namespace std;

int main() {
	
	Allocator alloc = Allocator(16000);
	void* test1 = alloc.mem_alloc(100);
	void* test2 = alloc.mem_alloc(100);
	void* test3 = alloc.mem_alloc(100);
	void* test4 = alloc.mem_alloc(100);
	void* test5 = alloc.mem_alloc(100);
	void* test6 = alloc.mem_alloc(1000);
	void* test7 = alloc.mem_alloc(3000);
	void* test8 = alloc.mem_alloc(100);
	alloc.mem_free(test7);
	alloc.mem_dump();

	alloc.mem_alloc(100);

	system("pause");
}