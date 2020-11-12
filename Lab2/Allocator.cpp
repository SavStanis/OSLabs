#include "Allocator.h"
#include <iostream>
#include <string>

Allocator::Allocator(unsigned int size) {
	freePagesAmount = pagesAmount = ceil((double) size / PAGE_SIZE);
	size = pagesAmount * PAGE_SIZE;
	memory = malloc(size);

	for (int i = 0; i < pagesAmount; i++) {
		int shift = i * PAGE_SIZE;
		void* newPage = (void*)((char*) memory + shift);

		PageDescriber pageDescriber = PageDescriber();
		pageDescriber.state = 0;
		pageDescriber.page = newPage;

		BlocksDescriber blockDescriber = BlocksDescriber();
		blockDescriber.firstFreeBlock = newPage;
		blockDescriber.freeBlocksAmount = 0;
		blockDescriber.classSize = 0;
		pageDescriber.blocksDescriber = blockDescriber;

		pageDescribersMap[newPage] = pageDescriber;
		freePages.push_back(newPage);
	}
}

void* Allocator::mem_alloc(unsigned int size) {
	if (size < PAGE_SIZE / 2) {
		return allocBlock(size);
	}

	return allocMultiple(size);
}

void* Allocator::allocBlock(int size) {
	int nextClassSize = (int)pow(2, ceil(log(size) / log(2)));
	int classSize = (MIN_ALLOC_SIZE > nextClassSize) ? MIN_ALLOC_SIZE : nextClassSize;

	void* allocatedBlock = nullptr;

	for (int i = 0; i < pagesAmount; i++) {
		int shift = i * PAGE_SIZE;
		void* page = (void*)((char*)memory + shift);

		PageDescriber pageDescriber = pageDescribersMap[page];

		if (pageDescriber.state != 1) {
			continue;
		}

		BlocksDescriber blocksDescriber = pageDescriber.blocksDescriber;

		if (classSize == blocksDescriber.classSize && blocksDescriber.freeBlocksAmount > 0) {
			allocatedBlock = blocksDescriber.firstFreeBlock;
			blocksDescriber.firstFreeBlock = (void*) ((char*) blocksDescriber.firstFreeBlock + blocksDescriber.classSize);
			blocksDescriber.freeBlocksAmount--;

			pageDescriber.blocksDescriber = blocksDescriber;
			pageDescribersMap[page] = pageDescriber;
			break;
		}
	}

	if (allocatedBlock == nullptr) {

		if (freePagesAmount == 0) {
			std::cout << "Not enough free pages" << std::endl;
			return nullptr;
		}
		void* page = freePages.back();
		freePages.pop_back();
		freePagesAmount--;

		PageDescriber pageDescriber = pageDescribersMap[page];
		BlocksDescriber blockDescriber = pageDescriber.blocksDescriber;

		pageDescriber.state = 1;

		blockDescriber.classSize = classSize;
		blockDescriber.freeBlocksAmount = PAGE_SIZE / classSize;
		allocatedBlock = blockDescriber.firstFreeBlock;
		blockDescriber.firstFreeBlock = (void*)((char*) blockDescriber.firstFreeBlock + blockDescriber.classSize);
		blockDescriber.freeBlocksAmount--;

		pageDescriber.blocksDescriber = blockDescriber;
		pageDescribersMap[page] = pageDescriber;
	}

	return allocatedBlock;
}

void* Allocator::allocMultiple(int size) {
	unsigned int neededPagesAmount = ceil((double)size / PAGE_SIZE);
	if (neededPagesAmount > freePagesAmount) {
		std::cout << "There is not enough pages to allocate this size of memory" << std::endl;
		return nullptr;
	}

	std::vector<void*> allocatedPages;

	for (int i = 0; i < pagesAmount; i++) {
		int shift = i * PAGE_SIZE;
		void* page = (void*)((char*)memory + shift);
		
		PageDescriber pageDescriber = pageDescribersMap[page];
		if (pageDescriber.state == 0) {
			allocatedPages.push_back(page);

			if (allocatedPages.size() == neededPagesAmount) {
				break;
			}
		}
		else {
			allocatedPages.clear();
		}
	}

	if (allocatedPages.size() != neededPagesAmount) {
		std::cout << "There is not enough pages in row to allocate this size of memory" << std::endl;
		return nullptr;
	}

	for (void* page : allocatedPages) {
		PageDescriber pageDescriber = pageDescribersMap[page];
		pageDescriber.state = 2;
		pageDescriber.blocksDescriber.classSize = PAGE_SIZE * neededPagesAmount;

		pageDescribersMap[page] = pageDescriber;
	}

	return allocatedPages.front();
} 

void Allocator::mem_dump() {
	std::cout << "-----------------------GENERAL--INFORMATION--------------------------" << std::endl;

	std::cout << "Number of pages: \t" << pagesAmount << std::endl;
	std::cout << "Size of each page: \t" << PAGE_SIZE << std::endl;
	std::cout << "Number of free pages: \t" << freePagesAmount << std::endl;

	std::cout << "------------------------pages--description--------------------------" << std::endl;

	for (int i = 0; i < pagesAmount; i++) {
		int shift = i * PAGE_SIZE;
		void* page = (void*)((char*)memory + shift);

		PageDescriber pageDescriber = pageDescribersMap[page];

		std::string state;
		std::string classSize = "";
		std::string freeSpace = "";

		if (pageDescriber.state == 0) {
			state = "Free";
		}
		else if (pageDescriber.state == 1) {
			state = "Divided";
		}
		else {
			state = "Multiple";
		}

		if (pageDescriber.state == 1) {
			classSize = "\n-----Size of each block in class:  " + std::to_string(pageDescriber.blocksDescriber.classSize);
			freeSpace = "\n-----Number of free blocks:  " +  std::to_string(pageDescriber.blocksDescriber.freeBlocksAmount);
		}

		std::cout << i + 1 
			<< ". Address:\t" << page 
			<< "\tType:\t" << state
			<< freeSpace 
			<< classSize 
			<< std::endl;
	}
}