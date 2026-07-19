#ifndef	GUI_PANEL_SCALE_H
#define GUI_PANEL_SCALE_H

#include "main.h"
#include "FreeRTOS.h"

//代表对“缩放面板”进行绘制
void vRepaintScale_Panel(void);

//选中“缩放面板”
void vScalePanel_Select(void);

//取消选中“缩放面板”
void vScalePanel_DeSelect(void);

//调节缩放面板”参数的值
//iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
void vScalePanel_Adjust(int iDirection);


//获取X轴的缩放比例————给“缩放标签的程序进行调用”
float fGetXScale(void);

//获取Y轴的缩放比例————给“缩放标签的程序进行调用”
float fGetYScale(void);



#endif
