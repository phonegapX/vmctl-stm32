#include "crc.h"


/*
 * ��������CRC_Init
 * ����  ��ʹ��CRCʱ��
 * ����  ����
 * ���  ����
 * ����  : �ⲿ����
 */
void CRC_Init(void)
{
	/* Enable CRC clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}
