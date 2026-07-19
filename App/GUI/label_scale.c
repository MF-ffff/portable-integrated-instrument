#include "label_scale.h"
#include "panel_scale.h"
#include "lcd.h"

//
//代表对“缩放标签”进行绘制
//
void ScaleLabel_Repaint(void){

	//1.暗黄色背景，坐标（187，6），宽60、高25，颜色#DAA400
	LCD_SetCursor(187, 6);
	LCD_SetBrush(0xDAA400);
	LCD_FillRect(60, 25);

	//2.打印参数1——X轴缩放比例，坐标（195，18），颜色#000000

		//获取x轴缩放比例
	float xScale = fGetXScale();

		//打印x轴缩放比例
	LCD_SetCursor(195, 18);
	LCD_SetPen(0x000000);
	LCD_PrintFloatSI(xScale, 5, "s");

	//3.打印参数2——Y轴缩放比例，坐标（195，29），颜色#000000

		//获取y轴缩放比例
	float yScale = fGetYScale();

		//打印y轴缩放比例
	LCD_SetCursor(195, 29);
	LCD_SetPen(0x000000);
	LCD_PrintFloatSI(yScale, 5, "V");
}


