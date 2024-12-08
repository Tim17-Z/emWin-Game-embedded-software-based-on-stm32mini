/**
 ****************************************************************************************************
 * @file        uc-os3_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-03-12
 * @brief       emWin��Button����ʹ��ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "uc-os3_demo.h"
#include "os.h"
#include "cpu.h"
#include "GUI.h"
#include "emwin_demo.h"
#include "WM.h"

#include "DinoRunner.h"
#include "TicTacToe.h"
#include "Ball.h"
/******************************************************************************************/
/* UCOSIII�������� */

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     3           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
OS_TCB StartTaskTCB;                    /* ������ƿ� */
CPU_STK START_TASK_STK[START_STK_SIZE]; /* �����ջ */
void start_task(void *p_arg);           /* ������ */
extern int runGame;
/* USER_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define USER_TASK_PRIO     4            /* �������ȼ� */
#define USER_STK_SIZE      1024         /* �����ջ��С */
OS_TCB User_TaskTCB;                    /* ������ƿ� */
CPU_STK USER_TASK_STK[USER_STK_SIZE];   /* �����ջ */
void user_task(void *p_arg);

/* TOUCH_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TOUCH_TASK_PRIO     5           /* �������ȼ� */
#define TOUCH_STK_SIZE      512         /* �����ջ��С */
OS_TCB Touch_TaskTCB;                   /* ������ƿ� */
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE]; /* �����ջ */
void touch_task(void *p_arg);

/* LED0_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED0_TASK_PRIO     6            /* �������ȼ� */
#define LED0_STK_SIZE      128          /* �����ջ��С */
OS_TCB Led0_TaskTCB;                    /* ������ƿ� */
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];   /* �����ջ */
void led0_task(void *p_arg);

/******************************************************************************************/


/**
 * @brief       ִ��emwin����
 * @param       ��
 * @retval      ��
 */
void uc_os3_demo(void)
{
    OS_ERR err;
    
    /* �ر������ж� */
    CPU_IntDis();
    
    /* ��ʼ��uC/OS-III */
    OSInit(&err);
    
    /* ������ʼ���� */
    OSTaskCreate((OS_TCB *     )&StartTaskTCB,       /* ������ƿ� */
                 (CPU_CHAR *   )"start task",        /* �������� */
                 (OS_TASK_PTR  )start_task,          /* ������ */
                 (void *       )0,                   /* ���ݸ��������Ĳ��� */
                 (OS_PRIO      )START_TASK_PRIO,     /* �������ȼ� */
                 (CPU_STK *    )&START_TASK_STK[0],  /* �����ջ����ַ */
                 (CPU_STK_SIZE )START_STK_SIZE / 10, /* �����ջ�����λ */
                 (CPU_STK_SIZE )START_STK_SIZE,      /* �����ջ��С */
                 (OS_MSG_QTY   )0,                   /* �����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ */
                 (OS_TICK      )0,                   /* ��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��� */
                 (void *       )0,                   /* �û�����Ĵ洢�� */
                 (OS_OPT       )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, /* ����ѡ�� */
                 (OS_ERR *     )&err);  /* ��Ÿú�������ʱ�ķ���ֵ */

    /* ��ʼ������� */
    OSStart(&err);
    
    for (;;)
    {
        /* ����������� */
    }
}

/**
 * @brief       start_task
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *p_arg)
{
    p_arg = p_arg;
    
    OS_ERR err;
    CPU_INT32U cnts;
    
    /* ��ʼ��CPU�� */
    CPU_Init();
    
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OSCfg_TickRate_Hz);
    OS_CPU_SysTickInit(cnts);
    
    /* ����ʱ��Ƭ���ȣ�ʱ��Ƭ��ΪĬ��ֵ */
    OSSchedRoundRobinCfg(OS_TRUE, 0, &err);
    
    GUI_Init();                       /* emWin ��ʼ�� */
    
    /* ����User���� */
    OSTaskCreate((OS_TCB *    )&User_TaskTCB,
                 (CPU_CHAR *  )"User task",
                 (OS_TASK_PTR )user_task,
                 (void *      )0,
                 (OS_PRIO     )USER_TASK_PRIO,
                 (CPU_STK *   )&USER_TASK_STK,
                 (CPU_STK_SIZE)USER_STK_SIZE / 10,
                 (CPU_STK_SIZE)USER_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    /* ����Led0���� */
    OSTaskCreate((OS_TCB *    )&Led0_TaskTCB,
                 (CPU_CHAR *  )"led0_task",
                 (OS_TASK_PTR )led0_task,
                 (void *      )0,
                 (OS_PRIO     )LED0_TASK_PRIO,
                 (CPU_STK *   )&LED0_TASK_STK,
                 (CPU_STK_SIZE)LED0_STK_SIZE / 10,
                 (CPU_STK_SIZE)LED0_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    /* ����Touch���� */
    OSTaskCreate((OS_TCB *    )&Touch_TaskTCB,
                 (CPU_CHAR *  )"Touch task",
                 (OS_TASK_PTR )touch_task,
                 (void *      )0,
                 (OS_PRIO     )TOUCH_TASK_PRIO,
                 (CPU_STK *   )&TOUCH_TASK_STK,
                 (CPU_STK_SIZE)TOUCH_STK_SIZE / 10,
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void *      )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
                 (OS_ERR *    )&err);

    /* ɾ��Start Task */
    OSTaskDel((OS_TCB *)0, &err);
}

/**
 * @brief       ִ��emwin����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void user_task(void *p_arg)
{
    p_arg = p_arg;
    OS_ERR err;
    emwin_main();  /* ִ��emwin���� */
    
    while(1)
    {
        OSTimeDly(5, OS_OPT_TIME_DLY, &err);    /* ��ʱ5ticks */
    }
}

/**
 * @brief       emwin����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void touch_task(void *p_arg)
{
    p_arg = p_arg;
	GUI_PID_STATE TS_State; // ��������״̬�ṹ��
    GUI_CURSOR_Show();
    
    while(1)
    {
        GUI_TOUCH_Exec();
        //GUI_Delay(5);
		if(runGame ==1)
		{
			  if (GUI_TOUCH_GetState(&TS_State)) // ����Ƿ��д����¼�
			{
				if (TS_State.Pressed) // ����Ƿ���
				{
					TouchHandler(); // ���ÿ�����Ծ������
				}
			}
			
		}
		if(runGame==3)
		{
			 if (GUI_TOUCH_GetState(&TS_State)) // ����Ƿ��д����¼�
			{
				if (TS_State.Pressed)  // ����Ƿ���
				{
					// ���ݴ���λ�ô��������¼�
					int touch_x = TS_State.x; // ��ȡ����X����
					int touch_y = TS_State.y; // ��ȡ����Y����
					HandleTouchEvent(touch_x, touch_y);  // ���ô����¼�������
				}
			}
		}
		if(runGame==2)
		{
			 if (GUI_TOUCH_GetState(&TS_State)) // ����Ƿ��д����¼�
			{
				if (TS_State.Pressed)  // ����Ƿ���
				{
					// ���ݴ���λ�ô��������¼�
					int touch_x = TS_State.x; // ��ȡ����X����
					int touch_y = TS_State.y; // ��ȡ����Y����
					HandleTouchEvent1(touch_x, touch_y);  // ���ô����¼�������
				}
			}
		}
    }
}

/**
 * @brief       ����ϵͳ��������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void led0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

    while (1)
    {
        LED0_TOGGLE();
        OSTimeDly(500, OS_OPT_TIME_DLY, &err);     /* ��ʱ500ticks */
    }
}
