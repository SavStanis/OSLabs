#pragma once
#include<vector>
#include<map>


#define PAGE_SIZE 4096
#define MIN_ALLOC_SIZE 16

struct BlocksDescriber {
	void* firstFreeBlock;
	int classSize;
	int freeBlocksAmount;
	std::map<void*, bool> blocksMap;
};

struct PageDescriber {
	void* page;
	//	0 - free, 1 - divided, 3 - multiple 
	int state;
	BlocksDescriber blocksDescriber;
};

class Allocator {
public:
	Allocator(unsigned int size);
	//~Allocator();

	void* mem_alloc(unsigned int size);
	void mem_free(void* addr);
	//void mem_realloc(void* addr, unsigned int size);
	void mem_dump();
private:
	void* memory;

	int pagesAmount;
	int freePagesAmount;
	std::vector<void*> freePages;
	std::map<void*, PageDescriber> pageDescribersMap;
	void* allocBlock(int size);
	void* allocMultiple(int size);

};