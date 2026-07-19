#include "key.h"
#include <string.h>
#include "main.h"
#include "buzzer_task.h"
#include "panel_generator.h"
#include "panel_run_mode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

Key_HandleTypeDef key2; // KEY2的句柄

static GPIO_PinState KEY2_ReadPinCallback(void);
static void KEY2_ClickedCallback(void);

void Key2_Init(void)
{
	key2.Init.ReadPinCallback = KEY2_ReadPinCallback;
	key2.Init.ClickedCallback = KEY2_ClickedCallback;
	Key_Init(&key2);
}

void Key2_Scan(void)
{
	Key_Scan(&key2);
}

static GPIO_PinState KEY2_ReadPinCallback(void)
{
	return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
}

static void KEY2_ClickedCallback(void)
{
	Buzzer_Beep();

	//按下KEY2时，选中“缩放面板”
	vScalePanel_Select();

	//按下KEY2时，取消选中“除缩放以外的面板”
	vGeneratorPanel_DeSelect();
	vRunModePanel_DeSelect();
	vCursorPanel_DeSelect();
}
