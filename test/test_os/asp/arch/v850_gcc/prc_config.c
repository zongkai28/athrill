/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  Copyright (C) 2005 by Freelines CO.,Ltd
 *
 *  Copyright (C) 2010 by Meika Sugimoto
 * 
 *  �嵭����Ԥϡ��ʲ��� (1)���(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 */

/*
 *	�ץ����å���¸�⥸�塼���V850�ѡ�
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"

/*
 *  �ץ����å���¸�����Ѥ����ѿ�
 */
uint8_t intnest;
uint8_t current_intpri;
bool_t lock_flag;
uint16_t saved_imr[IMR_SIZE];
uint16_t disint_table[IMR_SIZE];

/*
 *  �ץ����å���¸�ν����
 */
void
prc_initialize(void)
{
	int_t i;
	
	/* �ѿ��ν���� */
	intnest = 0u;
	current_intpri = 0u;
	lock_flag = true;
	
	for(i = 0 ; i < IMR_SIZE ; i++)
	{
		disint_table[i] = 0x0000;
	}
}

/*
 *  �ץ����å���¸�ν�λ����
 */
void
prc_terminate(void)
{
	/* �ä˹Ԥ������Ϥʤ� */
}

/* ISPR�ơ��֥�λ���(prc.tf�ˤ�����) */
extern const uint16_t imr_table[][IMR_SIZE];

/*
 *	���ߤγ����ͥ���٥ޥ���(����ɽ��)������
 *
 *	����饤��ؿ��Ǥʤ��Τϡ�������֥餫�����Ѥ��뤿��Ǥ��롥
 */
void
set_intpri(uint8_t intpri)
{
	sil_wrh_mem((void *)(IMR0) , imr_table[intpri][0] | disint_table[0]);
	sil_wrh_mem((void *)(IMR1) , imr_table[intpri][1] | disint_table[1]);
	sil_wrh_mem((void *)(IMR2) , imr_table[intpri][2] | disint_table[2]);
	sil_wrh_mem((void *)(IMR3) , imr_table[intpri][3] | disint_table[3]);
	sil_wrh_mem((void *)(IMR4) , imr_table[intpri][4] | disint_table[4]);
	sil_wrh_mem((void *)(IMR5) , imr_table[intpri][5] | disint_table[5]);
	sil_wrh_mem((void *)(IMR6) , imr_table[intpri][6] | disint_table[6]);
	sil_wrb_mem((void *)(IMR7) , (uint8_t)( imr_table[intpri][7] | disint_table[7] ) );
}


/*
 *  ������׵�饤��°��������
 */

struct _int_pol_table
{
	intptr_t pol_setting0;
	intptr_t pol_setting1;
	uint8_t bitpos;
};

struct _int_pol_table const int_pol_table[] = 
{
	/* ���֤ˤ�äư㤦��ǽ�������뤿�ᡤ�ޥ���������� */
	INT_POLREG_TABLE
};

void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
	assert(VALID_INTNO_CFGINT(intno));
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);
	uint32_t intreg_addr = INTREG_ADDRESS(intno);
	
	/*
	 *  ����ߤΥޥ���
	 *
	 *  ����ߤ�����դ����ޤޡ���٥�ȥꥬ�����å��ȥꥬ������䡤��
	 *  ����ͥ���٤������Ԥ��Τϴ����ʤ��ᡤ�����°���ˤ�����餺��
	 *  ��ö�ޥ������롥
	 */
	(void)x_disable_int(intno);
	
	/*
	 *  �����ͥ���٤�����
	 */
	sil_wrb_mem((void *)intreg_addr ,
		((sil_reb_mem((void *)intreg_addr) & ~0x7)
			| (7u - INT_IPM((volatile PRI)intpri))));
	
	/*
	 *  ����ߤΥޥ������
 	 */
	(void)x_enable_int(intno);
}

/*
 * CPU�㳰�ϥ�ɥ�ν����
 * �����ޥ����ˤ���������asp/kernel/exception.h�ǥץ��ȥ��������
 * ������Ƥ��뤿�ᡢ�ؿ��Ȥ���������ʤ���Фʤ�ʤ���
 */
void
initialize_exception(void)
{
	/* ���⤷�ʤ� */
}


#ifndef OMIT_DEFAULT_EXC_HANDLER
/*
 *  ��Ͽ����Ƥ��ʤ��㳰��ȯ������ȸƤӽФ����
 */
void
default_exc_handler(void *p_excinf)
{
    target_exit();
}
#endif /* OMIT_DEFAULT_EXC_HANDLER */

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 *  ̤��Ͽ�γ���ߤ�ȯ���������˸ƤӽФ����
 */
void
default_int_handler(void *p_excinf)
{
    target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */
