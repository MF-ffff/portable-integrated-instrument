#include "waveform_capture.h"
#include "panel_waveform.h"
#include "lcd_task.h"
#include "timers.h"
#include "semphr.h"

static uint16_t raw_DataBuffer[1024];	//用于读取ADC的原始数据

static float waveform[1024];	//用于保存：处理完ADC读取出的数据后的数据

static TimerHandle_t xHoldOffTimer; // 软件定时器：用于禁止触发
static TimerHandle_t xForceTriggerTimer; // 软件定时器：用于强制触发

SemaphoreHandle_t xMutexForWaveform;	//为数组“waveform”声明一个互斥锁句柄，用于保护数组“waveform”

static void HoldOffTimerCallback(TimerHandle_t xTimer);		//用于禁止触发的“软件定时器回调函数”
static void ForceTriggerTimerCallback(TimerHandle_t xTimer);		//用于强制触发的“软件定时器回调函数”


//
//初始化波形捕捉器
//
void WaveformCapture_Init(void){

	extern TIM_HandleTypeDef htim3;

	//启动定时器3，用于触发ADC1的常规序列的通道1
	HAL_TIM_Base_Start(&htim3);

	// 创建软件定时器，用于禁止触发（10ms）
	xHoldOffTimer = xTimerCreate("Hold Off", pdMS_TO_TICKS(10), pdFALSE, NULL, HoldOffTimerCallback);

	// 创建软件定时器，用于强制触发（100ms）
	xForceTriggerTimer = xTimerCreate("Force Trigger", pdMS_TO_TICKS(100), pdTRUE, NULL, ForceTriggerTimerCallback);

	//启动“强制触发的”软件定时器
	xTimerStart(xForceTriggerTimer,portMAX_DELAY);

	//为数组“waveform”创建一个互斥锁，用于保护数组“waveform”
	xMutexForWaveform = xSemaphoreCreateMutex();

}

//
//EXTI中断的回调函数：
//以DMA方式启动ADC1
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	extern ADC_HandleTypeDef hadc1;

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(GPIO_Pin == Trigger_Pin){

		//以DMA方式启动ADC1
		//最后一个参数length：1024的含义————代表要连续采集 1024 个点，每个点就是一次ADC转换的数值
		//而转化一次ADC，需要一次TRGO信号，因此要等1024个TRGO信号
		//当1024个数值都被采集完毕后，就触发ADC的中断
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *)raw_DataBuffer, 1024);

		HAL_NVIC_DisableIRQ(Trigger_EXTI_IRQn); // 屏蔽中断，以防再次触发

		//启动“禁止触发”的软件定时器，到期后再恢复中断
		xTimerStartFromISR(xHoldOffTimer, &xHigherPriorityTaskWoken);

		//复位“强制触发”的软件定时器重新计时
		xTimerResetFromISR(xForceTriggerTimer,&xHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	}

}

//
//ADC中断的回调函数（DMA完成采集和转化后触发）
//
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	if(hadc->Instance == ADC1){

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		//1.把采集到的原始数据转化为电压，并处理
		//获取“互斥锁”，用于保护数组“waveform”
		if(xSemaphoreTakeFromISR(xMutexForWaveform,&xHigherPriorityTaskWoken) == pdPASS){

			for(int i=0; i<1024; i++)
				{

					waveform[i] = (raw_DataBuffer[i] / 4095.0f * 3.3f) * 2.0f - 2.5f;
				}

		//归还锁
		xSemaphoreGiveFromISR(xMutexForWaveform,&xHigherPriorityTaskWoken);

			//2.以事件组的方式，通知对LCD的波形面板进行重绘
			extern EventGroupHandle_t xEventGroupForLCD;
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;

			xEventGroupSetBitsFromISR(xEventGroupForLCD, WAVEFORM_PANEL_REPAINT_BIT, &xHigherPriorityTaskWoken);

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

		}
	}

}

//
//以数组形式获取波形数据
//
float *WaveformCapture_GetWaveform(void)
{
	return waveform;
}

//
// 禁止触发定时器的回调函数
//
static void HoldOffTimerCallback(TimerHandle_t xTimer)
{
	// 清除中断标志
	__HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_3);
	HAL_NVIC_ClearPendingIRQ(EXTI3_IRQn);

	// 再次开启中断，禁止触发结束
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

//
// 强制触发定时器的回调函数
//
static void ForceTriggerTimerCallback(TimerHandle_t xTimer)
{
	//手动开启EXTI3的中断（对应PA3引脚）
	__HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_3);

}

//
//设置波形捕捉器的采样率（即设计TIM3的时基单元）
//PSC——预分频器的值，ARR——自动重装寄存器的值
//
void WaveformCapture_SetSampleRate(uint16_t PSC,uint16_t ARR){

	extern TIM_HandleTypeDef htim3;

	//影子寄存器保证的是下一轮生效，若想立即生效，则需要先关闭定时器
	HAL_TIM_Base_Stop(&htim3);	//临时关闭定时器

	__HAL_TIM_SET_PRESCALER(&htim3,PSC);	//修改PSC的值
	__HAL_TIM_SET_AUTORELOAD(&htim3,ARR);	//修改ARR的值

	HAL_TIM_Base_Start(&htim3);	//恢复定时器
}

//
//“获取”波形捕捉器的采样率，用于波形面板文件进行绘制
//
uint32_t WaveformCapture_GetSampleRate(void){

	extern TIM_HandleTypeDef htim3;

	//获取PSC和ARR的值
	uint16_t PSC = htim3.Instance->PSC;
	uint16_t ARR = htim3.Instance->ARR;

	//计算并采样率=72MHZ/PSC+1/ARR+1
	return (uint32_t)(72.0e6f/(PSC+1)/(ARR+1));
}

