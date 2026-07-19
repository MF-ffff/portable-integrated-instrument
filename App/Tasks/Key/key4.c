#include "key.h"
#include <string.h>
#include "main.h"
#include "buzzer_task.h"
#include "panel_generator.h"
#include "panel_run_mode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

Key_HandleTypeDef key4; // KEY4的句柄

static GPIO_PinState KEY4_ReadPinCallback(void);
static void KEY4_ClickedCallback(void);

void Key4_Init(void)
{
	key4.Init.ReadPinCallback = KEY4_ReadPinCallback;
	key4.Init.ClickedCallback = KEY4_ClickedCallback;
	Key_Init(&key4);
}

void Key4_Scan(void)
{
	Key_Scan(&key4);
}

static GPIO_PinState KEY4_ReadPinCallback(void)
{
	return HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
}

static void KEY4_ClickedCallback(void)
{
	Buzzer_Beep();

	//按下KEY4时，对“信号发生器”面板进行选中
	vGeneratorPanel_Select();

	//按下KEY4时，取消选中“除信号发生器的面板”
	vRunModePanel_DeSelect();
	vScalePanel_DeSelect();
	vCursorPanel_DeSelect();
}
