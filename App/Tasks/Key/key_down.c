#include "key.h"
#include <string.h>
#include "main.h"
#include "buzzer_task.h"
#include "panel_generator.h"
#include "panel_run_mode.h"
#include "panel_scale.h"
#include "panel_cursor.h"

Key_HandleTypeDef keyDown; // KEY_DOWN的句柄

static GPIO_PinState KEYDOWN_ReadPinCallback(void);
static void KEYDOWN_ClickedCallback(void);

void KeyDown_Init(void)
{
	keyDown.Init.ReadPinCallback = KEYDOWN_ReadPinCallback;
	keyDown.Init.ClickedCallback = KEYDOWN_ClickedCallback;
	Key_Init(&keyDown);
}

void KeyDown_Scan(void)
{
	Key_Scan(&keyDown);
}

static GPIO_PinState KEYDOWN_ReadPinCallback(void)
{
	return HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
}

static void KEYDOWN_ClickedCallback(void)
{
	Buzzer_Beep();

	//按下KEY_DOWN时，调整参数，方向为-1；
	vGeneratorPanel_Adjust(-1);
	vRunModePanel_Adjust(-1);
	vScalePanel_Adjust(-1);
	vCursorPanel_Adjust(-1);

}
