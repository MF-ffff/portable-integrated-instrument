#include "key.h"
#include <string.h>
#include "main.h"
#include "buzzer_task.h"
#include "panel_generator.h"
#include "panel_run_mode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

Key_HandleTypeDef key3; // KEY3的句柄

static GPIO_PinState KEY3_ReadPinCallback(void);
static void KEY3_ClickedCallback(void);

void Key3_Init(void)
{
	key3.Init.ReadPinCallback = KEY3_ReadPinCallback;
	key3.Init.ClickedCallback = KEY3_ClickedCallback;
	Key_Init(&key3);
}

void Key3_Scan(void)
{
	Key_Scan(&key3);
}

static GPIO_PinState KEY3_ReadPinCallback(void)
{
	return HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
}

static void KEY3_ClickedCallback(void)
{
	Buzzer_Beep();

	//按下KEY3时，选中“游标面板”
	vCursorPanel_Select();

	//按下KEY3时，取消选中“除游标以外的面板”
	vGeneratorPanel_DeSelect();
	vRunModePanel_DeSelect();
	vScalePanel_DeSelect();
}
