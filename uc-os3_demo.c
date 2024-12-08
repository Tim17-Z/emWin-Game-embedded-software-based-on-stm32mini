/**
 ****************************************************************************************************
 * @file        uc-os3_demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-03-12
 * @brief       emWin的Button部件使用实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
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
/* UCOSIII任务设置 */

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO     3           /* 任务优先级 */
#define START_STK_SIZE      128         /* 任务堆栈大小 */
OS_TCB StartTaskTCB;                    /* 任务控制块 */
CPU_STK START_TASK_STK[START_STK_SIZE]; /* 任务堆栈 */
void start_task(void *p_arg);           /* 任务函数 */
extern int runGame;
/* USER_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define USER_TASK_PRIO     4            /* 任务优先级 */
#define USER_STK_SIZE      1024         /* 任务堆栈大小 */
OS_TCB User_TaskTCB;                    /* 任务控制块 */
CPU_STK USER_TASK_STK[USER_STK_SIZE];   /* 任务堆栈 */
void user_task(void *p_arg);

/* TOUCH_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TOUCH_TASK_PRIO     5           /* 任务优先级 */
#define TOUCH_STK_SIZE      512         /* 任务堆栈大小 */
OS_TCB Touch_TaskTCB;                   /* 任务控制块 */
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE]; /* 任务堆栈 */
void touch_task(void *p_arg);

/* LED0_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED0_TASK_PRIO     6            /* 任务优先级 */
#define LED0_STK_SIZE      128          /* 任务堆栈大小 */
OS_TCB Led0_TaskTCB;                    /* 任务控制块 */
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];   /* 任务堆栈 */
void led0_task(void *p_arg);

/******************************************************************************************/


/**
 * @brief       执行emwin例程
 * @param       无
 * @retval      无
 */
void uc_os3_demo(void)
{
    OS_ERR err;
    
    /* 关闭所有中断 */
    CPU_IntDis();
    
    /* 初始化uC/OS-III */
    OSInit(&err);
    
    /* 创建开始任务 */
    OSTaskCreate((OS_TCB *     )&StartTaskTCB,       /* 任务控制块 */
                 (CPU_CHAR *   )"start task",        /* 任务名字 */
                 (OS_TASK_PTR  )start_task,          /* 任务函数 */
                 (void *       )0,                   /* 传递给任务函数的参数 */
                 (OS_PRIO      )START_TASK_PRIO,     /* 任务优先级 */
                 (CPU_STK *    )&START_TASK_STK[0],  /* 任务堆栈基地址 */
                 (CPU_STK_SIZE )START_STK_SIZE / 10, /* 任务堆栈深度限位 */
                 (CPU_STK_SIZE )START_STK_SIZE,      /* 任务堆栈大小 */
                 (OS_MSG_QTY   )0,                   /* 任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息 */
                 (OS_TICK      )0,                   /* 当使能时间片轮转时的时间片长度，为0时为默认长度 */
                 (void *       )0,                   /* 用户补充的存储区 */
                 (OS_OPT       )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, /* 任务选项 */
                 (OS_ERR *     )&err);  /* 存放该函数错误时的返回值 */

    /* 开始任务调度 */
    OSStart(&err);
    
    for (;;)
    {
        /* 不会进入这里 */
    }
}

/**
 * @brief       start_task
 * @param       p_arg : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *p_arg)
{
    p_arg = p_arg;
    
    OS_ERR err;
    CPU_INT32U cnts;
    
    /* 初始化CPU库 */
    CPU_Init();
    
    /* 根据配置的节拍频率配置SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OSCfg_TickRate_Hz);
    OS_CPU_SysTickInit(cnts);
    
    /* 开启时间片调度，时间片设为默认值 */
    OSSchedRoundRobinCfg(OS_TRUE, 0, &err);
    
    GUI_Init();                       /* emWin 初始化 */
    
    /* 创建User任务 */
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

    /* 创建Led0任务 */
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

    /* 创建Touch任务 */
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

    /* 删除Start Task */
    OSTaskDel((OS_TCB *)0, &err);
}

/**
 * @brief       执行emwin例程
 * @param       p_arg : 传入参数(未用到)
 * @retval      无
 */
void user_task(void *p_arg)
{
    p_arg = p_arg;
    OS_ERR err;
    emwin_main();  /* 执行emwin例程 */
    
    while(1)
    {
        OSTimeDly(5, OS_OPT_TIME_DLY, &err);    /* 延时5ticks */
    }
}

/**
 * @brief       emwin触摸
 * @param       p_arg : 传入参数(未用到)
 * @retval      无
 */
void touch_task(void *p_arg)
{
    p_arg = p_arg;
	GUI_PID_STATE TS_State; // 创建触摸状态结构体
    GUI_CURSOR_Show();
    
    while(1)
    {
        GUI_TOUCH_Exec();
        //GUI_Delay(5);
		if(runGame ==1)
		{
			  if (GUI_TOUCH_GetState(&TS_State)) // 检测是否有触摸事件
			{
				if (TS_State.Pressed) // 检测是否按下
				{
					TouchHandler(); // 调用恐龙跳跃处理函数
				}
			}
			
		}
		if(runGame==3)
		{
			 if (GUI_TOUCH_GetState(&TS_State)) // 检测是否有触摸事件
			{
				if (TS_State.Pressed)  // 检测是否按下
				{
					// 根据触摸位置处理井字棋事件
					int touch_x = TS_State.x; // 获取触摸X坐标
					int touch_y = TS_State.y; // 获取触摸Y坐标
					HandleTouchEvent(touch_x, touch_y);  // 调用触摸事件处理函数
				}
			}
		}
		if(runGame==2)
		{
			 if (GUI_TOUCH_GetState(&TS_State)) // 检测是否有触摸事件
			{
				if (TS_State.Pressed)  // 检测是否按下
				{
					// 根据触摸位置处理井字棋事件
					int touch_x = TS_State.x; // 获取触摸X坐标
					int touch_y = TS_State.y; // 获取触摸Y坐标
					HandleTouchEvent1(touch_x, touch_y);  // 调用触摸事件处理函数
				}
			}
		}
    }
}

/**
 * @brief       代表系统在运行中
 * @param       p_arg : 传入参数(未用到)
 * @retval      无
 */
void led0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

    while (1)
    {
        LED0_TOGGLE();
        OSTimeDly(500, OS_OPT_TIME_DLY, &err);     /* 延时500ticks */
    }
}
