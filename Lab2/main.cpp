#include <iostream>
#include "PageAllocator.h"

int main() {
	PageAllocator allocator(20 * 1024);
	allocator.mem_dump();
	void* test1 = allocator.mem_alloc(300);
	allocator.mem_dump();
	void* test2 = allocator.mem_alloc(1600);
	allocator.mem_dump();
	allocator.mem_free();
	allocator.mem_dump();
	system("pause");
}