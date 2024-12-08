#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "TicTacToe.h"
/* 全局常量 */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BOARD_SIZE    3  /* 棋盘的大小 */
#define CELL_SIZE     80 /* 每个格子的大小 */
#define LINE_WIDTH    10  /* 线条宽度 */
#define PLAYER_X      1  /* 玩家X */
#define PLAYER_O      2  /* 玩家O */
#define EMPTY         0  /* 空格 */

///* 游戏状态 */
//int who=0;
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

/* 绘制棋盘 */
void DrawBoard(void) {
    GUI_SetColor(GUI_BLACK);
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] != EMPTY) {
                DrawMove(row, col, board[row][col]);
            }
        }
    }
}

/* 绘制网格 */
void DrawGrid(void) {
	who = 0;
    GUI_SetColor(GUI_BLACK);
    for (int i = 1; i < BOARD_SIZE; i++) {
        /* 绘制水平线 */
        GUI_DrawHLine(i * CELL_SIZE, 0, SCREEN_WIDTH);
        /* 绘制垂直线 */
        GUI_DrawVLine(i * CELL_SIZE, 0, SCREEN_HEIGHT);
    }
}

/* 绘制玩家的标记 (X 或 O) */
void DrawMove(int row, int col, int player) {
	who = 1;
    int x = col * CELL_SIZE + CELL_SIZE / 2;
    int y = row * CELL_SIZE + CELL_SIZE / 2;
    GUI_SetBkColor(GUI_WHITE);
    if (player == PLAYER_X) {
        GUI_SetFont(&GUI_Font32_ASCII);
        GUI_SetColor(GUI_RED);
        GUI_DispStringAt("X", x - 10, y - 10);  // 绘制X
    } else if (player == PLAYER_O) {
        GUI_SetFont(&GUI_Font32_ASCII);
        GUI_SetColor(GUI_BLUE);
        GUI_DispStringAt("O", x - 10, y - 10);  // 绘制O
    }
}

/* 绘制玩家信息区域 */
void DrawPlayerInfo(void) {
    // 绘制右侧黄色背景区域
    GUI_SetColor(GUI_YELLOW);
    GUI_FillRect(240, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // 绘制黄色背景
    
    // 显示当前玩家
	GUI_SetBkColor(GUI_YELLOW );
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_ASCII);
	if(who==0)
	{
		GUI_DispStringAt("Player X", 245, 80);
	}
	else
	{
		if (current_player == PLAYER_X) {
			GUI_DispStringAt("Player O", 245, 80);
		} else {
			GUI_DispStringAt("Player X", 245, 80);
		}
	}
}

/* 绘制游戏结果 */
void DrawGameResult(int winner) {
    GUI_SetColor(GUI_YELLOW);
    GUI_FillRect(240, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // 清除黄色背景区域
    GUI_SetBkColor(GUI_YELLOW);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_ASCII);
    if (winner == PLAYER_X) {
        GUI_DispStringAt("X Wins!", 245, 145);
    } else if (winner == PLAYER_O) {
        GUI_DispStringAt("O Wins!", 245, 145);
    } else {
        GUI_DispStringAt("   Draw!", 245, 145);
    }
}

/* 检查是否有玩家获胜 */
int CheckWinner(void) {
    // 检查行
    for (int row = 0; row < BOARD_SIZE; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2] && board[row][0] != EMPTY) {
            return board[row][0];
        }
    }

    // 检查列
    for (int col = 0; col < BOARD_SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col] && board[0][col] != EMPTY) {
            return board[0][col];
        }
    }

    // 检查对角线
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        return board[0][2];
    }

    // 检查平局（如果所有格子都已填充且没有赢家）
    int filled = 1;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == EMPTY) {
                filled = 0;
                break;
            }
        }
    }
    if (filled) {
        return -1;  // 平局
    }

    return 0;  // 游戏继续
}

/* 处理触摸事件，更新棋盘 */
void HandleTouchEvent(int x, int y) {
    if (game_over) return;

    // 计算触摸的行列
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;

    if (row < BOARD_SIZE && col < BOARD_SIZE && board[row][col] == EMPTY) {
        board[row][col] = current_player;  // 在指定位置放置玩家的标记
        DrawMove(row, col, current_player);
        DrawPlayerInfo();  // 更新当前玩家信息区域

        // 切换玩家
        current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;

        // 检查是否有胜者
        int winner = CheckWinner();
        if (winner != 0) {
            game_over = 1;
            DrawGameResult(winner);  // 显示游戏结果
        }
    }
}

/* 重置游戏状态 */
void ResetGame(void) {
    game_over = 0;
    current_player = PLAYER_X;  // 从玩家X开始
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = EMPTY;  // 清空棋盘
        }
    }
    GUI_Clear();  // 清空显示
    DrawGrid();   // 重新绘制网格
    DrawBoard();  // 重新绘制棋盘
    DrawPlayerInfo();  // 显示当前玩家信息
}

/* 游戏主循环 */
void TicTacToeGame(void) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    ResetGame();  // 初始化游戏

    while (1) {
        /* 游戏的主循环 */
		 GUI_SetBkColor(GUI_WHITE);
        if (game_over) {
            // 等待用户点击重新开始
            GUI_Delay(1000);
            ResetGame();  // 重置游戏
        }

        GUI_TOUCH_Exec();  // 处理触摸事件
        GUI_Delay(10);     // 控制游戏刷新频率
    }
}

///* 游戏入口 */
//void emwin_main(void) {
//    WM_SetCreateFlags(WM_CF_MEMDEV); /* 启用内存设备加速 */
//    GUI_Init();

//    /* 启动游戏 */
//    TicTacToeGame();
//}
