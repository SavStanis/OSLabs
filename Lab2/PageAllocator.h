#include <vector>
#include <map>
#include <cmath>

enum class StateOfPage {
	Free,
	Divided,
	Multiple,
};

struct Header {
	void* startPosition;
	StateOfPage state;
	unsigned int size;
	unsigned int freeBlocksNumber;
};

struct BlockHeader {
	void* nextBlock;
};

class PageAllocator {
public:
	PageAllocator(unsigned int size);
	void *mem_alloc(unsigned int size);
	void *mem_realloc(void* addr, unsigned int size);
	void mem_free(void* addr);
	void mem_free();
	void mem_dump();
	~PageAllocator();

private:
	void* startPosition;
	int const PAGE_SIZE = 4096;			// 4 kb
	int const MIN_ALLOC_SIZE = 16;		// 2^x (x >= 4)
	unsigned int numberOfPages;

	std::vector<void*> freePagesList;
	std::map<void*, Header> headersMap;
	std::map<unsigned int, std::vector<void*>> freeClassPagesMap;

	void* getPage(void* addr);
	void* divideFreePage(unsigned int classSize);
	void* allocateBlock(void* page);
	void* allocateSeveralPages(unsigned int size);
	void freeBlock(void* page, void* addr);
	void freeSeveralPages(void* firstPage, unsigned int pagesAmount);
	void changeAddr(void* addr, void* newAddr);
};