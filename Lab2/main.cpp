#include <iostream>
#include "PageAllocator.h"

int main() {
	PageAllocator allocator(30000);
	allocator.mem_dump();
	void* test1 = allocator.mem_alloc(300);
	allocator.mem_dump();
	void* test2 = allocator.mem_alloc(1600);
	allocator.mem_dump();
	void* test3 = allocator.mem_alloc(8000);
	allocator.mem_dump();
	void* test4 = allocator.mem_alloc(1600);
	allocator.mem_dump();
	system("pause");
}