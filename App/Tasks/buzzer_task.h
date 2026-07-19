#ifndef TASKS_BUZZER_TASK_H_
#define TASKS_BUZZER_TASK_H_

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

void Buzzer_Init(void);		//蜂鸣器的初始化(创建一个二进制信号量即可)

//蜂鸣器的任务函数：负责取信号
//取到信号后，控制蜂鸣器鸣叫
void vBuzzerTask(void *pvParameters);

void Buzzer_Beep(void);		//蜂鸣器的给信号函数，负责给二进制信号量信号

#endif
