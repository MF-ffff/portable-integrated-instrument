#ifndef	GUI_PANEL_GENERATOR_H_
#define	GUI_PANEL_GENERATOR_H_

#include "main.h"
#include "FreeRTOS.h"

//声明一组枚举类型的常量，用于表示波形
typedef enum{
	WaveformShape_Off,			//关闭波形输出
	WaveformShape_Sin,			//正弦波
	WaveformShape_Triangle,		//三角波
	WaveformShape_Square,		//方波
}WaveformShape_t;

//代表对“信号发生器的面板”进行绘制
void vRepaintGenerator_Panel(void);

//选中“信号发生器的面板”
void vGeneratorPanel_Select(void);

//取消选中“信号发生器的面板”
void vGeneratorPanel_DeSelect(void);

//调节“信号发生器的面板”参数的值
//iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
void vGeneratorPanel_Adjust(int iDirection);

#endif
