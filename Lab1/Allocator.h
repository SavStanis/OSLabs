#pragma once
class Allocator
{
public:
	Allocator();
	void *mem_alloc(int size);
	void *mem_realloc(void* addr, int size);
	void mem_free(void *addr);
private:
	char* memory;
	char* freeblock;
	const int MAX_SIZE = 2048;
	const int HEADER_SIZE = 9;
};