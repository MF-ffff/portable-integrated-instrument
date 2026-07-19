#include "waveform_generator.h"
#include "math.h"
#define PI 3.14159265358979f // 圆周率

static uint16_t waveform[1000];		//声明一个数组，使用1000个采样点来保存单个周期的波形数据

static uint8_t shape = 0;			//声明一个变量，代表产生的波形的形状，0——OFF，1——正弦波，2——三角波，3——方波
static uint32_t frequency = 1.0e3;	//声明一个变量，代表产生的波形的频率

static uint16_t waveformLength;		//波形的实际长度（即：实际采样点数）

//根据shape和frequency的值，实际修改采样率和波形形状
static void AutoSampleRate(void);	//根据产生的波形频率，修改采样率
static void PrepareWaveformData(void);	//根据Shape的值，实际修改波形形状

static void RestartDAC(void);		//负责：更新采样率 + 将波形数据发送到DAC + 重启DAC


//
//用于设置产生的波形的形状——在“信号发生器面板”的参数调整处进行调用，便可根据根据参数1设置波形的形状
//0——OFF，1——正弦波，2——三角波，3——方波
void WaveformGenerator_SetShape(uint8_t Shape){
	shape = Shape;
	RestartDAC();
}

//
//用于设置产生的波形的频率——在“信号发生器面板”的参数调整处进行调用，便可根据根据参数2设置波形的频率
//
void WaveformGenerator_SetFrequency(uint32_t Frequency){
	frequency = Frequency;
	RestartDAC();
}

//
//根据产生的波形频率，修改采样率
//采样率 = 采样点总数（1000个点） * 波形频率
static void AutoSampleRate(void){
	uint32_t sampleRate;

	sampleRate = 1000 * frequency;

	//当采样率过大时，取最大值 + 纠正此时的波形实际采样点数（实际的采样点数 = 采样率 / 信号频率）

	waveformLength = 1000;

	if(sampleRate > 500e3){

		//采样率过大时，取最大值
		sampleRate = 500e3;

		//纠正此时的波形实际采样点数（实际的采样点数 = 采样率 / 信号频率）
		waveformLength = sampleRate / frequency;
	}

	//根据采样率，反推出TIM5的PSC和ARR的值，从而在硬件层面成功修改采样率
	uint16_t psc = 0;
	uint16_t arr = 72e6 / sampleRate -1 ;

	extern TIM_HandleTypeDef htim5;
	__HAL_TIM_SET_PRESCALER(&htim5, psc); // 设置PSC的值
	__HAL_TIM_SET_AUTORELOAD(&htim5, arr); // 设置ARR的值

}

//
//根据Shape的值，实际修改波形形状
//
static void PrepareWaveformData(void){

	if(shape == 0) // OFF挡，输出0V电压
		{
			for(int i=0; i<waveformLength; i++)
			{
				waveform[i] = 1241;
			}
		}

		else if(shape == 1) // Sin挡，输出正弦波
		{
			for(int i=0; i<waveformLength; i++)
			{
				// 计算DAC的输出电压
				float volt = 1.0f + sinf(((float)i/waveformLength)*2*PI);
				// 将电压转换为0~4095之间的数字
				waveform[i] = roundf(volt / 3.3f * 4095);
			}
		}

		else if(shape == 2) // Triangle挡，输出三角波
		{
			for(int i=0; i<waveformLength; i++)
			{
				// 计算DAC的输出电压
				float volt = 1.0f + ((float)i/waveformLength) * (2.0 - 1.0f);
				// 将电压转换为0~4095之间的数字
				waveform[i] = roundf(volt / 3.3f * 4095);

			}
		}

		else if(shape == 3) // Square挡，输出方波
		{
			for(int i=0; i<waveformLength; i++)
			{
				if(i<waveformLength / 2)
				{
					waveform[i] = 2482;
				}
				else
				{
					waveform[i] = 1241;
				}
			}
		}
}

//
//负责：更新采样率 + 将波形数据发送到DAC + 重启DAC;
//在修改了“采样率和波形形状”后，进行调用，以便重绘
static void RestartDAC(void){

	extern DAC_HandleTypeDef hdac;
	extern TIM_HandleTypeDef htim5;

	// 先停止定时器和DAC
	HAL_TIM_Base_Stop(&htim5);
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);

	AutoSampleRate(); // 调整采样率
	PrepareWaveformData(); // 准备波形数据

	// 开启DAC
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (const uint32_t)waveform, waveformLength, DAC_ALIGN_12B_R);
	// 开启TIM5
	HAL_TIM_Base_Start(&htim5);
}

// @作用：初始化波形发生器
void WaveformGenerator_Init(void)
{
	RestartDAC();  // 重新加载DAC波形
}
