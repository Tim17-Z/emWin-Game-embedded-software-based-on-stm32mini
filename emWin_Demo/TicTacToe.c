#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"
#include "TicTacToe.h"
/* ȫ�ֳ��� */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BOARD_SIZE    3  /* ���̵Ĵ�С */
#define CELL_SIZE     80 /* ÿ�����ӵĴ�С */
#define LINE_WIDTH    10  /* ������� */
#define PLAYER_X      1  /* ���X */
#define PLAYER_O      2  /* ���O */
#define EMPTY         0  /* �ո� */

///* ��Ϸ״̬ */
//int who=0;
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

/* �������� */
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

/* �������� */
void DrawGrid(void) {
	who = 0;
    GUI_SetColor(GUI_BLACK);
    for (int i = 1; i < BOARD_SIZE; i++) {
        /* ����ˮƽ�� */
        GUI_DrawHLine(i * CELL_SIZE, 0, SCREEN_WIDTH);
        /* ���ƴ�ֱ�� */
        GUI_DrawVLine(i * CELL_SIZE, 0, SCREEN_HEIGHT);
    }
}

/* ������ҵı�� (X �� O) */
void DrawMove(int row, int col, int player) {
	who = 1;
    int x = col * CELL_SIZE + CELL_SIZE / 2;
    int y = row * CELL_SIZE + CELL_SIZE / 2;
    GUI_SetBkColor(GUI_WHITE);
    if (player == PLAYER_X) {
        GUI_SetFont(&GUI_Font32_ASCII);
        GUI_SetColor(GUI_RED);
        GUI_DispStringAt("X", x - 10, y - 10);  // ����X
    } else if (player == PLAYER_O) {
        GUI_SetFont(&GUI_Font32_ASCII);
        GUI_SetColor(GUI_BLUE);
        GUI_DispStringAt("O", x - 10, y - 10);  // ����O
    }
}

/* ���������Ϣ���� */
void DrawPlayerInfo(void) {
    // �����Ҳ��ɫ��������
    GUI_SetColor(GUI_YELLOW);
    GUI_FillRect(240, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // ���ƻ�ɫ����
    
    // ��ʾ��ǰ���
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

/* ������Ϸ��� */
void DrawGameResult(int winner) {
    GUI_SetColor(GUI_YELLOW);
    GUI_FillRect(240, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // �����ɫ��������
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

/* ����Ƿ�����һ�ʤ */
int CheckWinner(void) {
    // �����
    for (int row = 0; row < BOARD_SIZE; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2] && board[row][0] != EMPTY) {
            return board[row][0];
        }
    }

    // �����
    for (int col = 0; col < BOARD_SIZE; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col] && board[0][col] != EMPTY) {
            return board[0][col];
        }
    }

    // ���Խ���
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        return board[0][2];
    }

    // ���ƽ�֣�������и��Ӷ��������û��Ӯ�ң�
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
        return -1;  // ƽ��
    }

    return 0;  // ��Ϸ����
}

/* �������¼����������� */
void HandleTouchEvent(int x, int y) {
    if (game_over) return;

    // ���㴥��������
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;

    if (row < BOARD_SIZE && col < BOARD_SIZE && board[row][col] == EMPTY) {
        board[row][col] = current_player;  // ��ָ��λ�÷�����ҵı��
        DrawMove(row, col, current_player);
        DrawPlayerInfo();  // ���µ�ǰ�����Ϣ����

        // �л����
        current_player = (current_player == PLAYER_X) ? PLAYER_O : PLAYER_X;

        // ����Ƿ���ʤ��
        int winner = CheckWinner();
        if (winner != 0) {
            game_over = 1;
            DrawGameResult(winner);  // ��ʾ��Ϸ���
        }
    }
}

/* ������Ϸ״̬ */
void ResetGame(void) {
    game_over = 0;
    current_player = PLAYER_X;  // �����X��ʼ
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            board[row][col] = EMPTY;  // �������
        }
    }
    GUI_Clear();  // �����ʾ
    DrawGrid();   // ���»�������
    DrawBoard();  // ���»�������
    DrawPlayerInfo();  // ��ʾ��ǰ�����Ϣ
}

/* ��Ϸ��ѭ�� */
void TicTacToeGame(void) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    ResetGame();  // ��ʼ����Ϸ

    while (1) {
        /* ��Ϸ����ѭ�� */
		 GUI_SetBkColor(GUI_WHITE);
        if (game_over) {
            // �ȴ��û�������¿�ʼ
            GUI_Delay(1000);
            ResetGame();  // ������Ϸ
        }

        GUI_TOUCH_Exec();  // �������¼�
        GUI_Delay(10);     // ������Ϸˢ��Ƶ��
    }
}

///* ��Ϸ��� */
//void emwin_main(void) {
//    WM_SetCreateFlags(WM_CF_MEMDEV); /* �����ڴ��豸���� */
//    GUI_Init();

//    /* ������Ϸ */
//    TicTacToeGame();
//}
