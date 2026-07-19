#ifndef APP_WAVEFORM_CAPTURE_H_
#define APP_WAVEFORM_CAPTURE_H_

#include "main.h"
#include "FreeRTOS.h"

//初始化波形捕捉器
void WaveformCapture_Init(void);

//以数组形式获取波形数据，用于波形面板文件进行绘制
float *WaveformCapture_GetWaveform(void);

//设置波形捕捉器的采样率（即设计TIM3的时基单元）
//PSC——预分频器的值，ARR——自动重装寄存器的值
void WaveformCapture_SetSampleRate(uint16_t PSC,uint16_t ARR);

//“获取”波形捕捉器的采样率，用于波形面板文件进行绘制
uint32_t WaveformCapture_GetSampleRate(void);

//初始化波形发生器
void WaveformGenerator_Init(void);

#endif
