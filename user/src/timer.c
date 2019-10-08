#include "stc8.h"
#include "intrins.h"
#include "config.h"
#include "event.h"
#include "wdg.h"

u32		g_time_ms = 0x00;
u8 g_heart_beat, g_heart_sign;

static uint8_t s_evnt_cnt = 0;



void p_delay_10ms(uint32_t ms)
{
	ms += my_time();
	while (ms > my_time())
	{

	}
}


void p_sysclk_init(void)
{
	/*ѡ���ڲ�24MHz�߾���IRC*/
	P_SW2 = 0x80;
	CKSEL = 0x00;                               //ѡ���ڲ�IRC ( Ĭ�� )
	CLKDIV = 0x02;								// 2��Ƶ 24MHz/2 = 12Mhz
	P_SW2 = 0x00;
}


/**
  * ***********************************************************************
  * @brief	��ʱ����ʼ��
  *
  *	@param  :
  *
  * @retval void:
  *
  * @attention	: none
  * ***********************************************************************
  */
void p_timer_init(void)
{
	disableInterrupts();

	/* Timer0 ��ʼ�� ʱ���10ms */
	AUXR &= 0x7F;		// ��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		// ���ö�ʱ��ģʽ
	TL0 = 0xF0;			// ���ö�ʱ��ֵ
	TH0 = 0xD8;			// ���ö�ʱ��ֵ
	TF0 = 0;			// ���TF0��־
	TR0 = 1;			// ��ʱ��0��ʼ��ʱ

	/* ʹ�ܶ�ʱ���ж� */
	ET0 = 1;	                   // ʹ�ܶ�ʱ��0�ж�
	enableInterrupts();
}

#if 0
void p_timer_uninit(void)
{
	/* Timer0 ��ʼ�� ʱ���1ms */
	TR0 = 0;		               // ��ʱ��0��ʼ��ʱ

	/* ʹ�ܶ�ʱ���ж� */
	ET0 = 0;	                   // ʹ�ܶ�ʱ��0�ж�
}
#endif

/* ��ʱ��0 ʱ��� */
void TM0_Isr() interrupt 1 using 3
{
	/* 10ms */
	++g_time_ms;
	++s_evnt_cnt;
	s_evnt_cnt %= 100;

	/* ��ʱ������ */
	++g_heart_beat;
	if (g_heart_beat == heart_time)
	{
		g_heart_sign = heart_send;
		g_heart_beat = 0;
	}
		

	/* ι�� */
	//WDG_RELOAD();

	/* ���ڳ�ʱ��� */
	if (COM2.RX_TimeOut > TimeOutSet2)
	{
		--COM2.RX_TimeOut;
	}

	if (COM1.RX_TimeOut > TimeOutSet2)
	{
		--COM1.RX_TimeOut;

		/* Ϊ�˷�ֹ������WiFi�ڷ������ݵ�ʱ�򲻽��������ϱ� */
		s_evnt_cnt = 0;
	}

	
}
