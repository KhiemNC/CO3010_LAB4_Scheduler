/*
 * scheduler.c
 *
 *  Created on: Nov 11, 2024
 *      Author: nguye
 */


#include "scheduler.h"

struct Node listData[SCH_MAX_TASKS];
int32_t head = -1;
int32_t freeHead = 0;
enum ErrorCode Error_code_G = INIT;

void resetNodeData(int32_t index)
{
	listData[index].data.pTask = NULL;
	listData[index].data.Delay = 0;
	listData[index].data.Period = 0;
	listData[index].data.RunMe = 0;
	listData[index].data.TaskID = 0;
}

void initFreeList()
{
	for (int i = 0; i < SCH_MAX_TASKS - 1; ++i)
	{
		listData[i].next = i + 1;
	}
	listData[SCH_MAX_TASKS - 1].next = -1;
}

void List_init()
{
	head = -1;
	freeHead = 0;
	initFreeList();
}

int32_t List_getFreeIndex()
{
	if (freeHead == -1)
	{
		return -1;
	}
	int32_t freeIndex = freeHead;
	freeHead = listData[freeHead].next;

	return freeIndex;
}

void List_freeIndex(int32_t index)
{
	resetNodeData(index);

	listData[index].next = freeHead;
	freeHead = index;
}

uint8_t List_isEmpty()
{
	return head == -1;
}

void List_insert(int32_t freeIndex)
{
	if (head == -1) // INSERT AT HEAD
	{
		// Ensure that if Delay = 0, it can be dispatched immediately
		if (listData[freeIndex].data.Delay == 0)
		{
			listData[freeIndex].data.RunMe = 1;
		}

		head = freeIndex;
		listData[head].next = -1;
		return;
	}

	// FIND INDEX TO INSERT
	int32_t current = head;
	int32_t previous = -1;
	while (current != -1)
	{
		if (listData[current].data.Delay > listData[freeIndex].data.Delay) // INSERT BEFORE CURRENT
		{
			// Ensure that if Delay = 0, it can be dispatched immediately
			if (listData[freeIndex].data.Delay == 0)
			{
				listData[freeIndex].data.RunMe = 1;
			}

			listData[freeIndex].next = current;
			if (previous == -1)
			{
				head = freeIndex;
			}
			else
			{
				listData[previous].next = freeIndex;
			}

			// The task behind need to minus by the new task delay
			listData[current].data.Delay -= listData[freeIndex].data.Delay;
			// Ensure that if Delay = 0, it can be dispatched immediately
			if (listData[current].data.Delay == 0)
			{
				listData[current].data.RunMe = 1;
			}
			return;
		}

		// If we have not reach the index yet, minus the delay and move on
		listData[freeIndex].data.Delay -= listData[current].data.Delay;
		previous = current;
		current = listData[current].next;
	}

	// INSERT AT TAIL (current = -1)
	// Ensure that if Delay = 0, it can be dispatched immediately
	if (listData[freeIndex].data.Delay == 0)
	{
		listData[freeIndex].data.RunMe = 1;
	}
	listData[previous].next = freeIndex;
	listData[freeIndex].next = -1;
}

void List_removeHead()
{
	if (head == -1)
	{
		Error_code_G = ERROR_SCH_EMPTY_LIST_CAN_NOT_REMOVED;
		return;
	}

	head = listData[head].next;
}

void SCH_Init(void)
{
	List_init();
	Error_code_G = INIT;
}
void SCH_Update(void)
{
	if (List_isEmpty())
	{
		return;
	}

	if (listData[head].data.Delay > 0)
	{
		--listData[head].data.Delay;
	}

	if (listData[head].data.Delay == 0)
	{
		listData[head].data.RunMe = 1;
	}
}
void SCH_Dispatch_Tasks(void)
{
	if (List_isEmpty())
	{
		return;
	}

	int32_t current = head;
	if (listData[current].data.RunMe == 1)
	{
		// RUN TASK
		listData[current].data.pTask();

		listData[current].data.RunMe = 0;
		List_removeHead();
		if (listData[current].data.Period == 0)
		{
			// ONE SHOT
			List_freeIndex(current);
		}
		else
		{
			// PERIODIC
			listData[current].data.Delay = listData[current].data.Period;
			List_insert(current);
		}
	}

	// Report system status
	SCH_Report_Status();

	if (listData[current].data.RunMe == 0)
	{
		// The scheduler enters idle mode at this point
		SCH_Go_To_Sleep();
	}
}
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD)
{
	int32_t freeIndex = List_getFreeIndex();
	if (freeIndex == -1)
	{
		Error_code_G = ERROR_SCH_EXCEED_MAX_TASK;
		return RETURN_ADD_ERROR;
	}

	// Assign data to freeIndex
	listData[freeIndex].data.pTask = pFunction;
	listData[freeIndex].data.Delay = DELAY;
	listData[freeIndex].data.Period = PERIOD;
	listData[freeIndex].data.RunMe = 0;
	listData[freeIndex].data.TaskID = freeIndex;

	List_insert(freeIndex);
	return freeIndex; // TASK ID
}
uint8_t SCH_Delete_Task(uint32_t taskID)
{
	if (List_isEmpty())
	{
		Error_code_G = ERROR_SCH_EMPTY_LIST_CAN_NOT_DELETE;
		return RETURN_ERROR;
	}

	int current = head;
	int previous = -1;
	while (current != -1)
	{
		if (listData[current].data.TaskID == taskID) // FOUND THE INDEX
		{
			if (previous == -1) // REMOVE AT HEAD
			{
				if (listData[current].next != -1) // IF NOT TAIL
				{
					// Modify the Delay of the behind task
					listData[listData[current].next].data.Delay += listData[current].data.Delay;

					// Ensure that if Delay = 0, it can be dispatched immediately
					if (listData[listData[current].next].data.Delay != 0)
					{
						listData[listData[current].next].data.RunMe = 0;
					}
				}
				List_removeHead();
				List_freeIndex(current);
			}
			else // NOT REMOVE AT HEAD
			{
				if (listData[current].next != -1) // IF NOT TAIL
				{
					// Modify the Delay of the behind task
					listData[listData[current].next].data.Delay += listData[current].data.Delay;

					// Ensure that if Delay = 0, it can be dispatched immediately
					if (listData[listData[current].next].data.Delay != 0)
					{
						listData[listData[current].next].data.RunMe = 0;
					}
				}
				listData[previous].next = listData[current].next;
				resetNodeData(current);
				List_freeIndex(current);
			}
			return RETURN_NORMAL;
		}
		// GO ON TO FIND THE INDEX
		previous = current;
		current = listData[current].next;
	}
	return RETURN_NORMAL;
}
void SCH_Report_Status()
{

}
void SCH_Go_To_Sleep()
{

}
