#include "GUI.h"
#include "WM.h"
#include "BUTTON.h"

/* 全局常量和变量 */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define GROUND_Y      200  /* 地平线Y坐标 */
#define TREE_WIDTH    20   /* 小树宽度 */
#define TREE_HEIGHT   40   /* 小树高度 */
#define DINO_WIDTH    30   /* 恐龙宽度 */
#define DINO_HEIGHT   30   /* 恐龙高度 */
#define JUMP_HEIGHT   80   /* 恐龙跳跃高度 */
#define JUMP_SPEED    15   /* 恐龙跳跃速度 */
#define TREE_SPEED    10    /* 小树移动速度 */

static int dino_x = 50;             /* 恐龙的X坐标 */
static int dino_y = GROUND_Y - DINO_HEIGHT; /* 恐龙的Y坐标 */
static int is_jumping = 0;          /* 是否正在跳跃 */
static int jump_direction = -1;     /* 跳跃方向（-1:向上，1:向下） */
static int tree_x = SCREEN_WIDTH;   /* 小树的X坐标 */
static int game_over = 0;           /* 游戏是否结束 */

/* 恐龙像素图案 */
#define DINO_PIXEL_SIZE 8 /* 恐龙像素图案的大小 */
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

/* 小树像素图案 */
#define TREE_PIXEL_SIZE 8 /* 小树像素图案的大小 */
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

/* 绘制恐龙 */
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

/* 绘制地平线 */
void DrawGround(void) {
    GUI_SetColor(GUI_BLACK);
    GUI_DrawHLine(GROUND_Y, 0, SCREEN_WIDTH);
}

/* 绘制小树 */
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

/* 检测碰撞 */
int CheckCollision(void) {
    if (tree_x < dino_x + DINO_WIDTH && tree_x + TREE_WIDTH > dino_x &&
        dino_y + DINO_HEIGHT > GROUND_Y - TREE_HEIGHT) {
        return 1; /* 碰撞发生 */
    }
    return 0; /* 没有碰撞 */
}

/* 更新恐龙位置 */
void UpdateDino(void) {
    if (is_jumping) {
        dino_y += jump_direction * JUMP_SPEED;
        if (dino_y <= GROUND_Y - DINO_HEIGHT - JUMP_HEIGHT) {
            jump_direction = 1; /* 到达最高点，开始下落 */
        } else if (dino_y >= GROUND_Y - DINO_HEIGHT) {
            dino_y = GROUND_Y - DINO_HEIGHT; /* 恢复到地面 */
            is_jumping = 0;
        }
    }
}

/* 更新小树位置 */
void UpdateTree(void) {
    tree_x -= TREE_SPEED;
    if (tree_x + TREE_WIDTH < 0) {
        tree_x = SCREEN_WIDTH; /* 小树重新出现在右边 */
    }
}

/* 游戏主循环 */
void DinoGame(void) {
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    while (!game_over) {
        /* 清屏 */
        GUI_Clear();

        /* 绘制元素 */
        DrawGround();
        DrawDino();
        DrawTree();

        /* 更新元素状态 */
        UpdateDino();
        UpdateTree();

        /* 检测碰撞 */
        if (CheckCollision()) {
            game_over = 1; /* 游戏结束 */
            break;
        }

        /* 延迟 */
        GUI_Delay(50);
    }

    /* 游戏结束界面 */
    GUI_SetColor(GUI_RED);
    GUI_SetFont(&GUI_Font32_ASCII);
    GUI_DispStringHCenterAt("GAME OVER", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

/* 触摸事件处理函数 */
void TouchHandler(void) {
    if (!is_jumping && !game_over) {
        is_jumping = 1;
        jump_direction = -1; /* 开始跳跃 */
    }
}

///* 游戏入口 */
//void emwin_main(void) {
//    WM_SetCreateFlags(WM_CF_MEMDEV); /* 启用内存设备加速 */
//    GUI_Init();

//    /* 启动游戏 */
//    DinoGame();
//}
