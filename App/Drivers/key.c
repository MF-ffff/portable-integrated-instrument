#include "key.h"


//负责按钮的初始化
void Key_Init(Key_HandleTypeDef *Handle){
	Handle->previous = GPIO_PIN_SET;
}

//负责按钮的硬件状态扫描
void Key_Scan(Key_HandleTypeDef *Handle){

	GPIO_PinState current;

	current = Handle->Init.ReadPinCallback();

	if(Handle->previous == GPIO_PIN_RESET && current == GPIO_PIN_SET){

		Handle->Init.ClickedCallback();

	}

	Handle->previous = current;

}
