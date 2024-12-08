#include "emwin_demo.h"
#include "./SYSTEM/usart/usart.h" 
#include "./BSP/LED/led.h" 
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

/*****************************************************************************************************/
/*EMWIN*/
#include "GUI.h"
#include "WM.h"
#include "DIALOG.h"
//#include "BUTTON.h"
#include "DinoRunner.H"
#include "TicTacToe.h"
#include "Ball.h"
/*****************************************************************************************************/
/*uC-OS3*/
#include "os.h"
#include "cpu.h"


#define ID_FRAMEWIN_0 (GUI_ID_USER + 0x00)
#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_BUTTON_2 (GUI_ID_USER + 0x03)
#define ID_BUTTON_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_0 (GUI_ID_USER + 0x05)
#define ID_TEXT_1 (GUI_ID_USER + 0x06)
#define ID_TEXT_2 (GUI_ID_USER + 0x07)
#define ID_TEXT_3 (GUI_ID_USER + 0x08)
 int runGame = 0; // 0: ��ʾ�˵�, 1: DinoGame ����
/* �Ի�����Դ�� */
/* �����Ի���Ϳؼ����� */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 240, 0, 0x64, 0 },
    { BUTTON_CreateIndirect, "Dino Runner", ID_BUTTON_0, 50, 10, 80, 80, 0, 0x0, 0 },  // ���ΰ�ť
    { BUTTON_CreateIndirect, "Ball", ID_BUTTON_1, 180, 10, 80, 80, 0, 0x0, 0 },   // ���ΰ�ť
    { BUTTON_CreateIndirect, "Tic Tac Toe", ID_BUTTON_2, 50, 110, 80, 80, 0, 0x0, 0 }, // ���ΰ�ť
    { BUTTON_CreateIndirect, "More Game", ID_BUTTON_3, 180, 110, 80, 80, 0, 0x0, 0 }, // ���ΰ�ť
//    { TEXT_CreateIndirect, "Game 1", ID_TEXT_0, 30, 90, 120, 20, 0, 0x64, 0 },
//    { TEXT_CreateIndirect, "Game 2", ID_TEXT_1, 160, 90, 120, 20, 0, 0x64, 0 },
//    { TEXT_CreateIndirect, "Game 3", ID_TEXT_2, 30, 190, 120, 20, 0, 0x64, 0 },
//    { TEXT_CreateIndirect, "Game 4", ID_TEXT_3, 160, 190, 120, 20, 0, 0x64, 0 },
};

/* �����ص������пؼ��ĳ�ʼ���߼� */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            GUI_SetBkColor(GUI_WHITE);
            GUI_Clear();
            break;

        case WM_INIT_DIALOG:
            /* ��ʼ���Ի��� */
            hItem = pMsg->hWin;
            FRAMEWIN_SetTitleHeight(hItem, 20);
            FRAMEWIN_SetText(hItem, "GAME");
            FRAMEWIN_SetFont(hItem, GUI_FONT_16_ASCII); /* ���������С */
            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
            FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

            /* ��ʼ��BUTTON0 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetText(hItem, "Dino Runner");

            /* ��ʼ��BUTTON1 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetText(hItem, "Ball");

            /* ��ʼ��BUTTON2 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
            BUTTON_SetText(hItem, "Tic Tac Toe");

            /* ��ʼ��BUTTON3 */
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
            BUTTON_SetText(hItem, "More Game");

//            /* ��ʼ��TEXT0 */
//            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
//            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//            TEXT_SetFont(hItem, GUI_FONT_16_ASCII); /* ���������С */
//            TEXT_SetText(hItem, "Dino Runner");

//            /* ��ʼ��TEXT1 */
//            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
//            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//            TEXT_SetFont(hItem, GUI_FONT_16_ASCII); /* ���������С */
//            TEXT_SetText(hItem, "Ball");

//            /* ��ʼ��TEXT2 */
//            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
//            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//            TEXT_SetFont(hItem, GUI_FONT_16_ASCII); /* ���������С */
//            TEXT_SetText(hItem, "Tic Tac Toe");

//            /* ��ʼ��TEXT3 */
//            hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
//            TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//            TEXT_SetFont(hItem, GUI_FONT_16_ASCII); /* ���������С */
//            TEXT_SetText(hItem, "More Game");
//            break;

        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;

            switch (Id)
            {
                case ID_BUTTON_0: /* Dino Runner�İ�ť֪ͨ */
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);

                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED: /* ��ť�����²��ͷ� */
                            //GUI_MessageBox("Dino Runner Selected!", "Info", GUI_MESSAGEBOX_CF_MOVEABLE);
							runGame = 1;    // ����״̬�л��� DinoGame
							WM_DeleteWindow(pMsg->hWin); // ���ٵ�ǰ����
							break;
                    }

                    break;

                case ID_BUTTON_1: /* Box Push�İ�ť֪ͨ */
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);

                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            runGame = 2;    // ����״̬�л��� DinoGame
							WM_DeleteWindow(pMsg->hWin); // ���ٵ�ǰ����
							break;
                    }

                    break;

                case ID_BUTTON_2: /* Tic Tac Toe�İ�ť֪ͨ */
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);

                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            //GUI_MessageBox("Tic Tac Toe Selected!", "Info", GUI_MESSAGEBOX_CF_MOVEABLE);
                            runGame = 3;    // ����״̬�л��� DinoGame
							WM_DeleteWindow(pMsg->hWin); // ���ٵ�ǰ����
							break;
                    }

                    break;

                case ID_BUTTON_3: /* Zhou Lihui�İ�ť֪ͨ */
                    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);

                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            break;

                        case WM_NOTIFICATION_RELEASED:
                            GUI_MessageBox("Coming soon...", "Developed by Tim17 ", GUI_MESSAGEBOX_CF_MOVEABLE);
                            break;
                    }

                    break;
            }

            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
 * @brief       ����һ���Ի���
 */
WM_HWIN emwin_create_framewin(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}

/**
 * @brief       emwin_main
 */
void emwin_main(void) 
{
    runGame = 0; // 0: ��ʾ�˵�, 1: DinoGame ����
    WM_HWIN hWin = emwin_create_framewin();

    while (1)
    {
        if (runGame == 1) {
			// ���ٵ�ǰ���ڣ����˵���
            WM_DeleteWindow(hWin);
            DinoGame();  // ���״̬����Ϸ���У�ֱ����ת�� DinoGame ��ѭ��
            runGame = 0; // DinoGame ���غ����»ص����˵�
            hWin = emwin_create_framewin(); // ���´������˵�
        }
		if (runGame == 3) {
			// ���ٵ�ǰ���ڣ����˵���
            WM_DeleteWindow(hWin);
            TicTacToeGame();  // ���״̬����Ϸ���У�ֱ����ת�� DinoGame ��ѭ��
            runGame = 0; //TicTacToeGame ���غ����»ص����˵�
            hWin = emwin_create_framewin(); // ���´������˵�
        }
		if (runGame == 2) {
			// ���ٵ�ǰ���ڣ����˵���
            WM_DeleteWindow(hWin);
			
            BallBattleGame();  // ���״̬����Ϸ���У�ֱ����ת�� DinoGame ��ѭ��
            runGame = 0; //TicTacToeGame ���غ����»ص����˵�
            hWin = emwin_create_framewin(); // ���´������˵�
        }
        GUI_Delay(100);
    }
}
