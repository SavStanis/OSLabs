#include "ProcessService.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <map>

ProcessService::ProcessService() {
	head = nullptr;
}

void ProcessService::add(Process* newProcess) {
	ProcessNode* newProcessNode = new ProcessNode();

	newProcessNode->process = newProcess;

	if (head == nullptr) {
		newProcessNode->next = nullptr;
		head = newProcessNode;
		
		return;
	}

	if (head->process->priority > newProcessNode->process->priority) {
		newProcessNode->next = head;
		head = newProcessNode;

		return;
	}

	ProcessNode* prevProcess = head;
	ProcessNode* nextProcess = head->next;

	while (prevProcess->next != nullptr && nextProcess->process->priority <= newProcess->priority) {
		prevProcess = nextProcess;
		nextProcess = nextProcess->next;
	}

	prevProcess->next = newProcessNode;
	newProcessNode->next = nextProcess;
}

Process* ProcessService::getFirst() {
	return (head == nullptr) ?  nullptr : head->process;
}

void ProcessService::removeFirst() {
	if (head != nullptr) {
		head = head->next;
	}
}


void ProcessService::printProcess(Process * process) {
	std::cout << process->id << "\t|" << process->priority << "\t\t|" << process->duration << std::endl;
}

void ProcessService::print() {
	ProcessNode* currProcessNode = head;
	std::cout << "Id" << "\t|" << "Priority" << "\t|" << "Duration" << std::endl;
	std::cout << "--------+---------------+---------------------" << std::endl;
	while (currProcessNode != nullptr) {
		printProcess(currProcessNode->process);
		currProcessNode = currProcessNode->next;
	}
}


void ProcessService::start() {
	int timeCounter = 0;
	int processCounter = 0;
	int waitingTimeCounter = 0;

	std::map<int, PriorityHeader> priorityMap;


	Process* process = getFirst();
	removeFirst();

	std::cout << "----------------------------------------------------------" << std::endl;
	std::cout << "-------------Execution-of-the-processes-started-----------" << std::endl;
	std::cout << "----------------------------------------------------------" << std::endl;


	std::cout << "Id" << "\t|" << "Waiting time" << "\t|" << "Priority" << "\t|" << "Duration" << std::endl;
	std::cout << "-----------+--------------------+-------------------------+---------------------" << std::endl;

	while (process != nullptr) {
		if (priorityMap.count(process->priority) == 0) {
			PriorityHeader priorityHeader;

			priorityHeader.priority = process->priority;
			priorityHeader.numberOfProcesses = 1;
			priorityHeader.waitingTimeSum = timeCounter;

			priorityMap[process->priority] = priorityHeader;
		}
		else {
			PriorityHeader priorityHeader = priorityMap[process->priority];
			priorityHeader.numberOfProcesses++;
			priorityHeader.waitingTimeSum += timeCounter;

			priorityMap[process->priority] = priorityHeader;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(process->duration));

		std::cout << process->id << "\t|" << timeCounter << "ms\t\t|" << process->priority << "\t\t|" << process->duration << std::endl;

		timeCounter += process->duration;
		processCounter++;
		waitingTimeCounter += timeCounter;

		process = getFirst();
		removeFirst();
	}

	std::cout << "Average waiting time: " << waitingTimeCounter / processCounter << std::endl;
	std::cout << "----------------------------------" << std::endl;

	for (std::pair<int, PriorityHeader> pair : priorityMap) {
		std::cout << "Priority: " << pair.first << " | Average waiting time: " << pair.second.waitingTimeSum / pair.second.numberOfProcesses << std::endl;
	}
}
