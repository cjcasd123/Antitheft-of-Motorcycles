#include "uart.h"
#include "stc8.h"
#include "intrins.h"
#include "config.h"
#include <string.h>

/*
     ����1 -- GSM   P36 P37
	 ����2 -- WIFI  P10 P11
*/

/* �ж��Ƿ���2�ı��� */
#define IsNotPowerOF2(x)			((x)&(x-1))
//u8 usart1_date,usart2_date;

// NB�� Timer 1 -> ����1	   115200    GSM
void Uart1Isr() interrupt 4 using 1
{
	if (TI)
	{
		TI = 0;
#if enableUart1SendInterrupt
		if (COM1.B_TX_busy == TRUE && COM1.TX_read != COM1.TX_write)
		{
			SBUF = TX1_Buffer[COM1.TX_read];
			++COM1.TX_read;
		}
		else {
			COM1.B_TX_busy = FALSE;
		}
#else
		COM1.B_TX_busy = FALSE;
#endif 
	}

	if (RI)
	{
		RI = 0;

		do {
			/* �ж��Ƿ��Ѿ�������� <==> lock */
			if (COM1.RX_Cnt
				&& COM1.RX_TimeOut <= TimeOutSet2)
			{
				break;
			}

			/* �������� */
			if (COM1.RX_Cnt < COM_RX1_Lenth - 1)
			{
				RX1_Buffer[COM1.RX_Cnt++] = SBUF;
				//usart1_date = SBUF;
			}
			COM1.RX_TimeOut = TimeOutSet1;
		} while (0);
	}
}

void p_uart1_init(void)
{
	AIR_TX = 1;
	AIR_RX = 1;
	PORT_MODE_SET(AIR_RX_G, AIR_RX_P, PORT_MODE_AD);

	/* ѡ�񴮿�һ�Ĺ�������Ϊ RxD:P3.6  TxD:P3.7 */
    P_SW1 = 0x40;
	//disableUartInterrupts();
	SCON = 0x50;		                // 8λ���ݣ��ɱ䲨����
	AUXR |= 0x40;		                // ��ʱ��1ʱ��1ΪFOSC,��1T
	AUXR &= 0xFE;		                // ����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		                // �趨��ʱ��1����ģʽ0 Ϊ16λ�Զ���װ��ʽ
	TL1 = (65536 - (FOSC / 4 / BAUD));     // ���ö�ʱ��ֵ ��������12Ϊ����
	TH1 = (65536 - (FOSC / 4 / BAUD)) >> 8;   // ���ö�ʱ��ֵ
	TR1 = 1;		                    // ������ʱ��1

	enableUartInterrupts();
}

void p_uart1_send_byte(char dat)
{
#if enableUart1SendInterrupt
	if (COM1.TX_read != COM1.TX_write + 1)
	{
		TX1_Buffer[COM1.TX_write] = dat;	//װ���ͻ���
		++COM1.TX_write;
	}

	if (COM1.B_TX_busy == FALSE)		//����
	{
		COM1.B_TX_busy = TRUE;		    //��־æ
		SBUF = dat;
		++COM1.TX_read;
	}
#else
	while (COM1.B_TX_busy);
	COM1.B_TX_busy = TRUE;
	SBUF = dat;
#endif
}

/**
  * ***********************************************************************
  * @brief	ͨ������1��������
  *
  *	@param  p: ��Ҫ���͵�����
  *	@param  length: ���͵����ݳ���
  *
  * @retval void:
  *
  * @attention	: none
  * ***********************************************************************
  */
void p_uart1_send_data(char *p, uint16_t length)
{
	uint8_t i = 0;

	for (i = 0; i < length; i++)
	{
		p_uart1_send_byte(p[i]);
	}
}

void p_uart1_send_string(char *p)
{
	while (*p)
	{
		p_uart1_send_byte(*p);
		++p;
	}
}

//BLE��Timer 2 -> ����2   115200    WIFI
void Uart2Isr() interrupt 8 using 1
{
	/* ���� */
	if (S2CON & 0x01)
	{
		S2CON &= ~0x01;

		RX2_Buffer[COM2.RX_Cnt++] = S2BUF;
		//usart2_date = S2BUF;
		COM2.RX_TimeOut = TimeOutSet1;
	}

	/* ���� */
	if (S2CON & 0x02)
	{
		S2CON &= ~0x02;
#if enableUart2SendInterrupt
		if (COM2.B_TX_busy == TRUE && COM2.TX_read != COM2.TX_write)
		{
			S2BUF = TX2_Buffer[COM2.TX_read & 0x7F];
			++COM2.TX_read;
		}
		else {
			COM2.B_TX_busy = FALSE;
		}
#else
		COM2.B_TX_busy = FALSE;
#endif
	}
}


/**
  * ***********************************************************************
  * @brief	����2��ʼ����115200
  *
  *	@param  :
  *
  * @retval void:
  *
  * @confirm	:
  * @attention	: none
  * ***********************************************************************
  */

void p_uart2_init(void)
{
	WIFI_RX = 1;
	WIFI_TX = 1;
	PORT_MODE_SET(WIFI_RX_G, WIFI_RX_P, PORT_MODE_AD);

	//disableUar2tInterrupts();
	S2CON = 0x50;		                // 8λ���ݣ��ɱ䲨����
	AUXR |= 0x04;		                // ��ʱ��2ʱ��1ΪFOSC,��1T 
	T2L = (65536 - (FOSC / 4 / BAUD));     // ���ö�ʱ��ֵ ��������12Ϊ����
	T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;   // ���ö�ʱ��ֵ
	AUXR |= 0x10;		                // ������ʱ��2	  

	enableUart2Interrupts();
}


void p_uart2_send_byte(char dat)
{
#if enableUart2SendInterrupt
	if (COM2.TX_read != COM2.TX_write + 1)
	{
		TX2_Buffer[COM2.TX_write & 0x7F] = dat;	//װ���ͻ���
		++COM2.TX_write;
	}

	if (COM2.B_TX_busy == FALSE)		//����
	{
		COM2.B_TX_busy = TRUE;		    //��־æ
		S2BUF = dat;
		++COM2.TX_read;
	}

#else
	while (COM2.B_TX_busy);
	COM2.B_TX_busy = 1;
	S2BUF = dat;
#endif
}

static uint16_t uart2_send_idx = 0;
static uint16_t uart1_send_idx = 0;
void p_uart2_send_data(char *p, uint8_t length)
{
	
	for (uart2_send_idx = 0; uart2_send_idx < length; uart2_send_idx++)
	{
		p_uart2_send_byte(p[uart2_send_idx]);
	}
}

/* ����WIFI�������ݸ�ʽ���͵�GSM�е� */
void p_uart1_send_data_backslash(char* p, uint16_t length)
{
	for (uart1_send_idx = 0; uart1_send_idx < length; ++uart1_send_idx)
	{
		/* ����\r\n */
		if ('\r' == p[uart1_send_idx]
			|| '\n' == p[uart1_send_idx])
		{
			continue;
		}

		if ('\"' == p[uart1_send_idx])
		{
			p_uart1_send_byte('\\');
		}
		p_uart1_send_byte(p[uart1_send_idx]);
	}
}

void p_uart2_send_string(char *p)
{
	while (*p)
	{
		p_uart2_send_byte(*p);
		++p;
	}
}



#if 0
void p_uart1_uninit(void)
{
	/* ѡ�񴮿�һ�Ĺ�������Ϊ RxD:P1.6  TxD:P1.7 */
	// SCON = 0x00;
	//TMOD = 0x00;		
	//TL1 = 0x00;		 
	//TH1 = 0x00;		 
	// AUXR = 0x00;		
	TF1 = 0;
	TR1 = 0;

	/*PORT_MODE_SET(LR_TX_G, LR_TX_P, PORT_MODE_PP);
	PORT_MODE_SET(LR_RX_G, LR_RX_P, PORT_MODE_PP);
	LR_TX = 0;
	LR_RX = 0;*/

	//disableUartInterrupts();
}


void p_uart2_uninit(void)
{
	/*PORT_MODE_SET(AIR_TX_G, AIR_TX_P, PORT_MODE_PP);
	PORT_MODE_SET(AIR_RX_G, AIR_RX_P, PORT_MODE_PP);
	AIR_TX = 0;
	AIR_RX = 0;*/
}



void p_uart1_init(void)
{
	LR_TX = 1;
	LR_RX = 1;
	PORT_MODE_SET(LR_RX_G, LR_RX_P, PORT_MODE_AD);

	/* ѡ�񴮿�һ�Ĺ�������Ϊ RxD:P3.6  TxD:P3.7 */
	// P_SW1 = 0x40;
	disableUartInterrupts();
	/* 9600 */
	//SCON = 0x50;		// 8λ����,�ɱ䲨����
	//AUXR |= 0x40;		// ��ʱ��1ʱ��ΪFosc,��1T
	//AUXR &= 0xFE;		// ����1ѡ��ʱ��1Ϊ�����ʷ�����
	//TMOD &= 0x0F;		// �趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	//TL1 = 0xC7;			// �趨��ʱ��ֵ
	//TH1 = 0xFE;			// �趨��ʱ��ֵ
	//ET1 = 0;			// ��ֹ��ʱ��1�ж�
	//TR1 = 1;			// ������ʱ��1
	/* 115200 */
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xC7;			// �趨��ʱ��ֵ
	TH1 = 0xFE;			// �趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

	enableUartInterrupts();
}

void p_uart2_init(void)
{
	AIR_RX = 1;
	AIR_TX = 1;
	PORT_MODE_SET(AIR_RX_G, AIR_RX_P, PORT_MODE_AD);

	disableUartInterrupts();

	S2CON = 0x50;		// 8λ����,�ɱ䲨����
	AUXR |= 0x04;		// ��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xE6;		    // �趨��ʱ��ֵ
	T2H = 0xFF;		    // �趨��ʱ��ֵ
	AUXR |= 0x10;		// ������ʱ��2

	IE2 |= 0x01;
	enableUartInterrupts();
}


#endif