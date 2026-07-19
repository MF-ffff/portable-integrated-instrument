#ifndef TASKS_LCD_TASK_H_
#define TASKS_LCD_TASK_H_

#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"

#define PWR_LABEL_REPAINT_BIT (EventBits_t)(0x01 << 0)				//第0位代表：稳压电源模块的标签
#define DMM_LABEL_REPAINT_BIT (EventBits_t)(0x01 << 1)				//第1位代表：万用表模块的标签
#define GENERATOR_PANEL_REPAINT_BIT (EventBits_t)(0x01 << 2)		//第2位代表：信号发生器的面板
#define RUNMODE_PANEL_REPAINT_BIT (EventBits_t)(0x01 << 3)			//第3位代表：运行模式的面板
#define SCALE_PANEL_REPAINT_BIT (EventBits_t)(0x01 << 4)			//第4位代表：缩放的面板
#define SCALE_LABEL_REPAINT_BIT (EventBits_t)(0x01 << 5)			//第5位代表：缩放的标签
#define CURSOR_PANEL_REPAINT_BIT (EventBits_t)(0x01 << 6)			//第6位代表：游标的面板
#define WAVEFORM_PANEL_REPAINT_BIT (EventBits_t)(0x01 << 7)			//第7位代表：示波器的面板
#define CURSOR_LABEL_REPAINT_BIT (EventBits_t)(0x01 << 8)			//第8位代表：游标的标签

//所有"面板和标签"用到的比特位
#define ALL_REPAINT_BITS (PWR_LABEL_REPAINT_BIT|DMM_LABEL_REPAINT_BIT|GENERATOR_PANEL_REPAINT_BIT|RUNMODE_PANEL_REPAINT_BIT|SCALE_PANEL_REPAINT_BIT|SCALE_LABEL_REPAINT_BIT|CURSOR_PANEL_REPAINT_BIT|WAVEFORM_PANEL_REPAINT_BIT|CURSOR_LABEL_REPAINT_BIT)

//LCD的任务函数
void vLCDTask(void *pvParameters);

//“LCD的任务”的初始化函数
void vLCDTaskInit(void);


#endif
