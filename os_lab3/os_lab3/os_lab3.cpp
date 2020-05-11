#include <iostream>
#include "TaskManager.h"

using namespace std;

int main()
{
    size_t queues;
    size_t weightStart;
    size_t weightEnd;
    size_t intervalStart;
    size_t intervalEnd;

    std::cout << "Enter num of queues, weight start, weight end, interval start, interval end" << endl;
    std::cin >> queues >> weightStart >> weightEnd >> intervalStart >> intervalEnd;

    TaskManager taskManager(queues, weightStart, weightEnd, intervalStart, intervalEnd);

    for (size_t i = 0; i < 500; i++)
    {
        taskManager.processTick();
    }

    taskManager.saveData2();
}
