#ifndef __TicTacToe_H__
#define __TicTacToe_H__

#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

/* 全局常量 */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BOARD_SIZE    3  /* 棋盘的大小 */
#define CELL_SIZE     80 /* 每个格子的大小 */
#define LINE_WIDTH    10  /* 线条宽度 */
#define PLAYER_X      1  /* 玩家X */
#define PLAYER_O      2  /* 玩家O */
#define EMPTY         0  /* 空格 */

/* 游戏状态 */

//static int board[BOARD_SIZE][BOARD_SIZE]; /* 棋盘 */
//static int current_player = PLAYER_X;      /* 当前玩家 */
//static int game_over = 0;                  /* 游戏结束标志 */

/* 函数原型 */
void DrawBoard(void);
void DrawGrid(void);
void DrawMove(int row, int col, int player);
int CheckWinner(void);
void HandleTouchEvent(int x, int y);
void ResetGame(void);
void DrawPlayerInfo(void);
void DrawGameResult(int winner);
/* 游戏主循环 */
void TicTacToeGame(void) ;
#endif
