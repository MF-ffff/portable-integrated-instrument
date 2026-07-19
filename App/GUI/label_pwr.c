#include "label_pwr.h"
#include "lcd.h"
#include "dmm.h"


//代表对“电源稳压模块的标签页”进行绘制
void PwrLabel_Repaint(void){

//1.绘制灰色矩形底框
	LCD_SetCursor(6, 215);	//设置光标位置
	LCD_SetBrush(0xcccccc);		//设置画刷颜色
	LCD_FillRect(90, 15);		//用画刷填充矩形

//2.打印“稳压电源模块的电压”：

		//获取万用表的数据
	DMMData_t xDmmData = xDMMGetData();

		//打印“电压”结果
	LCD_SetPen(0x000000);		//设置画笔颜色——黑色
	LCD_SetCursor(18, 226);		//设置光标位置
	LCD_PrintString("Pwr:");	//打印稳压电源电压值的前缀
	LCD_PrintFloatSI(xDmmData.fVPwr, 5, "V");	//打印最终的测量结果

}

