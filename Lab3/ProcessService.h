#pragma once
#include <vector>


struct Process {
	int id;
	int duration;
	int priority;
};


class ProcessService {
public:
	ProcessService();

	void add(Process* newProcess);
	Process* getFirst();
	void removeFirst();
	void start();
	void print();
private:
	struct ProcessNode {
		Process* process;
		ProcessNode* next;
	};
	
	void printProcess(Process* process);
	ProcessNode* head;
};