#include "time.h"

//初始化定时器0的函数
void init_BFTM0()
{
	 NVIC_EnableIRQ(BFTM0_IRQn);

  /*  0.01秒触发一次定时器中断                                                        */
  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/100);
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
	BFTM_EnaCmd(HT_BFTM0, DISABLE);//关闭定时器0
}


