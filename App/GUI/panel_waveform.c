#include "panel_waveform.h"
#include "panel_cursor.h"
#include "panel_scale.h"
#include "lcd.h"
#include "waveform_capture.h"
#include "dmm.h"
#include "semphr.h"

//
//代表对“示波器面板”进行绘制
//
void vRepaintWaveform_Panel(void){
	WaveformParamTypeDef waveformParam = {0};

	extern SemaphoreHandle_t xMutexForWaveform;

	//在用数组“waveform”画图前，获取锁；
	//访问完数组“waveform”后，释放锁；
	xSemaphoreTake(xMutexForWaveform,portMAX_DELAY);
	waveformParam.Waveform = WaveformCapture_GetWaveform();
	xSemaphoreGive(xMutexForWaveform);

	waveformParam.WaveLength = 1024;
	waveformParam.SampleRate = WaveformCapture_GetSampleRate();
	waveformParam.Trigger = xDMMGetData().fTriggerLevel;

	waveformParam.XScale= fGetXScale();
	waveformParam.YScale = fGetYScale();

	waveformParam.CursorX1 = GetCursorX1Pct();
	waveformParam.CursorX2 = GetCursorX2Pct();
	waveformParam.CursorY1 = GetCursorY1Pct();
	waveformParam.CursorY2 = GetCursorY2Pct();

	LCD_DrawWaveform(&waveformParam);
}


