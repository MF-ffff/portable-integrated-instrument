#ifndef	APP_WAVEFORM_GENERATOR_H_
#define	APP_WAVEFORM_GENERATOR_H_

#include "main.h"
#include "FreeRTOS.h"

void WaveformGenerator_SetShape(uint8_t Shape);				//用于设置产生的波形的形状——在“信号发生器面板”的参数调整处进行调用，便可根据根据参数1设置波形的形状
void WaveformGenerator_SetFrequency(uint32_t Frequency);	//用于设置产生的波形的频率——在“信号发生器面板”的参数调整处进行调用，便可根据根据参数2设置波形的频率

#endif
