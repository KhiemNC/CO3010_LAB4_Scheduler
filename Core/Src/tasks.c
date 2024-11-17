/*
 * tasks.c
 *
 *  Created on: Nov 11, 2024
 *      Author: nguye
 */


#include "tasks.h"

void get3ButtonInput()
{
	getKeyInput(&button0);
	getKeyInput(&button1);
	getKeyInput(&button2);
}

void blinkyLED1()
{
	HAL_GPIO_TogglePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin);
}

void controlLED234()
{
	if (isButtonPressed(&button0))
	{
		HAL_GPIO_TogglePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin);
	}
	if (isButtonPressed(&button1))
	{
		HAL_GPIO_TogglePin(LED_RED_3_GPIO_Port, LED_RED_3_Pin);
	}
	if (isButtonPressed(&button2))
	{
		HAL_GPIO_TogglePin(LED_RED_4_GPIO_Port, LED_RED_4_Pin);
	}
}

void blinkyLED5()
{
	HAL_GPIO_TogglePin(LED_RED_5_GPIO_Port, LED_RED_5_Pin);
}

