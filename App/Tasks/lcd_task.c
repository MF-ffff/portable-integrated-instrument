#include "lcd_task.h"
#include "semphr.h"
#include "task.h"
#include "lcd.h"

#include "label_dmm.h"

#include "label_pwr.h"

#include "panel_generator.h"

#include "panel_run_mode.h"

#include "panel_scale.h"
#include "label_scale.h"

#include "panel_cursor.h"
#include "label_cursor.h"

#include "panel_waveform.h"



//二进制信号量的句柄：用于在LCD发送数据时，进入阻塞态
static SemaphoreHandle_t xSemaphoreForDMA;

//事件组的句柄：负责“重绘”的事件组
EventGroupHandle_t xEventGroupForLCD;

static void LCD_BackLightControl(uint8_t On);
static void LCD_Reset(void);
static void SendCommand(uint8_t Cmd);
static void SendData(uint8_t *pData, uint16_t Size);


//“LCD的任务”的初始化函数
void vLCDTaskInit(void){
	//创建一个事件组，为后续各“界面的绘制”分配比特位作准备
	xEventGroupForLCD = xEventGroupCreate();
}


//LCD的任务函数
void vLCDTask(void *pvParameters){
	//创建二进制信号量：为后续LCD任务进入阻塞态做准备
	xSemaphoreForDMA = xSemaphoreCreateBinary();

	//点亮背光
	LCD_BackLightControl(1);

	//对于LCD开机后的首次绘制，重绘LCD屏的“所有标签”
	xEventGroupSetBits(xEventGroupForLCD,ALL_REPAINT_BITS);

	// 声明一个LCD_InitTypeDef类型的变量,对LCD初始化
	LCD_InitTypeDef LCD_InitStruct = {0};

	LCD_InitStruct.reset_callback = LCD_Reset;			// 设置LCD复位的回调函数
	LCD_InitStruct.send_command_callback = SendCommand;	// 设置发送命令的回调函数
	LCD_InitStruct.send_data_callback = SendData;		// 设置发送数据的回调函数

	LCD_Init(&LCD_InitStruct);

	LCD_SetBrush(0xffffff);		// 设置画刷颜色为白色
	LCD_Clear();				// 清空屏幕（最终屏幕为画刷颜色）

	for(;;){

		EventBits_t xBitRet;

	//1.当有界面需要重绘时，就会往界面的对应的比特位写1后，进行等待
		//当有任意一个位置1时，都会其对应标签重绘
		xBitRet = xEventGroupWaitBits(xEventGroupForLCD, ALL_REPAINT_BITS, pdTRUE, pdFALSE, portMAX_DELAY);

	//2.通过判断等待中的“事件组”中各比特位是否置1，从而判断各比特位对应的“标签”是否需要重绘
		if(xBitRet & PWR_LABEL_REPAINT_BIT){
			PwrLabel_Repaint();		//绘制并更新“电源稳压模块的标签页”
		}

		if(xBitRet & DMM_LABEL_REPAINT_BIT){
			DMMLabel_Repaint();		//绘制并更新“数字万用表的标签页”
		}
		if(xBitRet & GENERATOR_PANEL_REPAINT_BIT){
			vRepaintGenerator_Panel();		//绘制并更新“信号发生器的面板”
		}
		if(xBitRet & RUNMODE_PANEL_REPAINT_BIT){
			vRepaintRunMode_Panel();		//绘制并更新“运行模式的面板”
		}
		if(xBitRet & SCALE_PANEL_REPAINT_BIT){
			vRepaintScale_Panel();			//绘制并更新“缩放的面板”
		}
		if(xBitRet & SCALE_LABEL_REPAINT_BIT){
			ScaleLabel_Repaint();			//绘制并更新“缩放标签页”
		}
		if(xBitRet & CURSOR_PANEL_REPAINT_BIT){
			vRepaintCursor_Panel();			//绘制并更新“游标的面板”
		}
		if(xBitRet & WAVEFORM_PANEL_REPAINT_BIT){
			vRepaintWaveform_Panel();		//绘制并更新“示波器的面板”
		}
		if(xBitRet & CURSOR_LABEL_REPAINT_BIT){
			CursorLabel_Repaint();			//绘制并更新“游标的标签”
		}

		vTaskDelay(pdMS_TO_TICKS(100));
	}

}

//负责控制LCD屏幕的背光点亮与熄灭
static void LCD_BackLightControl(uint8_t on){

	if(on != 0){
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
	}
}

//负责复位LCD屏幕
static void LCD_Reset(void){
	//发送10μs的一个低脉冲，启动对LCD的复位
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	vTaskDelay(pdMS_TO_TICKS(1));
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);

	//等待125ms复位完成
	vTaskDelay(pdMS_TO_TICKS(125));
}

//负责通过SPI向LCD发送“命令”
//Cmd - 要发送的命令对应的十六进制数
static void SendCommand(uint8_t Cmd){

	extern SPI_HandleTypeDef hspi1;

	//切换到命令模式
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

	//向NSS引脚发送低电平表示选中从机
	HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_RESET);

	//调用SPI发送东西的API，发送命令
	HAL_SPI_Transmit(&hspi1,&Cmd,1,HAL_MAX_DELAY);

	//取消选中从机
	HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_SET);

}

//负责通过SPI向LCD发送“数据”
//pData - 代表要发送的数据（存储在数组中），Size - 总共数据的字节数
static void SendData(uint8_t *pData,uint16_t Size){

	extern SPI_HandleTypeDef hspi1;

	//切换到数据模式
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	//向NSS引脚发送低电平表示选中从机
	HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_RESET);

	//调用SPI发送东西的API，发送命令
	HAL_SPI_Transmit_DMA(&hspi1,pData,Size);

	//取信号进入阻塞态
	xSemaphoreTake(xSemaphoreForDMA,portMAX_DELAY);

	//取消选中从机
	HAL_GPIO_WritePin(LCD_NSS_GPIO_Port, LCD_NSS_Pin, GPIO_PIN_SET);

}

//当DMA发送完数据后，触发中断响应函数，负责给信号
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){

	if(hspi->Instance == SPI1){

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		xSemaphoreGiveFromISR(xSemaphoreForDMA, &xHigherPriorityTaskWoken);

		//当参数为pdTRUE时，切换至下一任务
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	}



}

