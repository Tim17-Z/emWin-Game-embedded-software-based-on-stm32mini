#ifndef __Ball_H__
#define __Ball_H__

#include "GUI.h"
#include "WM.h"
#include "math.h"
#include "stdlib.h" // Ϊ�����������

/* ȫ�ֳ��� */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define GAME_AREA_SIZE 240
#define JOYSTICK_AREA_X 240
#define JOYSTICK_BASE_RADIUS 40  // ��Բ�뾶
#define JOYSTICK_SMALL_RADIUS 25 // ң��СԲ�뾶
#define JOYSTICK_PATH_SAMPLES 36 // ·����������
#define BALL_RADIUS 10
#define FOOD_RADIUS 5            // ���С��뾶
#define MAX_BALL_RADIUS 30       // ��Ϸʧ�ܵ����뾶
#define FOOD_TIMEOUT 10000        // ʳ�ﳬʱʱ�䣨���룩
/* ���С���������� */
#define MAX_FOOD_COUNT 10

/* С��λ�� */
static int ball_x = GAME_AREA_SIZE / 2;
static int ball_y = GAME_AREA_SIZE / 2;
static float ball_radius = BALL_RADIUS;

/* ң�л����뵱ǰλ�� */
static int joystick_base_x = JOYSTICK_AREA_X + 40;
static int joystick_base_y = SCREEN_HEIGHT / 2;
static int joystick_current_x = JOYSTICK_AREA_X + 40;
static int joystick_current_y = SCREEN_HEIGHT / 2;

/* С���ƶ��ٶ� */
static int ball_speed_x = 10;
static int ball_speed_y = 10;

/* �������С����ɫ */
static int food_x = 0;
static int food_y = 0;
static GUI_COLOR food_color;
static int last_food_time = 0; // �ϴ�����С���ʱ��

static GUI_COLOR GetRandomColor() {
    return GUI_MAKE_COLOR(rand() % 0xFFFFFF);
}
/* �洢������С���λ�á���ɫ��Ϣ */
typedef struct {
    int x;
    int y;
    GUI_COLOR color;
} Food;

static Food foods[MAX_FOOD_COUNT];
static int food_count = 0;
/* ������Ϸ���� */
void DrawGameArea(void);
/* ��ʼ�����С�� */
void GenerateFoods(void) ;

/* ����С�� */
void DrawBall(void) ;
/* �������С�� */
void DrawFood(void) ;
// ���ƻ����Բ�������Ӵ�
void DrawThickCircle(int x, int y, int radius, int thickness, GUI_COLOR color) ;
/* ����ң�м�����·�� */
void DrawJoystick(void);
/* �����ײ */
int CheckCollision1(int x1, int y1, int r1, int x2, int y2, int r2) ;
/* ��������Ƿ�Ե��κ�һ�����С�� */
int CheckFoodsCollision(void) ;
/*�����Ϸ����*/
int CheckGameOver(void) ;
/* ����С��λ�� */
void UpdateBallPosition(void) ;
/* ��Ϸʧ�ܴ��� */
void HandleGameOver(int reason) ;
/* �������¼� */
void HandleTouchEvent1(int x, int y) ;
/* ��Ϸ���� */
void ResetGame1(void) ;

/* ��ⴥ��״̬������ҡ�� */
void CheckJoystickState(void) ;

/* ��Ϸ��ѭ�� */
void BallBattleGame(void);

#endif
