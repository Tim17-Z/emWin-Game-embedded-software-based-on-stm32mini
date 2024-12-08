#ifndef __DinoRunner_H__
#define __DinoRunner_H__
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
void DrawDino(void) ;
/* 绘制地平线 */
void DrawGround(void);

/* 绘制小树 */
void DrawTree(void) ;
/* 检测碰撞 */
int CheckCollision(void);
/* 更新恐龙位置 */
void UpdateDino(void) ;
/* 更新小树位置 */
void UpdateTree(void);

/* 游戏主循环 */
void DinoGame(void);

/* 触摸事件处理函数 */
void TouchHandler(void);

#endif
