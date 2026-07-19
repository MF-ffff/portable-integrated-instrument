#include "key_task.h"
#include "key.h"
#include "key1.h"
#include "buzzer_task.h"
#include "string.h"
#include "panel_generator.h"
#include "panel_run_mode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

Key_HandleTypeDef key1; // KEY1的句柄

static GPIO_PinState KEY1_ReadPinCallback(void);
static void KEY1_ClickedCallback(void);

void Key1_Init(void)
{
	key1.Init.ReadPinCallback = KEY1_ReadPinCallback;
	key1.Init.ClickedCallback = KEY1_ClickedCallback;
	Key_Init(&key1);
}

void Key1_Scan(void)
{
	Key_Scan(&key1);
}

static void KEY1_ClickedCallback(void)
{
	Buzzer_Beep();

	//按下KEY1时，选中“运行模式面板”
	vRunModePanel_Select();

	//按下KEY1时，取消选中“除运行模式外的面板”
	vGeneratorPanel_DeSelect();
	vScalePanel_DeSelect();
	vCursorPanel_DeSelect();

}

static GPIO_PinState KEY1_ReadPinCallback(void)
{
	return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
}


