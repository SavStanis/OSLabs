#include <iostream>
#include "Allocator.h"

using namespace std;

int main() {
	
	Allocator alloc = Allocator(16000);
	alloc.mem_alloc(100);
	alloc.mem_alloc(100);
	alloc.mem_alloc(100);
	alloc.mem_alloc(100);
	alloc.mem_alloc(100);
	alloc.mem_alloc(1000);
	alloc.mem_alloc(1000);
	alloc.mem_alloc(3024);	

	alloc.mem_dump();

	alloc.mem_alloc(100);

	system("pause");
}