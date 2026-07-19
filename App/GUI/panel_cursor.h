#ifndef	GUI_PANEL_CURSOR_H
#define GUI_PANEL_CURSOR_H

#include "main.h"
#include "FreeRTOS.h"

//代表对“游标面板”进行绘制
void vRepaintCursor_Panel(void);

//选中“游标面板”
void vCursorPanel_Select(void);

//取消选中“游标面板”
void vCursorPanel_DeSelect(void);

//调节“游标面板”参数的值
//iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
void vCursorPanel_Adjust(int iDirection);


int8_t GetCursorX1Pct(void); // 获取游标X1的百分比
int8_t GetCursorX2Pct(void); // 获取游标X2的百分比
int8_t GetCursorY1Pct(void); // 获取游标Y1的百分比
int8_t GetCursorY2Pct(void); // 获取游标Y2的百分比



#endif
