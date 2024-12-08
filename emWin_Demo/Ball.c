#include "GUI.h"
#include "WM.h"
#include "math.h"
#include "stdlib.h" // 为了生成随机数
#include "Ball.h"

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
 int gameover1 = 0;                  /* 游戏结束标志 */
/* 小球位置 */
//static int ball_x = GAME_AREA_SIZE / 2;
//static int ball_y = GAME_AREA_SIZE / 2;
//static float ball_radius = BALL_RADIUS;

///* 遥感基点与当前位置 */
//static int joystick_base_x = JOYSTICK_AREA_X + 40;
//static int joystick_base_y = SCREEN_HEIGHT / 2;
//static int joystick_current_x = JOYSTICK_AREA_X + 40;
//static int joystick_current_y = SCREEN_HEIGHT / 2;

///* 小球移动速度 */
//static int ball_speed_x = 10;
//static int ball_speed_y = 10;

///* 随机生成小球颜色 */
//static int food_x = 0;
//static int food_y = 0;
//static GUI_COLOR food_color;
//static int last_food_time = 0; // 上次生成小球的时间

//GUI_COLOR GetRandomColor() {
//    return GUI_MAKE_COLOR(rand() % 0xFFFFFF);
//}
///* 存储多个随机小球的位置、颜色信息 */
//typedef struct {
//    int x;
//    int y;
//    GUI_COLOR color;
//} Food;

//static Food foods[MAX_FOOD_COUNT];
//static int food_count = 0;
/* 绘制游戏区域 */
void DrawGameArea(void) {

    GUI_SetColor(GUI_BLACK);
    GUI_FillRect(0, 0, GAME_AREA_SIZE, GAME_AREA_SIZE);

    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_DrawRect(0, 0, GAME_AREA_SIZE, GAME_AREA_SIZE);
}
/* 初始化随机小球 */
void GenerateFoods(void) {
    food_count = (rand() % (MAX_FOOD_COUNT - 1)) + 2; // 随机生成2到MAX_FOOD_COUNT个小球
    for (int i = 0; i < food_count; i++) {
        foods[i].x = rand() % (GAME_AREA_SIZE - 2 * FOOD_RADIUS) + FOOD_RADIUS;
        foods[i].y = rand() % (GAME_AREA_SIZE - 2 * FOOD_RADIUS) + FOOD_RADIUS;
        foods[i].color = GetRandomColor();
    }
}


/* 绘制小球 */
void DrawBall(void) {
      GUI_SetColor(GUI_GREEN);
    GUI_FillCircle(ball_x, ball_y, (int)ball_radius);
}
/* 绘制随机小球 */
void DrawFood(void) {
    GUI_SetColor(food_color);
    GUI_FillCircle(food_x, food_y, FOOD_RADIUS);
}

// 绘制基点大圆，线条加粗
void DrawThickCircle(int x, int y, int radius, int thickness, GUI_COLOR color) {
    for (int i = 0; i < food_count; i++) {
        GUI_SetColor(foods[i].color);
        GUI_FillCircle(foods[i].x, foods[i].y, FOOD_RADIUS);
    }
}

/* 绘制遥感及半周路径 */
void DrawJoystick(void) {
 GUI_SetColor(GUI_LIGHTGRAY);
    GUI_FillRect(JOYSTICK_AREA_X, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // 判断方向并确定采样范围
    float start_angle = 0, end_angle = 0;
    int dx = joystick_current_x - joystick_base_x;
    int dy = joystick_current_y - joystick_base_y;

    if (dy < 0) { // 上移，绘制下半周
        start_angle = 3.15f;  // 180°
        end_angle = 2 * 3.15f; // 360°
    } else if (dy > 0) { // 下移，绘制上半周
        start_angle = 0;           // 0°
        end_angle = 3.15f;    // 180°
    } else if (dx > 0) { // 右移，绘制左半周
        start_angle = 3.15f / 2; // 90°
        end_angle = 3 * 3.15f / 2; // 270°
    } else if (dx < 0) { // 左移，绘制右半周
        start_angle = -3.15f / 2; // -90°
        end_angle = 3.15f / 2;   // 90°
    }

    // 绘制路径
    GUI_SetColor(GUI_BLACK);
    float angle_step = (end_angle - start_angle) / JOYSTICK_PATH_SAMPLES;
    for (int i = 0; i <= JOYSTICK_PATH_SAMPLES; i++) {
        float angle = start_angle + i * angle_step;

        // 同心圆上点的位置
        int path_x = joystick_current_x + JOYSTICK_SMALL_RADIUS * cos(angle);
        int path_y = joystick_current_y + JOYSTICK_SMALL_RADIUS * sin(angle);

        // 绘制路径线段
        GUI_DrawLine(joystick_base_x, joystick_base_y, path_x, path_y);
    }
	GUI_SetColor(GUI_DARKGREEN);
	DrawThickCircle(joystick_base_x, joystick_base_y, JOYSTICK_BASE_RADIUS, 3, GUI_ORANGE); // 设置线条厚度为3
  
	 GUI_SetColor(GUI_BLUE);
    GUI_FillCircle(joystick_current_x, joystick_current_y, JOYSTICK_SMALL_RADIUS);

    // 绘制当前遥感小黑实心圆
    GUI_SetColor(GUI_BLACK);
    GUI_FillCircle(joystick_current_x, joystick_current_y, JOYSTICK_SMALL_RADIUS - 5);

}
/* 检测碰撞 */
int CheckCollision1(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int distance_squared = dx * dx + dy * dy;
    int radius_sum = r1 + r2;
    return distance_squared <= radius_sum * radius_sum;
}
/* 检测主球是否吃到任何一个随机小球 */
int CheckFoodsCollision(void) {
    for (int i = 0; i < food_count; i++) {
        if (CheckCollision1(ball_x, ball_y, BALL_RADIUS, foods[i].x, foods[i].y, FOOD_RADIUS)) {
            // 吃到小球后移除当前小球
            for (int j = i; j < food_count - 1; j++) {
                foods[j] = foods[j + 1];
            }
            food_count--;
            ball_radius += 2; // 主球变大
            return 1; // 吃到小球
        }
    }
    return 0; // 未吃到小球
}
/*检测游戏结束*/
int CheckGameOver(void) {
   // int current_time = GUI_GetTime();
    if (ball_radius >= MAX_BALL_RADIUS) {
        return 1; // 主球半径过大失败
    }
//    if (current_time - last_food_time > FOOD_TIMEOUT) {
//        return 2; // 超时未吃到随机小球失败
//    }
    return 0; // 游戏未失败
}
/* 更新小球位置 */
void UpdateBallPosition(void) {
    ball_x += ball_speed_x;
    ball_y += ball_speed_y;

    if (ball_x - BALL_RADIUS < 0) ball_x = BALL_RADIUS;
    if (ball_x + BALL_RADIUS > GAME_AREA_SIZE) ball_x = GAME_AREA_SIZE - BALL_RADIUS;
    if (ball_y - BALL_RADIUS < 0) ball_y = BALL_RADIUS;
    if (ball_y + BALL_RADIUS > GAME_AREA_SIZE) ball_y = GAME_AREA_SIZE - BALL_RADIUS;
	
	 // 检测是否吃到随机小球
//    if (CheckCollision(ball_x, ball_y, BALL_RADIUS, food_x, food_y, FOOD_RADIUS)) {
//         ball_radius += 2; // 吃到后主球变大
//		GenerateFood(); // 生成新小球
		if (CheckFoodsCollision()) {
        if (food_count == 0) {
			 ball_radius += 2; // 吃到后主球变大
            GenerateFoods(); // 所有小球被吃完后重新生成
        }
    }
}
/* 游戏失败处理 */
void HandleGameOver(int reason) {
	
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_SetColor(GUI_LIGHTRED);
    if (reason == 1) {
        GUI_DispStringAt("GAME OVER!", 240, 5);
		 GUI_DispStringAt("overfeed!", 250, 25);
		gameover1=1;
    } else if (reason == 2) {
        GUI_DispStringHCenterAt("游戏失败：超时未吃小球!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }
    GUI_Delay(2000); // 停留2秒
}
/* 处理触摸事件 */
void HandleTouchEvent1(int x, int y) {
    if (x > JOYSTICK_AREA_X && x < SCREEN_WIDTH) {
        int dx = x - joystick_base_x;
        int dy = y - joystick_base_y;

        int distance = (int)sqrt(dx * dx + dy * dy);
        if (distance > JOYSTICK_BASE_RADIUS) {
            float scale = (float)JOYSTICK_BASE_RADIUS / distance;
            dx = (int)(dx * scale);
            dy = (int)(dy * scale);
        }

        joystick_current_x = joystick_base_x + dx;
        joystick_current_y = joystick_base_y + dy;

        if (dx > 10) {
            ball_speed_x = 8;
        } else if (dx < -10) {
            ball_speed_x = -8;
        } else {
            ball_speed_x = 0;
        }

        if (dy > 10) {
            ball_speed_y = 8;
        } else if (dy < -10) {
            ball_speed_y = -8;
        } else {
            ball_speed_y = 0;
        }
    }
}

/* 游戏重置 */
void ResetGame1(void) {
	gameover1=0;
    ball_x = GAME_AREA_SIZE / 2;
    ball_y = GAME_AREA_SIZE / 2;
	 ball_radius = BALL_RADIUS;
    joystick_current_x = joystick_base_x;
    joystick_current_y = joystick_base_y;
    ball_speed_x = 0;
    ball_speed_y = 0;
	 GenerateFoods(); // 生成初始随机小球
    GUI_Clear();
    DrawGameArea();
    DrawJoystick();
    DrawBall();
	  DrawFood();
	DrawThickCircle(joystick_base_x, joystick_base_y, JOYSTICK_BASE_RADIUS, 3, GUI_ORANGE);
}

/* 检测触摸状态并重置摇杆 */
void CheckJoystickState(void) {
    GUI_PID_STATE touchState;
    GUI_TOUCH_GetState(&touchState);

    if (touchState.Pressed == 0) {  // 如果没有触摸
        joystick_current_x = joystick_base_x;
        joystick_current_y = joystick_base_y;
        ball_speed_x = 0;
        ball_speed_y = 0;
    }
}

/* 游戏主循环 */
void BallBattleGame(void) {
	 // 初始化游戏背景
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();  // 清屏并填充为黑色背景
    ResetGame1();

    while (!gameover1) {
        GUI_TOUCH_Exec();
        GUI_Delay(30);

		 CheckJoystickState();  // 检查触摸状态
        UpdateBallPosition();

		int game_over = CheckGameOver();
		gameover1=game_over;
        if (game_over) {
            HandleGameOver(game_over);
            
        }
		
        GUI_ClearRect(0, 0, GAME_AREA_SIZE, GAME_AREA_SIZE);
		 // DrawGameArea();
        DrawBall();
		DrawFood();
        DrawJoystick();
    }
	
}

///* 游戏入口 */
//void emwin_main(void) {
//    WM_SetCreateFlags(WM_CF_MEMDEV);
//    GUI_Init();
//    BallBattleGame();
//}
