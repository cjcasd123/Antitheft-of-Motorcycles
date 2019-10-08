/**
  *************************************************************************
  * @file    event.h
  * @author  monkey.Huaqian
  * @version V1.0.0
  * @date    2019/03/08 15:17:48
  * @brief
  *************************************************************************
  * @attention ���֧��32���¼�
  *
  * <h2><center>&Copyright(c) 2014-2020 Xiamen HuaQian Studio</center></h2>
  *
  *************************************************************************
  */

  /*************************     ���ظ���������     *************************/
#ifndef EVENT_H
#define EVENT_H

/*************************     ϵͳͷ�ļ�����     *************************/

/*************************     ˽��ͷ�ļ�����     *************************/
#include "type.h"

/*************************      ���������ռ�      *************************/

/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
extern "C" {
#endif 
#endif

	/*************************        ȫ��ö��        *************************/
	typedef enum
	{
		EVNT_INIT,					// ϵͳ��ʼ�� ������
		/* �¼���� begin */
		EVNT_WATERMARK,			    // ˮӡ���жϣ�����ˮ���𶯣�
		EVNT_SLEEP,					// ˯������
		/* �¼���� end */
		EVNT_MAX					// �¼������ţ�ѹ�����
	}EVNT_Handle;

	/*************************       ȫ�ֺ궨��       *************************/



	/*************************     ȫ�ֽṹ�嶨��     *************************/

#define p_evnt_init()					g_evnt_triggle_flg = 0      // ��־�����¼��Ƿ񴥷����ı�8λ�����е�һλ��

#define p_envt_set_event(event)			_envt_set_event(event)		// �����¼�����
#define _envt_set_event(event)			do{ g_evnt_triggle_flg |= (0x01<<(uint8_t)(event));}while(0)

#define p_envt_clear_event(event)		_envt_clear_event(event)	// ����¼�������־
#define _envt_clear_event(event)		do{ g_evnt_triggle_flg &= ~(0x01<<(uint8_t)(event));}while(0)

#define p_envt_get_event(event)			_envt_get_event(event)		// �ж��¼�������־
#define _envt_get_event(event)			(uint8_t)((g_evnt_triggle_flg & (0x01<<(uint8_t)(event)))!=0)

#define p_evnt_clear()					g_evnt_triggle_flg = 0;

/*************************      ȫ�ֱ�������      *************************/
	extern uint32_t  g_evnt_triggle_flg;

	/*************************        ��������        *************************/

	/*************************      C++ ���ݶ���      *************************/
#ifdef __cplusplus 
#if __cplusplus 
}
#endif 
#endif

/*************************     ���ظ���������     *************************/
#endif



