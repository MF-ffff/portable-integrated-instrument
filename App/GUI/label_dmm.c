#include "label_dmm.h"
#include "lcd.h"
#include "dmm.h"

//代表对“数字万用表的标签页”进行绘制
void DMMLabel_Repaint(void){

//1.绘制灰色矩形底框
	LCD_SetCursor(110, 215);	//设置光标位置
	LCD_SetBrush(0xcccccc);		//设置画刷颜色
	LCD_FillRect(120, 15);		//用画刷填充矩形

//2.打印“档位信息与测量结果”：

	//获取万用表的数据
	DMMData_t xDmmData = xDMMGetData();

	//2.1打印档位信息

		//设置要打印的档位信息
	const char *pccRangeStrs[] = {"DMM--2V:", "DMM-10V:", "DMM-50V:", "DMM--1k:", "DMM-10k:", "DMM100k:", "DMM--1M:"};

		//设置光标位置和画笔颜色
	LCD_SetCursor(116, 226);
	LCD_SetPen(0x000000);

		//提取出“万用表数据中的”档位信息，并打印
	const char *pcRangeStr = pccRangeStrs[xDmmData.eDMMRange];
	LCD_PrintString(pcRangeStr);

	//2.2打印万用表的最终测量结果

		//确定万用表的单位（由档位信息确定）
	const char *pcUnit;

	if(xDmmData.eDMMRange < eDMMRange1kOhm){
		pcUnit = "V";
	}
	else{
		pcUnit = "R";
	}
		//打印最终的测量结果
	LCD_PrintFloatSI(xDmmData.fDMMValue, 6, pcUnit);
}
