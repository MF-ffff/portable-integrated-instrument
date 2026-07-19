#ifndef DRIVERS_KEY_TASK_H_
#define DRIVERS_KEY_TASK_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct{
	GPIO_PinState (*ReadPinCallback)(void);		//负责读取按钮引脚电平的函数
	void (*ClickedCallback)(void);	//负责执行按钮按下的函数
}Key_InitTypeDef;

typedef struct{
	Key_InitTypeDef Init;	//声明一个结构体类型的变量，用于调用回调函数和按钮函数
	GPIO_PinState previous;    //声明一个值保存上一时刻的按钮状态
}Key_HandleTypeDef;

void Key_Init(Key_HandleTypeDef *Handle);
void Key_Scan(Key_HandleTypeDef *Handle);

#endif
