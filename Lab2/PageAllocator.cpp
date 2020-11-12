#include"PageAllocator.h"
#include <iostream>
#include <string>
#include <algorithm>


PageAllocator::PageAllocator(unsigned int  size) {

	numberOfPages = ceil((double) size / PAGE_SIZE);
	size = numberOfPages * PAGE_SIZE;

	startPosition = malloc(size);
	
	for (int i = 0; i < numberOfPages; ++i) {
		void* page = (void*) ((char*) (startPosition) + (i * PAGE_SIZE));
		freePagesList.push_back(page);

		PageHeader header = PageHeader();
		header.state = StateOfPage::Free;

		headersMap[page] = header;
	}

	for (unsigned int i = MIN_ALLOC_SIZE; i <= PAGE_SIZE / 2; i *= 2) {
		// 2^x (x >= 4)
		freeClassPagesMap[i] = std::vector<void*>();
	}
}

void *PageAllocator::mem_alloc(unsigned int size) {
	if (size < PAGE_SIZE / 2) {

		// getting of the page class size 
		unsigned int nextClassSize = (unsigned int) pow (2, ceil(log(size) / log(2)));
		unsigned int classSize = (MIN_ALLOC_SIZE > nextClassSize) ? MIN_ALLOC_SIZE : nextClassSize;

		// getting of class page
		void* page;
		std::vector<void*> pages = freeClassPagesMap[classSize];
		
		if (pages.empty()) {
			page = nullptr;
		}
		else {
			page = pages.back();
		}

		if (page == nullptr || headersMap[page].freeBlocksNumber == 0) {
			page = divideFreePage(classSize);
		}

		return allocateBlock(page);
	}

	return allocateSeveralPages(size);
}

void *PageAllocator::allocateBlock(void *page) {
	PageHeader header = headersMap[page];

	void* addr = header.startPosition;
	void* next = ((BlockHeader*) addr)->nextBlock;

	header.startPosition = next;
	header.freeBlocksNumber--;

	if (header.freeBlocksNumber == 0) {
		std::vector<void*> pages = freeClassPagesMap[header.size];
		pages.erase(std::remove(pages.begin(), pages.end(), page), pages.end());
	}

	headersMap[page] = header;
	
	return addr;
}


void *PageAllocator::divideFreePage(unsigned int classSize) {

	if (freePagesList.empty()) {
		std::cout << "There is no free pages" << std::endl;
		return nullptr;
	}

	void* page = freePagesList.back();
	freePagesList.pop_back();

	PageHeader header = headersMap[page];

	header.size = classSize;
	header.state = StateOfPage::Divided;
	header.startPosition = page;
	header.freeBlocksNumber = PAGE_SIZE / classSize;
	headersMap[page] = header;

	for (int i = 0; i < header.freeBlocksNumber - 1; ++i) {
		void* block = (int *) ((char*) page + i * header.size);
		BlockHeader* blockHeader = (BlockHeader*) block;

		void* nextBlock = (int *)((char*) page + header.size * (i + 1));
		blockHeader->nextBlock = nextBlock;
	}

	freeClassPagesMap[classSize].push_back(page);
	return page;
}

void *PageAllocator::allocateSeveralPages(unsigned int size) {
	unsigned int neededPagesAmount = ceil((double) size / PAGE_SIZE);

	if (neededPagesAmount > freePagesList.size()) {
		std::cout << "There is not enough pages to allocate this size of memory" << std::endl;
		return nullptr;
	}

	std::vector<void*> pages;

	for (int i = 0; i < numberOfPages; ++i) {

		void* page = (void*) ((char*) (startPosition) + (i * PAGE_SIZE));
		PageHeader header = headersMap[page];

		if (header.state == StateOfPage::Free) {
			pages.push_back(page);

			if (pages.size() == neededPagesAmount) {
				break;
			}
		}
	}

	if (pages.size() != neededPagesAmount) {
		std::cout << "There is not enough pages to allocate this size of memory" << std::endl;
		return nullptr;
	}
	
	for (void* page : pages) {
		PageHeader header = headersMap[page];

		header.state = StateOfPage::Multiple;
		header.freeBlocksNumber = neededPagesAmount;
		header.size = neededPagesAmount * PAGE_SIZE;
		headersMap[page] = header;

		freePagesList.erase(std::remove(freePagesList.begin(), freePagesList.end(), page), freePagesList.end());
	}

	return pages.front();
}

void* PageAllocator::getPage(void* addr) {
	unsigned int pageNumber = ((char*) addr - (char*) startPosition) / PAGE_SIZE;
	return (char*) startPosition + pageNumber * PAGE_SIZE;
}

void PageAllocator::mem_free(void *addr) {
	void* page = getPage(addr);
	PageHeader header = headersMap[page];

	if (header.state == StateOfPage::Divided) {
		freeBlock(page, addr);
	}
	else {
		freeSeveralPages(page, header.freeBlocksNumber);
	}
}

void PageAllocator::freeSeveralPages(void *firstPage, unsigned int amount) {
	for (int i = 0; i < amount; ++i) {
		void* page = ((char*) (firstPage) + (i * PAGE_SIZE));
		PageHeader header = headersMap[page];

		header.state = StateOfPage::Free;
		headersMap[page] = header;

		freePagesList.push_back(page);
	}
}

void PageAllocator::freeBlock(void* page, void* addr) {
	PageHeader header = headersMap[page];

	void* next = header.startPosition;
	header.startPosition = addr;
	((BlockHeader*) addr)->nextBlock = next;

	header.freeBlocksNumber++;
	if (header.freeBlocksNumber == PAGE_SIZE / header.size) {
		header.state = StateOfPage::Free;
		freePagesList.push_back(page);

		std::vector<void*> pages = freeClassPagesMap[header.size];
		pages.erase(std::remove(pages.begin(), pages.end(), page), pages.end());
	}

	headersMap[page] = header;
}

void PageAllocator::mem_free() {
	freePagesList.clear();

	for (int i = 0; i < numberOfPages; ++i) {
		void* page = (void*) ((char*) (startPosition) + (i * PAGE_SIZE));
		PageHeader header = headersMap[page];

		header.state = StateOfPage::Free;
		headersMap[page] = header;

		freePagesList.push_back(page);
	}

	for (std::pair<unsigned int, std::vector<void*>> classes : freeClassPagesMap) {
		classes.second.clear();
	}
}

PageAllocator::~PageAllocator() {
	mem_free();
}

void PageAllocator::changeAddr(void *addr, void *newAddr) {
	void* page = getPage(addr);
	void* newPage = getPage(newAddr);

	unsigned int size = headersMap[page].size;
	unsigned int newSize = headersMap[newPage].size;

	memcpy(addr, newAddr, std::min(size, newSize));
}


void *PageAllocator::mem_realloc(void *addr, unsigned int size) {
	void* newAddr = mem_alloc(size);

	if (newAddr == nullptr) {
		return nullptr;
	}

	changeAddr(addr, newAddr);
	mem_free(addr);

	return newAddr;
}

void PageAllocator::mem_dump() {
	std::cout << "-----------------------GENERAL--INFORMATION--------------------------" << std::endl;

	std::cout << "Number of pages: \t" << numberOfPages << std::endl;
	std::cout << "Size of each page: \t" << PAGE_SIZE << std::endl;
	std::cout << "Number of free pages: \t" << freePagesList.size() << std::endl;

	std::cout << "------------------------pages--description--------------------------" << std::endl;

	for (int i = 0; i < numberOfPages; ++i) {
		void* page = (void*) ((char*) (startPosition) + (i * PAGE_SIZE));
		PageHeader header = headersMap[page];

		std::string state;
		std::string classSize = "";
		std::string freeSpace = "";

		if (header.state == StateOfPage::Free) {
			state = "Free";
		} else if (header.state == StateOfPage::Divided) {
			state = "Divided";
		} else {
			state = "Multiple";
		}

		if (header.state == StateOfPage::Divided) {
			classSize = "\n-----Size of each block in class:  " + std::to_string(header.size);
			freeSpace = "\n-----Number of free blocks:  " + std::to_string(header.freeBlocksNumber);
		}

		std::cout << i + 1 << ". Address:\t" << page << "\tType:\t" << state << freeSpace << classSize << std::endl;
	}
	std::cout << std::endl;
}