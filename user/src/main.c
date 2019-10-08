#include "stc8.h"
#include "config.h"
#include "event.h"
#include "uart.h"
#include <intrins.h>
#include <string.h>
#include "watermark.h"
#include "mainproc.h"

//extern u8 usart1_date,usart2_date;
/*
      1.0�汾
   1 ��ԭ�����Դ���1P30/P31�˿�ת��ΪP36/P37������GSM�����������ڣ�
   2 ɾ����һЩ���Ե���־
   3 ���Ӷ�ʱ����MCU����
*/

void main(void)
{
	EVNT_Handle evnt = 0;
	u8    last_rxd_stat = 0x00;
	u8	  last_rxd_change_cnt = 0x00;
	/* ��ʽִ�г��� */
	p_appc_all_init();

	/* ����������� */
	//p_uart1_send_string(__DATE__" "__TIME__" Compiled.");

	p_delay_10ms(100);

	while (1)
	{
		/* ��ֹ������¼�û�б�ѯ�ʵ� */
		evnt = evnt >= EVNT_MAX ? 0 : evnt + 1;

		/* �¼���ѯ */
		for (; evnt < EVNT_MAX; ++evnt)
		{
			if (p_envt_get_event(evnt))
			{
				p_envt_clear_event(evnt);
				break;
			}
		}

		/* �¼����� */
		switch (evnt)
		{
		case EVNT_INIT:
			break;
		case EVNT_WATERMARK:
			p_watermark_ctrl(g_config.m_enbale_sy1, g_config.m_enbale_sy2);
			if ((g_config.m_enbale_sy1 == 0) && (g_config.m_enbale_sy2 == 0))   // ��ֹ�����ϴλ��е�����
			{
				g_sy1_shark_cnt = 0;
				g_sy2_shark_cnt = 0;
			}
			evnt = 0;
			break;
		case EVNT_SLEEP:
			/* ˯������ */
			//p_uart1_send_string("OK2");
			p_delay_10ms(100);
			p_appc_task_sleep_proc(); // ˯�Ѻ���з���һ������
			//p_uart1_send_string("OK3");
			evnt = 0;                 // ���һ��Ҫ��0
			break;

		default:
			break;
		}

		/* ����1���մ�����Air������ */
		if (COM1.RX_TimeOut
			&& COM1.RX_TimeOut <= TimeOutSet2)
		{
			/* ��ֹ��û�н��յ����ݵ�ʱ���ظ����봦���� */
			COM1.RX_TimeOut = 0;
			/* ���������ݣ���������GSM�����ݣ� */
			p_proc_data_parser_from_air();
			memset(RX1_Buffer, 0, COM1.RX_Cnt);
			COM1.RX_Cnt = 0;
		}

		/* ����2���մ�����wifi������ */
		if (COM2.RX_TimeOut
			&& COM2.RX_TimeOut <= TimeOutSet2)
		{
			/* ��ֹ��û�н��յ����ݵ�ʱ���ظ����봦���� */
			COM2.RX_TimeOut = 0;
			/* ����������(���WIFI���ݺ�MCU��������) */
			p_proc_data_parser_from_loRa();
			memset(RX2_Buffer, 0, COM2.RX_Cnt + 1);
			COM2.RX_Cnt = 0;
			//p_uart2_send_string("OK2");
		}

		/* ��ʱ�������ݰ���GSM */
		if (g_heart_sign == heart_send)
		{
			p_proc_data_parser_from_loRa();
			memset(RX2_Buffer, 0, COM2.RX_Cnt + 1);
			COM2.RX_Cnt = 0;
			g_heart_sign = heart_clear;
		}

#if 0
		/* �ٽ��н���һ���޴���2������ֵʱ����MCU�����ݣ���һ��Э���ȥ���߼Ӹ���ʱ���� */
		if (usart1_date == 1)
		{
			/* ���������� */
			p_proc_data_parser_from_loRa();
			//p_uart1_send_string("OK1");
			usart1_date = 0;
		}

		/* ����һ����д�̼���ת�� */
		// �ж��Ƿ�Ҫ��������ģʽ���������ת����WIFI�Ĺ̼���Ŀǰ��ֱ��IO������ת�����У���1RX--��2RX��   
		//��������������ĺ���δ֪����
		if (IAP_RXD == last_rxd_stat)
		{
			last_rxd_stat = IAP_RXD;
			if (last_rxd_change_cnt++ > 10)
			{
				//EA = 0;
				disableInterrupts();
				WIFI_RST = 0;
				WIFI_SET = 0;
				WIFI_PU = 0;
				Delay1000ms();
				WIFI_PU = 1;
				WIFI_RST = 1;

				while (1)
				{
					WIFI_RXD = IAP_RXD;
					IAP_TXD = WIFI_TXD;
				}
			}
		}
#endif

	}
}


/*

void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 61;
	j = 225;
	k = 62;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

*/

