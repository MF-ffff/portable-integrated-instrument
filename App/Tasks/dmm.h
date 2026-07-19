#ifndef TASKS_DMM_H_
#define TASKS_DMM_H_

#include "main.h"
#include "FreeRTOS.h"

//万用表档位判断所需的结构体
typedef enum
{
	eDMMRange2V,      // 2V电压挡
	eDMMRange10V,     // 10V电压挡
	eDMMRange50V,     // 50V电压挡
	eDMMRange1kOhm,   // 1k电阻档
	eDMMRange10kOhm,  // 10k电阻档
	eDMMRange100kOhm, // 100k电阻档
	eDMMRange1MOhm,   // 1M电阻档
}DMMRange_t;

//万用表最终测量结果所需的结构体
typedef struct
{
	DMMRange_t eDMMRange;	//万用表的档位
	float fDMMValue;		//万用表的最终测量结果
	float fVPwr;			//稳压电源模块的电压
	float fTriggerLevel;	//触发电压的大小
}DMMData_t;

void vDMMInit(void);	//初始化DMM模块
DMMData_t xDMMGetData(void);	//获取万用表的最终测量结果

#endif
