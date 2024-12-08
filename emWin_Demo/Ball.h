#ifndef __Ball_H__
#define __Ball_H__

#include "GUI.h"
#include "WM.h"
#include "math.h"
#include "stdlib.h" // 为了生成随机数

/* 全局常量 */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define GAME_AREA_SIZE 240
#define JOYSTICK_AREA_X 240
#define JOYSTICK_BASE_RADIUS 40  // 大圆半径
#define JOYSTICK_SMALL_RADIUS 25 // 遥感小圆半径
#define JOYSTICK_PATH_SAMPLES 36 // 路径采样点数
#define BALL_RADIUS 10
#define FOOD_RADIUS 5            // 随机小球半径
#define MAX_BALL_RADIUS 30       // 游戏失败的最大半径
#define FOOD_TIMEOUT 10000        // 食物超时时间（毫秒）
/* 随机小球的最大数量 */
#define MAX_FOOD_COUNT 10

/* 小球位置 */
static int ball_x = GAME_AREA_SIZE / 2;
static int ball_y = GAME_AREA_SIZE / 2;
static float ball_radius = BALL_RADIUS;

/* 遥感基点与当前位置 */
static int joystick_base_x = JOYSTICK_AREA_X + 40;
static int joystick_base_y = SCREEN_HEIGHT / 2;
static int joystick_current_x = JOYSTICK_AREA_X + 40;
static int joystick_current_y = SCREEN_HEIGHT / 2;

/* 小球移动速度 */
static int ball_speed_x = 10;
static int ball_speed_y = 10;

/* 随机生成小球颜色 */
static int food_x = 0;
static int food_y = 0;
static GUI_COLOR food_color;
static int last_food_time = 0; // 上次生成小球的时间

static GUI_COLOR GetRandomColor() {
    return GUI_MAKE_COLOR(rand() % 0xFFFFFF);
}
/* 存储多个随机小球的位置、颜色信息 */
typedef struct {
    int x;
    int y;
    GUI_COLOR color;
} Food;

static Food foods[MAX_FOOD_COUNT];
static int food_count = 0;
/* 绘制游戏区域 */
void DrawGameArea(void);
/* 初始化随机小球 */
void GenerateFoods(void) ;

/* 绘制小球 */
void DrawBall(void) ;
/* 绘制随机小球 */
void DrawFood(void) ;
// 绘制基点大圆，线条加粗
void DrawThickCircle(int x, int y, int radius, int thickness, GUI_COLOR color) ;
/* 绘制遥感及半周路径 */
void DrawJoystick(void);
/* 检测碰撞 */
int CheckCollision1(int x1, int y1, int r1, int x2, int y2, int r2) ;
/* 检测主球是否吃到任何一个随机小球 */
int CheckFoodsCollision(void) ;
/*检测游戏结束*/
int CheckGameOver(void) ;
/* 更新小球位置 */
void UpdateBallPosition(void) ;
/* 游戏失败处理 */
void HandleGameOver(int reason) ;
/* 处理触摸事件 */
void HandleTouchEvent1(int x, int y) ;
/* 游戏重置 */
void ResetGame1(void) ;

/* 检测触摸状态并重置摇杆 */
void CheckJoystickState(void) ;

/* 游戏主循环 */
void BallBattleGame(void);

#endif
