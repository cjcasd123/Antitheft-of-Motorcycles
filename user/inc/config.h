/**
  *************************************************************************
  * @file    config.h
  * @author  Toson.Huaqian
  * @version V1.0.0
  * @date    2018/11/12 18:10:43
  * @brief
  *************************************************************************
  * @attention
  *
  * <h2><center>&Copyright(c) 2014-2020 Xiamen HuaQian Studio</center></h2>
  *
  *************************************************************************
  */

  /*************************     ���ظ���������     *************************/
#ifndef CONFIG_H
#define CONFIG_H

/*************************     ϵͳͷ�ļ�����     *************************/
#include "stc8.h"
#include "type.h"

/*************************     ˽��ͷ�ļ�����     *************************/

/*************************      ���������ռ�      *************************/

/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
extern "C" {
#endif 
#endif

	/*************************        ȫ��ö��        *************************/

	/*************************       ȫ�ֺ궨��       *************************/

#define enableInterrupts()		EA = 1
#define disableInterrupts()		EA = 0

#define PORT_MODE_AD			0x00	// ׼˫���
#define PORT_MODE_PP			0x01	// �������
#define PORT_MODE_HZ			0x02	// ��������
#define PORT_MODE_OD			0x03	// �������

/* ���ö˿ڵ�ģʽ */
#define PORT_MODE_SET(g, p, m)	_PORT_MODE_SET(g, p, m)
#define _PORT_MODE_SET(g, p, m)	do{\
									P##g##M0 &= ~(0x01<<(p));\
									P##g##M1 &= ~(0x01<<(p));\
									P##g##M0 |= ((m&0x01)<<(p)); \
									P##g##M1 |= ((m>>1)<<(p)); \
								}while(0)

/* ���ö˿��Ƿ���Ҫ��������3.7K, P3.0 P3.1��Сһ�� */
#define PORT_PULL_UP_SET(g, p)	_PORT_PULL_UP_SET(g, p)
#define _PORT_PULL_UP_SET(g, p)	do{P##g##PU &= ~(0x01<<(p)); P##g##PU |= (0x01<<(p));}while(0)

/* ׼ȷ���ڲ�ʱ�� */
#define IRC32K_FREP				(uint16_t)(*((uint16_t*)(0xF8)))

/* ˮ�����أ��ⲿ�жϣ� */
#define SY1_G				3
#define SY1_P				2
#define SY1					P32

#define SY2_G				3
#define SY2_P				3
#define SY2					P33

/* WIFI���ӵ���������IO�ڣ���λ ���� ʹ�ܣ�  */
#define WIFI_RST_G  1
#define WIFI_RST_P  2
#define WIFI_RST P12

#define WIFI_UPDATE P13
#define WIFI_UPDATE_G 1
#define WIFI_UPDATE_P 3

#define WIFI_EN_G  1
#define WIFI_EN_P  4
#define WIFI_EN P14
/* Air800Eģ��-GSM/GNSS ���Ͽ�GSM�ĵ�Դ��*/
#define AIR_POWERKEY_G		2
#define AIR_POWERKEY_P		0
#define AIR_POWERKEY		P20

#define AIR_POWER_CTRL_G		2
#define AIR_POWER_CTRL_P		1
#define AIR_POWER_CTRL			P21

/* ���͵�WIFI�Ĵ��ڶ˿� */
#define WIFI_RX_G			1
#define WIFI_RX_P			0
#define WIFI_RX				P10

#define WIFI_TX_G			1
#define WIFI_TX_P			1
#define WIFI_TX				P11

/* GSMģ�飨�������Ҫ�ĵ�P36P37�� */
#define AIR_RX_G				3
#define AIR_RX_P				6
#define AIR_RX				P36

#define AIR_TX_G				3
#define AIR_TX_P				7
#define AIR_TX				P37

/* ���ش���͹̼���IO�� */
#define  IAP_RX    P30
#define  IAP_TX    P31

#define	COM_TX1_Lenth	256
#define	COM_RX1_Lenth	512

#define	COM_TX2_Lenth	128
#define	COM_RX2_Lenth	256	

#define	TimeOutSet1		5
#define	TimeOutSet2		2
#define NULL			(void*)0

#define heart_send    5
#define heart_clear   0
#define heart_time    100

#define WDT_DUAL_CORE_SEC	120
#define my_time()			g_time_ms

/*************************     ȫ�ֽṹ�嶨��     *************************/
	typedef struct
	{
		u8	TX_read;		//���Ͷ�ָ��
		u8	TX_write;		//����дָ��
		u8	B_TX_busy;		//æ��־

		u8	RX_TimeOut;		//���ճ�ʱ
		u16 RX_Cnt;			//�����ֽڼ���
	} COMx_Define;

	typedef struct
	{
		// 	u8  m_is_config_ok;    // �����Ƿ����
		u8  m_enbale_sy1;	   // ʹ��ˮӡ1
		u8  m_enbale_sy2;	   // ʹ��ˮӡ2
		//u8  m_sy_shark_cnt;    // ˮ���ζ���������
		//u8  m_sy2_shark_cnt;   // ˮ���ζ���������
		u8  m_fast_sleep;      // �Ƿ�����˯��
	//	u16 m_timer_report;    // ��ʱ�ϱ�hour, �趨��0�򲻶�ʱ�ϱ�
		u16 m_bypass_hour;	   // ��·ʱ��,���ó�0��������·����
	}st_now_config;

	typedef struct
	{
		char start;
		//char WS[6];
		//char WS_Data[3];
		char S1[6];
		char S1_Data[3];
		char S2[8];
		char S2_Data[3];
		char AT[8];
	}st_pkg_to_luat;

	typedef struct
	{
		char key_name[6];
		char val[5];
	}st_item_from_luat;


	/*************************      ȫ�ֱ�������      *************************/
	extern idata COMx_Define		COM1;
	extern idata COMx_Define		COM2;
	extern xdata u8			RX1_Buffer[];	// ���ͻ���
	extern xdata u8			TX1_Buffer[];	// ���ͻ���
	extern xdata u8			RX2_Buffer[];	// ���ͻ���
	extern xdata u8			TX2_Buffer[];	// ���ͻ���
	extern st_now_config	g_config;
	//extern idata char*		g_p_boot_reason;

	extern xdata st_pkg_to_luat			g_pkg_to_luat;
	extern idata st_item_from_luat*		g_p_item_from_luat;

	extern u32			g_time_ms;
	extern u8			g_sy1_shark_cnt;
	extern u8			g_sy2_shark_cnt;
	extern u8           g_heart_beat, g_heart_sign;
	//extern u32			next_close_bypass_ts;

	/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
}
#endif 
#endif

/*************************     ���ظ���������     *************************/
#endif