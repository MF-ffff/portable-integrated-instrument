#include "buzzer_task.h"

static SemaphoreHandle_t xBinarySempahoreForBuzzer; 	//声明一个计数信号量的句柄，用于后续表示二进制信号量

//蜂鸣器的初始化(创建一个计数信号量即可)
void Buzzer_Init(void){
	xBinarySempahoreForBuzzer = xSemaphoreCreateCounting(5,0);
}

//蜂鸣器的任务函数：负责取信号
//取到信号后，控制蜂鸣器鸣叫
void vBuzzerTask(void *pvParameters){
	//任务函数：固定要么死循环，要么执行完后删除
	for(;;){
		//取信号(等待时间，即阻塞时间为无限长；当取到信号后，才会执行下一步)
		xSemaphoreTake(xBinarySempahoreForBuzzer,portMAX_DELAY);

		//取到信号后，蜂鸣器开始鸣叫（写1使能蜂鸣器）
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
		vTaskDelay(pdMS_TO_TICKS(100));
		HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
		vTaskDelay(pdMS_TO_TICKS(100));
	}

}

//蜂鸣器的给信号函数，负责给计数信号量信号
void Buzzer_Beep(void){
	xSemaphoreGive(xBinarySempahoreForBuzzer);
}

