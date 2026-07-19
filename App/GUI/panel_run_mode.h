#ifndef	GUI_PANEL_RUN_MODE_H
#define GUI_PANEL_RUN_MODE_H

#include "main.h"
#include "FreeRTOS.h"

//代表对“运行模式的面板”进行绘制
void vRepaintRunMode_Panel(void);

//选中“运行模式的面板”
void vRunModePanel_Select(void);

//取消选中“运行模式的面板”
void vRunModePanel_DeSelect(void);

//调节“运行模式的面板”参数的值
//iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
void vRunModePanel_Adjust(int iDirection);

#endif
