#pragma once
#include <vector>


struct Process {
	int id;
	int duration;
	int priority;
};

struct PriorityHeader {
	int priority;
	int numberOfProcesses;
	int waitingTimeSum;
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