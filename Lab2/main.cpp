#include <iostream>
#include "PageAllocator.h"

int main() {
	
	
	PageAllocator allocator(15000);
	allocator.mem_dump();
	void* test1 = allocator.mem_alloc(300);
	allocator.mem_dump();
	void* test2 = allocator.mem_alloc(1600);
	allocator.mem_dump();
	void* test3 = allocator.mem_alloc(8000);
	allocator.mem_dump();
	void* test4 = allocator.mem_alloc(1600);
	allocator.mem_dump(); 
	/*
	PageAllocator allocator(15000);
	void* test1 = allocator.mem_alloc(300);
	allocator.mem_free(test1);
	void* test2 = allocator.mem_alloc(3000);
	allocator.mem_free(test1);
	allocator.mem_dump(); 
	*/
	/*
	PageAllocator allocator(15000);
	void* test1 = allocator.mem_alloc(300);
	allocator.mem_dump();
	allocator.mem_realloc(test1, 3000);
	allocator.mem_dump();
	*/

	system("pause");
}