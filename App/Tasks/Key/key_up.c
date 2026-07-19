#include "key.h"
#include <string.h>
#include "main.h"
#include "buzzer_task.h"
#include "panel_generator.h"
#include "panel_run_mode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

Key_HandleTypeDef keyUp;   // KEY_UP的句柄

static GPIO_PinState KEYUP_ReadPinCallback(void);
static void KEYUP_ClickedCallback(void);

void KeyUp_Init(void)
{
	keyUp.Init.ReadPinCallback = KEYUP_ReadPinCallback;
	keyUp.Init.ClickedCallback = KEYUP_ClickedCallback;
	Key_Init(&keyUp);
}

void KeyUp_Scan(void)
{
	Key_Scan(&keyUp);
}

static GPIO_PinState KEYUP_ReadPinCallback(void)
{
	return HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
}

static void KEYUP_ClickedCallback(void)
{
	Buzzer_Beep();

	//按下KEY_UP时，调整参数，方向为+1；
	vGeneratorPanel_Adjust(+1);
	vRunModePanel_Adjust(+1);
	vScalePanel_Adjust(+1);
	vCursorPanel_Adjust(+1);
}
