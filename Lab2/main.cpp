#include <iostream>
#include "Allocator.h"

using namespace std;

int main() {
	
	Allocator alloc = Allocator(16000);

	void* test7 = alloc.mem_alloc(3000);
	alloc.mem_dump();
	alloc.mem_free(test7);
	alloc.mem_dump();


	system("pause");
}