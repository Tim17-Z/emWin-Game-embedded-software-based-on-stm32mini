#ifndef __TicTacToe_H__
#define __TicTacToe_H__

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

/* ȫ�ֳ��� */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BOARD_SIZE    3  /* ���̵Ĵ�С */
#define CELL_SIZE     80 /* ÿ�����ӵĴ�С */
#define LINE_WIDTH    10  /* ������� */
#define PLAYER_X      1  /* ���X */
#define PLAYER_O      2  /* ���O */
#define EMPTY         0  /* �ո� */

/* ��Ϸ״̬ */

//static int board[BOARD_SIZE][BOARD_SIZE]; /* ���� */
//static int current_player = PLAYER_X;      /* ��ǰ��� */
//static int game_over = 0;                  /* ��Ϸ������־ */

/* ����ԭ�� */
void DrawBoard(void);
void DrawGrid(void);
void DrawMove(int row, int col, int player);
int CheckWinner(void);
void HandleTouchEvent(int x, int y);
void ResetGame(void);
void DrawPlayerInfo(void);
void DrawGameResult(int winner);
/* ��Ϸ��ѭ�� */
void TicTacToeGame(void) ;
#endif
