#include <iostream>
#include "TaskManager.h"

using namespace std;

int main()
{
    size_t queues = 3;
    size_t weightStart = 5;
    size_t weightEnd = 80;
    size_t intervalStart = 10;
    size_t intervalEnd = 100;
    TaskManager taskManager(queues, weightStart, weightEnd, intervalStart, intervalEnd);

    for (size_t interval = intervalEnd; interval >= intervalStart; interval -= 2)
    {
        taskManager.reset(interval);

        for (size_t i = 0; i < weightEnd*100; i++)
        {
            taskManager.processTick();
            if (interval == 30)
            {
                taskManager.saveData2();
            }
        }

        taskManager.saveData();
    }
}
