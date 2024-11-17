/*
 * scheduler.h
 *
 *  Created on: Nov 11, 2024
 *      Author: nguye
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"

typedef struct
{
	// Pointer to the task (must be a "void (void)" function)
	void (*pTask)(void);
	// Delay (ticks) until the function will (next) be run
	uint32_t Delay;
	// Interval (ticks) between subsequent runs
	uint32_t Period;
	// Incremented (by scheduler) when task is due to execute
	uint8_t RunMe;
	// Save the ID of the task. In this LAB, this will be the index in task List
	uint32_t TaskID;

} sTask;

struct Node
{
	sTask data;
	int32_t next;
};

enum ErrorCode
{
	INIT,
	ERROR_SCH_EXCEED_MAX_TASK,
	ERROR_SCH_EMPTY_LIST_CAN_NOT_REMOVED,
	ERROR_SCH_EMPTY_LIST_CAN_NOT_DELETE
};

#define RETURN_ERROR 0
#define RETURN_NORMAL 1
#define RETURN_ADD_ERROR UINT32_MAX

// MUST BE ADJUSTED FOR EACH PROJECT
#define SCH_MAX_TASKS 40
#define NO_TASK_ID 0

extern struct Node listData[SCH_MAX_TASKS];
extern int32_t head;
extern int32_t freeHead;
extern enum ErrorCode Error_code_G;

/**
 * @brief Set NULL and 0 value for a node.
 */
void resetNodeData(int32_t index);

/**
 * @brief This function init the FreeList.
 *
 * This function will ensure the freeHead hold all free
 * elements in the listData[], all the node will be linked
 * together at start.
 */
void initFreeList();

/**
 * @brief This function init the the listData[].
 *
 * All parameters related to the list (head, freeHead)
 * will be assigned the init values.
 */
void List_init();

/**
 * @brief Return a free index if it exists.
 *
 * This function find the FreeList hold by freeHead
 * to find and return a freeIndex, if there are not
 * any available, it return -1. The freeIndex will be
 * removed from the freeList.
 *
 * @return freeIndex, -1 if there are not any available.
 */
int32_t List_getFreeIndex();

/**
 * @brief Free a index out of LinkedList to FreeList.
 *
 * This function find insert the index to the FreeList
 * (which hold by freeHead).
 */
void List_freeIndex(int32_t index);

/**
 * @brief Check if the LinkedList is empty or not.
 *
 *
 * @return 1 if empty, 0 if not.
 */
uint8_t List_isEmpty();

/**
 * @brief Insert a Node to the list base on Delay.
 *
 * This function insert a Node, which is assigned value,
 * in the freeIndex to the LinkedList (task list).
 *
 * @param[in] freeIndex To indicate the index that is the Node
 * need to be inserted. Notice that the freeIndex data is already.
 */
void List_insert(int32_t freeIndex);

/**
 * @brief Remove the Node in Head.
 */
void List_removeHead();

void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t taskID);
void SCH_Report_Status();
void SCH_Go_To_Sleep();


#endif /* INC_SCHEDULER_H_ */
