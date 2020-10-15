#include <iostream>
#include "Allocator.h"

//			  s   p   n
//HEADER_SIZE 1 + 4 + 4

Allocator::Allocator() {
	memory = new char[MAX_SIZE];
	
	freeblock = &memory[0];
	memory[0] = 0;

	*(int*)(memory + 1) = -1;
	*(int*)(memory + 5) = MAX_SIZE - HEADER_SIZE;
}

void *Allocator::mem_alloc(int size) {
	
	void* result = nullptr;

	if (size > MAX_SIZE - HEADER_SIZE) {
		std::cout << "Max size for this allocator excessed" << std::endl;
		return result;
	}

	char* fb = freeblock;

	while (true) {

		if (fb >= &memory[MAX_SIZE - 1] - HEADER_SIZE) {
			return nullptr;
		}

		if ((int) *fb == 0) {
			if (*(int*)(fb + 5) < size) {
				std::cout << "Too small block" << std::endl;
				fb += *(int*)(fb + 5);
				continue;
			}
			
			*fb = 1;
			int padding = 4 - (size + HEADER_SIZE) % 4;
			int block_size = padding + size + HEADER_SIZE;
			*(int*)(fb + 5) = block_size;
			result = (void*)(fb + HEADER_SIZE);

			char* next_block =  fb + block_size;

			if (*next_block != 1) {
				*(int*)(next_block + 1) = *(int*)(fb + 5);
				*next_block = 0;
				*(int*)(next_block + 5) = &memory[MAX_SIZE] - next_block + HEADER_SIZE;
			} 

			if (freeblock == fb) {
				freeblock = next_block;
			}

			break;
		} else {

			int size = (*(int*)fb + 5);
			fb = fb + (*(int*)fb + 5);
		}
	}

	std::cout << " Beginning of allocated block: " << result << " Size of allocated block: " << size << std::endl;
	
	return result;
}

void Allocator::mem_free(void* addr) {
	char* block = (char*)addr - HEADER_SIZE;
	if (*block != 1) {
		return;
	}

	*block = 0;

	int prev_size = *(int*)(block + 1);
	int curr_size = *(int*)(block + 5);

	char* prev = block - prev_size;
	char* next = block + curr_size;

	int next_size = *(int*)(next + 5);

	char* nnext = next + next_size;

	std::cout << " Block with beginning at: " << addr << " deallocated successfully" << std::endl;

	if (prev_size == -1 && *next == 0) {
		int new_block_size = *(int*)(block + 5) + *(int*)(next + 5);
		*(int*)(block + 5) = new_block_size;

		return;
	}

	if (*prev == 0 && *next == 0) {
		int new_block_size = *(int*)(block + 1) + *(int*)(block + 5) + *(int*)(next + 1);
		*(int*)(prev + 5) = new_block_size;
		*(int*)(nnext + 1) = new_block_size;

		return;
	}

	if (*prev == 1 && *next == 0) {
		int new_block_size = *(int*)(block + 5) + *(int*)(next + 5);
		*(int*)(prev + 5) = new_block_size;
		*(int*)(nnext + 1) = new_block_size;

		return;
	}
	
	if (*prev == 0 && *next == 1) {
		int new_block_size = *(int*)(block + 1) + *(int*)(block + 5);
		*(int*)(prev + 5) = new_block_size;
		*(int*)(next + 1) = new_block_size;

		return;
	}
}

void *Allocator::mem_realloc(void* addr, int size) {
	char* block = (char*)addr - HEADER_SIZE;
	
	if (*block != 1) {
		return nullptr;
	}

	int block_size = *(int*)(block + 5);
	int data_size = block_size - HEADER_SIZE;

	char* data = new char[data_size];

	char* c = block + HEADER_SIZE;
	for (int i = 0; i < data_size; i++) {
		data[i] = *c;
		c++;
	}

	mem_free(addr);

	char* new_mem = (char*) mem_alloc(size);
	
	c = new_mem;

	for (int i = 0; (i < size) || (i < data_size); i++) {
		*c = data[i];
		c++;
	}

	std::cout << " Block with beginning at " << addr << " reallocated successfully to " << (void*) c << " with size" << size << std::endl;

	return (void*) c;
}