#include<iostream>
#include "ProcessService.h"
#include <vector>

#define MIN_PRIORITY 1
#define MAX_PRIORITY 32

#define MIN_DURATION 100
#define MAX_DURATION 300

using namespace std;

std::vector<Process*> generateProcesses(int n) {
	std::vector<Process*> processList;

	for (int i = 0; i < n; i++) {
		Process* newProcess = new Process();
		
		newProcess->id = i + 1;
		newProcess->priority = MIN_PRIORITY + rand() % MAX_PRIORITY;
		newProcess->duration = MIN_DURATION + rand() % MAX_DURATION;

		processList.push_back(newProcess);
	}

	return processList;
}


int main() {

	ProcessService processService;

	std::vector<Process*> processList = generateProcesses(20);

	for (Process* process : processList) {
		processService.add(process);
	}

	processService.print();

	processService.start();

	system("pause");
	return 0;
}