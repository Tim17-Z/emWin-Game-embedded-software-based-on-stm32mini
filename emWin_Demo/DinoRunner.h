#ifndef __DinoRunner_H__
#define __DinoRunner_H__
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
void DrawDino(void) ;
/* ���Ƶ�ƽ�� */
void DrawGround(void);

/* ����С�� */
void DrawTree(void) ;
/* �����ײ */
int CheckCollision(void);
/* ���¿���λ�� */
void UpdateDino(void) ;
/* ����С��λ�� */
void UpdateTree(void);

/* ��Ϸ��ѭ�� */
void DinoGame(void);

/* �����¼������� */
void TouchHandler(void);

#endif
