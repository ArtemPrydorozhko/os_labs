#include <ctime>
#include "TaskManager.h"
#include <iostream>

using namespace std;

TaskManager::TaskManager(size_t queueAmount, size_t weightStart, size_t weightEnd, size_t intervalStart, size_t intervalEnd)
{
	tickToAddTask = currentTick = 1;
	this->queueAmount = queueAmount;
	this->weightStart = weightStart;
	this->weightEnd = weightEnd;
	this->intervalStart = intervalStart;
	this->intervalEnd = intervalEnd;
	processingTask = false;
	taskAmount = maxTaskAmount = 0;
	queues = vector<list<Task>>(queueAmount);
	srand(time(0));
	file.open("data.csv", ios::out | ios::trunc);
	file2.open("taskAwaiting.csv", ios::out | ios::trunc);
}

TaskManager::~TaskManager()
{
	file.close();
	file2.close();
}

void TaskManager::processTick()
{
	if (tickToAddTask == currentTick)
	{
		addTask();
		tickToAddTask = currentTick + rand() % (intervalEnd - intervalStart + 1) + intervalStart;
	}

	if (processingTask)
	{
		processTask();
	}
	else
	{
		timeToProcessTask = -1;

		for (size_t i = 0; i < queues.size(); i++)
		{
			if (queues[i].size())
			{
				currentTask = queues[i].front();
				queues[i].pop_front();

				if (queues.size() - 1 == i || currentTask.timeLeft <= i + 1)
				{
					timeToProcessTask = currentTask.timeLeft;
				}
				else
				{
					timeToProcessTask = i + 1;
				}
				break;
			}
		}

		if (timeToProcessTask != -1)
		{
			processingTask = true;
			processTask();
		}
	}
	
	currentTick++;
}

void TaskManager::addTask()
{
	size_t weight = rand() % (weightEnd - weightStart + 1) + weightStart;
	Task task(weight, currentTick);
	queues[0].push_back(task);
	taskAmount++;
	if (taskAmount > maxTaskAmount)
	{
		maxTaskAmount = taskAmount;
	}
}

void TaskManager::processTask()
{
	currentTask.timeLeft--;
	timeToProcessTask--;
	updateAwaitingTime();
	if (!currentTask.timeLeft)
	{
		timeToProcessTask = 0;
		processingTask = false;
		currentTask.timeFinished = currentTick;
		doneTasks.push_back(currentTask);
		taskAmount--;
	}
	else if (!timeToProcessTask)
	{
		processingTask = false;
		if (currentTask.queueNum != queues.size() - 1)
		{
			currentTask.queueNum++;
			queues[currentTask.queueNum].push_back(currentTask);
		}
	}
	saveData();
}

void TaskManager::updateAwaitingTime()
{
	for (size_t i = 0; i < queues.size(); i++)
	{
		for (Task &task : queues[i])
		{
			task.awaitTime++;
		}
	}
}

void TaskManager::saveData()
{
	size_t avAwTime = 0;
	for (size_t i = 0; i < doneTasks.size(); i++)
	{
		avAwTime += doneTasks[i].awaitTime;
	}
	for (size_t i = 0; i < queues.size(); i++)
	{
		for (Task task : queues[i])
		{
			avAwTime += task.awaitTime;
		}
	}

	avAwTime = ceil(avAwTime / ((double)taskAmount + doneTasks.size()));
	file << currentTick << ';' << taskAmount << ';' << maxTaskAmount << ';' << avAwTime << endl;
}

void TaskManager::saveData2()
{
	for (size_t i = 0; i < doneTasks.size(); i++)
	{
		file2 << doneTasks[i].awaitTime << '\n';
	}
	for (size_t i = 0; i < queues.size(); i++)
	{
		for (Task task : queues[i])
		{
			file2 << task.awaitTime << '\n';
		}
	}

	file2.flush();
}