#include "time.h"

//��ʼ����ʱ��0�ĺ���
void init_BFTM0()
{
	 NVIC_EnableIRQ(BFTM0_IRQn);

  /*  0.01�봥��һ�ζ�ʱ���ж�                                                        */
  BFTM_SetCompare(HT_BFTM0, SystemCoreClock/100);
  BFTM_SetCounter(HT_BFTM0, 0);
  BFTM_IntConfig(HT_BFTM0, ENABLE);
	BFTM_EnaCmd(HT_BFTM0, DISABLE);//�رն�ʱ��0
}


