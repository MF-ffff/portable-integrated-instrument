#include "label_cursor.h"
#include "panel_cursor.h"
#include "panel_scale.h"
#include "lcd.h"

static float LCD_GetXCursorValue(int8_t xPercent, float xScale);
static float LCD_GetYCursorValue(int8_t yPercent, float yScale);

//
//代表对“缩放标签”进行绘制
//
void CursorLabel_Repaint(void){

//A.先获取要绘制的内容
	float xScale = fGetXScale(); // X轴向缩放比
	float yScale = fGetYScale(); // Y轴向缩放比

	int8_t cursorX1Pct = GetCursorX1Pct(); // 游标X1的百分比值
	int8_t cursorX2Pct = GetCursorX2Pct(); // 游标X2的百分比值
	int8_t cursorY1Pct = GetCursorY1Pct(); // 游标Y1的百分比值
	int8_t cursorY2Pct = GetCursorY2Pct(); // 游标Y2的百分比值

		//根据“百分比值 + 缩放比”，得到游标真正的数值
	float cursorX1 = LCD_GetXCursorValue(cursorX1Pct, xScale); // 游标X1的值
	float cursorX2 = LCD_GetXCursorValue(cursorX2Pct, xScale); // 游标X2的值
	float cursorY1 = LCD_GetYCursorValue(cursorY1Pct, yScale); // 游标Y1的值
	float cursorY2 = LCD_GetYCursorValue(cursorY2Pct, yScale); // 游标Y2的值

		//最下方需要绘制的差值
	float deltaX = cursorX2 - cursorX1; // 游标X的差值
	float deltaY = cursorY2 - cursorY1; // 游标Y的差值

//B.直接开始绘制

	//1.天蓝色背景，（8，6），宽174、高10，颜色#7BFFFF
		LCD_SetCursor(8, 6);
		LCD_SetBrush(0x7BFFFF);
		LCD_FillRect(174, 10);

	// 2. 游标X1，（13，15）
		LCD_SetCursor(13,15);
		LCD_PrintString("X1:");
		LCD_PrintFloatSI(cursorX1, 5, "s");

	// 3. 游标Y1，（99，15）
		LCD_SetCursor(99, 15);
		LCD_PrintString("Y1:");
		LCD_PrintFloatSI(cursorY1, 5, "V");

	// 4. 洋红色背景，（8，21），宽174、高10，颜色#FF7BFF
		LCD_SetCursor(8, 21);
		LCD_SetBrush(0xFF7BFF);
		LCD_FillRect(174, 10);

	// 5. 游标X2，（13，30）
		LCD_SetCursor(13,30);
		LCD_PrintString("X2:");
		LCD_PrintFloatSI(cursorX2, 5, "s");

	// 6. 游标Y2，（99，30）
		LCD_SetCursor(99, 30);
		LCD_PrintString("Y2:");
		LCD_PrintFloatSI(cursorY2, 5, "V");

	// 7. 白色背景，（8，32），宽174、高10，颜色#FFFFFF
		LCD_SetCursor(8,32);
		LCD_SetBrush(0xFFFFFF);
		LCD_FillRect(174, 10);

	// 8. ΔX，（34，42）
		LCD_SetCursor(34, 42);
		LCD_PrintFloatSI(deltaX, 5, "s");

	// 9. ΔY，（120，42）
		LCD_SetCursor(120, 42);
		LCD_PrintFloatSI(deltaY, 5, "V");

}

//
//函数名：LCD_GetXCursorValue
//功能：X游标百分比 → 真实时间值(秒)
//参数：xPercent - 百分比(0~100)
//      xScale - X轴缩放比(秒/格)
//返回：真实时间值
float LCD_GetXCursorValue(int8_t xPercent, float xScale)
{
    // 公式：百分比 → 0~10格对应的时间
    return (xPercent / 100.0f) * xScale * 10.0f;
}

//
//函数名：LCD_GetYCursorValue
//功能：Y游标百分比 → 真实电压值(伏)
//参数：yPercent - 百分比(-100~100)
//      yScale - Y轴缩放比(伏/格)
//返回：真实电压值
float LCD_GetYCursorValue(int8_t yPercent, float yScale)
{
    // 公式：百分比 → -5格 ~ +5格对应的电压
    return (yPercent / 100.0f) * yScale * 5.0f;
}


