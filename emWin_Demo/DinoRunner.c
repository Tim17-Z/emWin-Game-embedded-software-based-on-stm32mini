#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

/* ȫ�ֳ����ͱ��� */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define GROUND_Y      200  /* ��ƽ��Y���� */
#define TREE_WIDTH    20   /* С����� */
#define TREE_HEIGHT   40   /* С���߶� */
#define DINO_WIDTH    30   /* ������� */
#define DINO_HEIGHT   30   /* �����߶� */
#define JUMP_HEIGHT   80   /* ������Ծ�߶� */
#define JUMP_SPEED    15   /* ������Ծ�ٶ� */
#define TREE_SPEED    10    /* С���ƶ��ٶ� */

static int dino_x = 50;             /* ������X���� */
static int dino_y = GROUND_Y - DINO_HEIGHT; /* ������Y���� */
static int is_jumping = 0;          /* �Ƿ�������Ծ */
static int jump_direction = -1;     /* ��Ծ����-1:���ϣ�1:���£� */
static int tree_x = SCREEN_WIDTH;   /* С����X���� */
static int game_over = 0;           /* ��Ϸ�Ƿ���� */

/* ��������ͼ�� */
#define DINO_PIXEL_SIZE 8 /* ��������ͼ���Ĵ�С */
static const unsigned char dino_bitmap[DINO_PIXEL_SIZE][DINO_PIXEL_SIZE] = {
    {0, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 0, 1, 1, 0, 1, 1},
    {0, 0, 0, 1, 1, 0, 0, 0},
};

/* С������ͼ�� */
#define TREE_PIXEL_SIZE 8 /* С������ͼ���Ĵ�С */
static const unsigned char tree_bitmap[TREE_PIXEL_SIZE][TREE_PIXEL_SIZE] = {
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
};

/* ���ƿ��� */
void DrawDino(void) {
    GUI_SetColor(GUI_GREEN);
    for (int y = 0; y < DINO_PIXEL_SIZE; y++) {
        for (int x = 0; x < DINO_PIXEL_SIZE; x++) {
            if (dino_bitmap[y][x]) {
                GUI_FillRect(dino_x + x * (DINO_WIDTH / DINO_PIXEL_SIZE),
                             dino_y + y * (DINO_HEIGHT / DINO_PIXEL_SIZE),
                             dino_x + (x + 1) * (DINO_WIDTH / DINO_PIXEL_SIZE) - 1,
                             dino_y + (y + 1) * (DINO_HEIGHT / DINO_PIXEL_SIZE) - 1);
            }
        }
    }
}

/* ���Ƶ�ƽ�� */
void DrawGround(void) {
    GUI_SetColor(GUI_BLACK);
    GUI_DrawHLine(GROUND_Y, 0, SCREEN_WIDTH);
}

/* ����С�� */
void DrawTree(void) {
    GUI_SetColor(GUI_BROWN);
    for (int y = 0; y < TREE_PIXEL_SIZE; y++) {
        for (int x = 0; x < TREE_PIXEL_SIZE; x++) {
            if (tree_bitmap[y][x]) {
                GUI_FillRect(tree_x + x * (TREE_WIDTH / TREE_PIXEL_SIZE),
                             GROUND_Y - TREE_HEIGHT + y * (TREE_HEIGHT / TREE_PIXEL_SIZE),
                             tree_x + (x + 1) * (TREE_WIDTH / TREE_PIXEL_SIZE) - 1,
                             GROUND_Y - TREE_HEIGHT + (y + 1) * (TREE_HEIGHT / TREE_PIXEL_SIZE) - 1);
            }
        }
    }
}

/* �����ײ */
int CheckCollision(void) {
    if (tree_x < dino_x + DINO_WIDTH && tree_x + TREE_WIDTH > dino_x &&
        dino_y + DINO_HEIGHT > GROUND_Y - TREE_HEIGHT) {
        return 1; /* ��ײ���� */
    }
    return 0; /* û����ײ */
}

/* ���¿���λ�� */
void UpdateDino(void) {
    if (is_jumping) {
        dino_y += jump_direction * JUMP_SPEED;
        if (dino_y <= GROUND_Y - DINO_HEIGHT - JUMP_HEIGHT) {
            jump_direction = 1; /* ������ߵ㣬��ʼ���� */
        } else if (dino_y >= GROUND_Y - DINO_HEIGHT) {
            dino_y = GROUND_Y - DINO_HEIGHT; /* �ָ������� */
            is_jumping = 0;
        }
    }
}

/* ����С��λ�� */
void UpdateTree(void) {
    tree_x -= TREE_SPEED;
    if (tree_x + TREE_WIDTH < 0) {
        tree_x = SCREEN_WIDTH; /* С�����³������ұ� */
    }
}

/* ��Ϸ��ѭ�� */
void DinoGame(void) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    while (!game_over) {
        /* ���� */
        GUI_Clear();

        /* ����Ԫ�� */
        DrawGround();
        DrawDino();
        DrawTree();

        /* ����Ԫ��״̬ */
        UpdateDino();
        UpdateTree();

        /* �����ײ */
        if (CheckCollision()) {
            game_over = 1; /* ��Ϸ���� */
            break;
        }

        /* �ӳ� */
        GUI_Delay(50);
    }

    /* ��Ϸ�������� */
    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_Font32_ASCII);
    GUI_DispStringHCenterAt("GAME OVER", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

/* �����¼������� */
void TouchHandler(void) {
    if (!is_jumping && !game_over) {
        is_jumping = 1;
        jump_direction = -1; /* ��ʼ��Ծ */
    }
}

///* ��Ϸ��� */
//void emwin_main(void) {
//    WM_SetCreateFlags(WM_CF_MEMDEV); /* �����ڴ��豸���� */
//    GUI_Init();

//    /* ������Ϸ */
//    DinoGame();
//}
