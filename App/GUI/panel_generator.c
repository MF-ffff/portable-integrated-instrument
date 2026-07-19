#include "panel_generator.h"
#include "lcd.h"
#include "lcd_task.h"
#include "waveform_generator.h"

//声明一个变量，用于表示当前光标所在的位置：
//负数————表示未选中，0————表示选中参数1，1————表示选中参数2
static int iSelectedIndex = -1;

//声明一个变量，用于表示当前“信号发生器的波形”————即参数1
//具体可表示波形，见.h文件的枚举类型常量—
static	WaveformShape_t eWaveformShape = WaveformShape_Off;

//声明一个变量，用于表示当前“信号发生器的频率”————即参数2
//可选频率有：1-50kHZ等
static float fWaveformFrequency = 1.0e3f;	//默认为1kHZ

static const char* prvGetWaveformShapeString(WaveformShape_t eShape);


//
//代表对“信号发生器的面板”进行绘制
//
void vRepaintGenerator_Panel(void){

//A.绘制洋红色标题栏和浅灰色背景
	//1.绘制标题栏，坐标（260，184），宽60、高15，颜色#FF7BFF（粉色）
	LCD_SetBrush(0xFF7BFF);
	LCD_SetCursor(260, 184);
	LCD_FillRect(60, 15);

	//2.绘制标题文字，坐标（262, 197），颜色#000000（黑色）
	LCD_SetPen(0x000000);
	LCD_SetCursor(262, 197);
	LCD_PrintString("Generator");

	//3.绘制面板背景，坐标(260, 199)，宽60、高35，颜色#CCCCCC（灰色）
	LCD_SetBrush(0xCCCCCC);
	LCD_SetCursor(260, 199);
	LCD_FillRect(60, 35);

//B.绘制天蓝色光标(根据iSelectedIndex的值，选择绘制光标的地方)
	//参数1被选中：(262,202)，宽58，高10，颜色#00FFFF
	//参数2被选中：(262,212)，宽58，高10，颜色#00FFFF
	if(iSelectedIndex >=0 && iSelectedIndex <= 1){

		//把画刷颜色调整为天蓝色：
		LCD_SetBrush(0x00FFFF);

		//根据iSelectedIndex值调，选择光标所在参数位置
		if(iSelectedIndex == 0){
			LCD_SetCursor(262, 202); 	//选中参数1
		}
		else{
			LCD_SetCursor(262, 212); 	//选中参数2
		}

		LCD_FillRect(58, 10);
	}

//C.绘制参数的文字
	//1.绘制参数1的文字
		//先获取参数1需要打印的字符串
	const char *pcParameter_1_String = prvGetWaveformShapeString(eWaveformShape);

		//为参数1的文字选择背景填充
		//（区分：上面的背景填充是为了选择光标位置）
	if(iSelectedIndex == 0){
		LCD_SetBrush(0x00FFFF);	  //当选中了参数1时，则以天蓝色填充
	}
	else{
		LCD_SetBrush(0xCCCCCC);	  //当未选中参数1时，则以灰色填充
	}

		//绘制参数1的文字
	LCD_SetCursor(263, 212);
	LCD_PrintString(pcParameter_1_String);

	//2.绘制参数2的文字
		//为参数2的文字选择背景填充
		//（区分：上面的背景填充是为了选择光标位置）
	if(iSelectedIndex == 1){
		LCD_SetBrush(0x00FFFF);	  //当选中了参数2时，则以天蓝色填充
	}
	else{
		LCD_SetBrush(0xCCCCCC);	  //当未选中参数2时，则以灰色填充
	}
		//绘制参数2的文字
	LCD_SetCursor(263, 222);
	LCD_PrintFloatSI(fWaveformFrequency, 6, "Hz");

}

//
// 选中信号发生器面板 + 再次点击按钮时，切换光标位置
//
void vGeneratorPanel_Select(void){

	//1.完成光标的切换
	if(iSelectedIndex < 0){	//当前面板处于未选中状态
		iSelectedIndex = 0;
	}
	else{					//当前面板处于选中状态,切换光标位置
		iSelectedIndex ++ ;
		if(iSelectedIndex > 1){
			iSelectedIndex = 0;
		}
	}

	extern EventGroupHandle_t xEventGroupForLCD;

	//2.对“信号发生器面板的比特位”置1，重绘“光标的切换”
	xEventGroupSetBits(xEventGroupForLCD,GENERATOR_PANEL_REPAINT_BIT);

}


//
// 取消选中信号发生器面板
//
void vGeneratorPanel_DeSelect(void){

	if(iSelectedIndex >= 0){	//当前面板处于选中状态
		iSelectedIndex = -1;
	}

	extern EventGroupHandle_t xEventGroupForLCD;

	//2.对“信号发生器面板的比特位”置1，重绘“光标的切换”
	xEventGroupSetBits(xEventGroupForLCD,GENERATOR_PANEL_REPAINT_BIT);

}


//
// 调整参数的值
// iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
void vGeneratorPanel_Adjust(int iDirection){

	//在未选中“信号发生器面板”时，不会改变任务参数
	if(iSelectedIndex < 0 )return;

	//1.参数1——波形，被选中时
	if(iSelectedIndex == 0){

		//公式解析：
		//eWaveformShape是枚举常量，可以是0——3，共4个数值，因此要对4取余，否则超出编号范围
		//而“+4”是为了防止负数，超出编号范围
		eWaveformShape = (eWaveformShape - iDirection + 4) % 4;

		//实际修改波形形状
		WaveformGenerator_SetShape(eWaveformShape);
	}

	//2.参数2——频率，被选中时
	if(iSelectedIndex == 1){

		//声明一个数组，保存频率的参数选项
		static const float freqs[] = {50.0f, 100.0f, 200.0f, 500.0f, 1.0e3f, 2.0e3f, 5.0e3f, 10.0e3f, 50.0e3f};

		//声明一个整型变量，得到数组的元素个数
		//公式解析：
		//数组的长度/数组中每个元素的长度 = 数组的元素个数
		static const int numof_Freqs = sizeof(freqs)/sizeof(float);

		// 通过一个for循环，定位到当前频率在数组当中的序号
		int i = 0;
		for(i = 0;i<numof_Freqs;i++){

			if(freqs[i] == fWaveformFrequency){
				break;
			}

		}


		//计算下一个频率的序号
		i = i + iDirection;

		//考虑序号的特殊情况（同参数1，考虑上一次为最大档和最低档时，怎么处理）
		if(i >= numof_Freqs){	 //若i在上一次为最大值，往下推时，我想让他变成最低档
			i = 0;
		}
		if(i < 0){	//若i在上一次为0，往上推时，我想让他变成最大档
			i = numof_Freqs-1;
		}

		//由序号，获取当前频率
		fWaveformFrequency = freqs[i];

		//实际修改波形频率
		WaveformGenerator_SetFrequency(fWaveformFrequency);
	}


	//3.更改完参数后，进行重绘
	extern EventGroupHandle_t xEventGroupForLCD;

	xEventGroupSetBits(xEventGroupForLCD,GENERATOR_PANEL_REPAINT_BIT);

}



//
//根据波形的值，获取对应的字符串名称
//
static const char* prvGetWaveformShapeString(WaveformShape_t eShape){

	const char *pcString = "Unkown";

	if(eShape == WaveformShape_Off){
		pcString = "OFF";	//关闭波形输出
	}
	else if(eShape == WaveformShape_Sin){
		pcString = "Sin";	//正弦波
	}
	else if(eShape == WaveformShape_Square){
		pcString = "Square";	//方波
	}
	else if(eShape == WaveformShape_Triangle){
		pcString = "Triangle";	//三角波
	}

	return pcString;

}




