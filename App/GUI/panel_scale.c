#include "panel_scale.h"
#include "lcd.h"
#include "lcd_task.h"
#include "label_cursor.h"
#include "waveform_capture.h"

//声明一个变量，用于表示当前光标所在的位置：
//负数————表示未选中，0————表示选中参数1(x轴的缩放比)，1——表示选中参数2（y轴的缩放比）
static int iSelectedIndex = -1;

//
//声明一个数组，用于表示当前“x轴的缩放比”的档位
//不直接记录X轴缩放比的大小，而是记录其对应挡位的序号
//因为这样等等在“调节参数的函数”中，就可以通过简易的调节序号，来调节参数了
static const float xScales[] = {10.0e-6f, 20.0e-6f, 50.0e-6f, 100.0e-6f, 200.0e-6f, 500.0e-6f, 1.0e-3f, 2.0e-3f, 5.0e-3f, 10.0e-3f, 20.0e-3f, 50.0e-3f};
static int16_t xScaleIndex = 5;		//当前档位的序号
static const uint16_t numOfXScales = sizeof(xScales)/sizeof(float);			//获取x轴的缩放比，共有几个选项

//
//与x轴的缩放比同理——：声明一个数组，用于表示当前“y轴的缩放比”的档位
//不直接记录Y轴缩放比的大小，而是记录其对应挡位的序号
static const float yScales[] = { 50.0e-3f, 100.0e-3f, 200.0e-3f, 500.0e-3f, 1.0f };
static int16_t yScaleIndex = 4; // 当前挡位的序号
static const uint16_t numOfYScales = sizeof(yScales) / sizeof(float); 		//获取y轴的缩放比，共有几个选项

//
//代表对“缩放面板”进行绘制
//
void vRepaintScale_Panel(void){

//A.绘制洋红色标题栏和浅灰色背景
	//1.绘制标题栏，坐标（260，67），宽60、高15，颜色#FF7BFF（粉色）
	LCD_SetBrush(0xFF7BFF);
	LCD_SetCursor(260, 67);
	LCD_FillRect(60, 15);

	//2.绘制标题文字，坐标（262, 82），颜色#000000（黑色）
	LCD_SetPen(0x000000);
	LCD_SetCursor(262, 82);
	LCD_PrintString("Scale");

	//3.绘制面板背景，坐标(260, 82)，宽60、高35，颜色#CCCCCC（灰色）
	LCD_SetBrush(0xCCCCCC);
	LCD_SetCursor(260, 82);
	LCD_FillRect(60, 35);

//B.绘制天蓝色光标(根据iSelectedIndex的值，选择绘制光标的地方)
	//参数1被选中：(274,89)，宽9，高11，颜色#00FFFF
	//参数2被选中：(294,89)，宽9，高11，颜色#00FFFF
	if(iSelectedIndex >=0 && iSelectedIndex <= 1){

		//把画刷颜色调整为天蓝色：
		LCD_SetBrush(0x00FFFF);

		//根据iSelectedIndex值调，选择光标所在参数位置
		if(iSelectedIndex == 0){
			LCD_SetCursor(274,89); 	//选中参数1
		}
		else{
			LCD_SetCursor(294,89); 	//选中参数2
		}

		LCD_FillRect(9, 11);
	}

//C.绘制参数的文字
	//1.绘制参数1的文字
		//为参数1的文字选择背景填充
		//（区分：上面的背景填充是为了选择光标位置）
	if(iSelectedIndex == 0){
		LCD_SetBrush(0x00FFFF);	  //当选中了参数1时，则以天蓝色填充
	}
	else{
		LCD_SetBrush(0xCCCCCC);	  //当未选中参数1时，则以灰色填充
	}

		//绘制参数1的文字
	LCD_SetCursor(275, 98);
	LCD_PrintString("x");

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
	LCD_SetCursor(295, 98);
	LCD_PrintString("y");


}

//
//选中“缩放面板” + 再次点击按钮时，切换光标位置
//
void vScalePanel_Select(void){

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
	xEventGroupSetBits(xEventGroupForLCD,SCALE_PANEL_REPAINT_BIT);

}

//
//取消选中“缩放面板”
//
void vScalePanel_DeSelect(void){

	if(iSelectedIndex >= 0){	//当前面板处于选中状态
		iSelectedIndex = -1;
	}

	extern EventGroupHandle_t xEventGroupForLCD;

	//2.对“信号发生器面板的比特位”置1，重绘“光标的切换”
	xEventGroupSetBits(xEventGroupForLCD,SCALE_PANEL_REPAINT_BIT);


}

//
//调节缩放面板”参数的值
//iDirection表示数值调整的方向，+1代表向上调整，-1代表向下调整
//
void vScalePanel_Adjust(int iDirection){
	//在未选中“信号发生器面板”时，不会改变任务参数
	if(iSelectedIndex < 0 )return;

	//1.参数1——x轴的缩放比例，被选中时
	if(iSelectedIndex == 0){

		//公式解析：
		//分子：当前的序号 + 按钮“改变的序号” + 数组总共的元素个数（防止为0，超出序号范围）
		//分母：对数组总共的元素个数取余，刚好范围就是“数组总共的元素个数-1”，也是保证不超出范围
		xScaleIndex = (xScaleIndex + iDirection + numOfXScales)%numOfXScales;

		static const uint16_t PSCs[] = {0,0,0,0,0,0,0,0,0,0,0,0};
		static const uint16_t ARRs[] = {143, 143, 143, 143, 225, 561, 1124, 2251, 5624, 11249, 22549, 56249};

		// 根据xScale的值设置采样率
		WaveformCapture_SetSampleRate(PSCs[xScaleIndex], ARRs[xScaleIndex]);
	}

	//2.参数2——y轴的缩放比例，被选中时
	if(iSelectedIndex == 1){

		//因为和参数1采用同样的方式保存参数
		//因此，公式也是一样的
		yScaleIndex = (yScaleIndex + iDirection + numOfYScales)%numOfYScales;

		}

	//3.更改完参数后，进行重绘
	extern EventGroupHandle_t xEventGroupForLCD;

	xEventGroupSetBits(xEventGroupForLCD,SCALE_PANEL_REPAINT_BIT|SCALE_LABEL_REPAINT_BIT|WAVEFORM_PANEL_REPAINT_BIT|CURSOR_LABEL_REPAINT_BIT);


}

//获取X轴的缩放比例————给“缩放标签的程序进行调用”
float fGetXScale(void){
	return xScales[xScaleIndex];
}

//获取Y轴的缩放比例————给“缩放标签的程序进行调用”
float fGetYScale(void){
	return yScales[yScaleIndex];
}


