#include "panel_run_mode.h"
#include "lcd.h"
#include "lcd_task.h"

//声明一个变量，用于表示当前光标所在的位置：
//负数————表示未选中，0————表示选中参数1
static int iSelectedIndex = -1;

//声明一个变量，用于表示当前“运行模式”————即参数1
//0————表示自动运行（Auto-run），1————表示单词运行（One-shot）
static uint8_t ucRunMode = 0;

//
//代表对“运行模式的面板”进行绘制
//
void vRepaintRunMode_Panel(void){

	//1.绘制标题栏，坐标（260，8），宽60、高15，颜色#FF7BFF
	LCD_SetBrush(0xFF7BFF);
	LCD_SetCursor(260, 8);
	LCD_FillRect(60, 15);

	//2.绘制标题的文字，坐标（262, 21），颜色#000000
	LCD_SetPen(0x000000);
	LCD_SetCursor(262, 21);
	LCD_PrintString("Run Mode");

	//3.绘制剩下的面板背景，坐标(260, 23)，宽60、高35，颜色#CCCCCC
	LCD_SetBrush(0xCCCCCC);
	LCD_SetCursor(260, 23);
	LCD_FillRect(60, 35);

	//4.当该面板被“选中”时，绘制天蓝色光标，(264,34)，宽51，高11，颜色#00FFFF
	if(iSelectedIndex == 0){
		LCD_SetBrush(0x00FFFF);
		LCD_SetCursor(264, 34);
		LCD_FillRect(51, 11);
	}

	//5.绘制参数1（运行模式），坐标（265, 43）
		//填充文字背景色
	if(iSelectedIndex == 0){
			LCD_SetBrush(0x00FFFF);
		}
	else{
			LCD_SetBrush(0xCCCCCC);
		}

		//根据ucRunMode的值，绘制具体参数
	LCD_SetCursor(265, 43);

	if(ucRunMode == 0){
		LCD_PrintString("Running");
	}
	else{
		LCD_PrintString("One-Shot");
	}

}

//选中“运行模式的面板”
void vRunModePanel_Select(void){

	//1.该变量 = 0时，代表选中
	iSelectedIndex = 0;

	//2.对“运行模式面板的比特位”置1，对“光标”进行重绘
	extern EventGroupHandle_t xEventGroupForLCD;

	xEventGroupSetBits(xEventGroupForLCD, RUNMODE_PANEL_REPAINT_BIT);

}

//取消选中“运行模式的面板”
void vRunModePanel_DeSelect(void){
	//1.该变量 = -1时，代表取消选中
	iSelectedIndex = -1;

	//2.对“运行模式面板的比特位”置1，对“光标”进行重绘
	extern EventGroupHandle_t xEventGroupForLCD;

	xEventGroupSetBits(xEventGroupForLCD, RUNMODE_PANEL_REPAINT_BIT);
}

//调节“运行模式的面板”参数的值
//iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
void vRunModePanel_Adjust(int iDirection){

	//在未选中“信号发生器面板”时，不会改变任务参数
	if(iSelectedIndex < 0 )return;

	//1.参数1——运行模式，被选中时
	if(iSelectedIndex == 0){
		//公式解析：
		//ucRunMode只有0和1两个数值，因此要对1取余，否则超出编号范围
		//而“+2”是为了防止负数，超出编号范围
		ucRunMode = (ucRunMode + iDirection + 2) % 2;
	}
	//2.对“运行模式面板的比特位”置1，对“参数”进行重绘
	extern EventGroupHandle_t xEventGroupForLCD;

	xEventGroupSetBits(xEventGroupForLCD, RUNMODE_PANEL_REPAINT_BIT);
}





