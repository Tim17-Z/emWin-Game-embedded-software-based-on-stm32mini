#include "GUI.h"
#include "WM.h"
#include "math.h"
#include "stdlib.h" // Ϊ�����������
#include "Ball.h"

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
 int gameover1 = 0;                  /* ��Ϸ������־ */
/* С��λ�� */
//static int ball_x = GAME_AREA_SIZE / 2;
//static int ball_y = GAME_AREA_SIZE / 2;
//static float ball_radius = BALL_RADIUS;

///* ң�л����뵱ǰλ�� */
//static int joystick_base_x = JOYSTICK_AREA_X + 40;
//static int joystick_base_y = SCREEN_HEIGHT / 2;
//static int joystick_current_x = JOYSTICK_AREA_X + 40;
//static int joystick_current_y = SCREEN_HEIGHT / 2;

///* С���ƶ��ٶ� */
//static int ball_speed_x = 10;
//static int ball_speed_y = 10;

///* �������С����ɫ */
//static int food_x = 0;
//static int food_y = 0;
//static GUI_COLOR food_color;
//static int last_food_time = 0; // �ϴ�����С���ʱ��

//GUI_COLOR GetRandomColor() {
//    return GUI_MAKE_COLOR(rand() % 0xFFFFFF);
//}
///* �洢������С���λ�á���ɫ��Ϣ */
//typedef struct {
//    int x;
//    int y;
//    GUI_COLOR color;
//} Food;

//static Food foods[MAX_FOOD_COUNT];
//static int food_count = 0;
/* ������Ϸ���� */
void DrawGameArea(void) {

    GUI_SetColor(GUI_BLACK);
    GUI_FillRect(0, 0, GAME_AREA_SIZE, GAME_AREA_SIZE);

    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_DrawRect(0, 0, GAME_AREA_SIZE, GAME_AREA_SIZE);
}
/* ��ʼ�����С�� */
void GenerateFoods(void) {
    food_count = (rand() % (MAX_FOOD_COUNT - 1)) + 2; // �������2��MAX_FOOD_COUNT��С��
    for (int i = 0; i < food_count; i++) {
        foods[i].x = rand() % (GAME_AREA_SIZE - 2 * FOOD_RADIUS) + FOOD_RADIUS;
        foods[i].y = rand() % (GAME_AREA_SIZE - 2 * FOOD_RADIUS) + FOOD_RADIUS;
        foods[i].color = GetRandomColor();
    }
}


/* ����С�� */
void DrawBall(void) {
      GUI_SetColor(GUI_GREEN);
    GUI_FillCircle(ball_x, ball_y, (int)ball_radius);
}
/* �������С�� */
void DrawFood(void) {
    GUI_SetColor(food_color);
    GUI_FillCircle(food_x, food_y, FOOD_RADIUS);
}

// ���ƻ����Բ�������Ӵ�
void DrawThickCircle(int x, int y, int radius, int thickness, GUI_COLOR color) {
    for (int i = 0; i < food_count; i++) {
        GUI_SetColor(foods[i].color);
        GUI_FillCircle(foods[i].x, foods[i].y, FOOD_RADIUS);
    }
}

/* ����ң�м�����·�� */
void DrawJoystick(void) {
 GUI_SetColor(GUI_LIGHTGRAY);
    GUI_FillRect(JOYSTICK_AREA_X, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // �жϷ���ȷ��������Χ
    float start_angle = 0, end_angle = 0;
    int dx = joystick_current_x - joystick_base_x;
    int dy = joystick_current_y - joystick_base_y;

    if (dy < 0) { // ���ƣ������°���
        start_angle = 3.15f;  // 180��
        end_angle = 2 * 3.15f; // 360��
    } else if (dy > 0) { // ���ƣ������ϰ���
        start_angle = 0;           // 0��
        end_angle = 3.15f;    // 180��
    } else if (dx > 0) { // ���ƣ����������
        start_angle = 3.15f / 2; // 90��
        end_angle = 3 * 3.15f / 2; // 270��
    } else if (dx < 0) { // ���ƣ������Ұ���
        start_angle = -3.15f / 2; // -90��
        end_angle = 3.15f / 2;   // 90��
    }

    // ����·��
    GUI_SetColor(GUI_BLACK);
    float angle_step = (end_angle - start_angle) / JOYSTICK_PATH_SAMPLES;
    for (int i = 0; i <= JOYSTICK_PATH_SAMPLES; i++) {
        float angle = start_angle + i * angle_step;

        // ͬ��Բ�ϵ��λ��
        int path_x = joystick_current_x + JOYSTICK_SMALL_RADIUS * cos(angle);
        int path_y = joystick_current_y + JOYSTICK_SMALL_RADIUS * sin(angle);

        // ����·���߶�
        GUI_DrawLine(joystick_base_x, joystick_base_y, path_x, path_y);
    }
	GUI_SetColor(GUI_DARKGREEN);
	DrawThickCircle(joystick_base_x, joystick_base_y, JOYSTICK_BASE_RADIUS, 3, GUI_ORANGE); // �����������Ϊ3
  
	 GUI_SetColor(GUI_BLUE);
    GUI_FillCircle(joystick_current_x, joystick_current_y, JOYSTICK_SMALL_RADIUS);

    // ���Ƶ�ǰң��С��ʵ��Բ
    GUI_SetColor(GUI_BLACK);
    GUI_FillCircle(joystick_current_x, joystick_current_y, JOYSTICK_SMALL_RADIUS - 5);

}
/* �����ײ */
int CheckCollision1(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int distance_squared = dx * dx + dy * dy;
    int radius_sum = r1 + r2;
    return distance_squared <= radius_sum * radius_sum;
}
/* ��������Ƿ�Ե��κ�һ�����С�� */
int CheckFoodsCollision(void) {
    for (int i = 0; i < food_count; i++) {
        if (CheckCollision1(ball_x, ball_y, BALL_RADIUS, foods[i].x, foods[i].y, FOOD_RADIUS)) {
            // �Ե�С����Ƴ���ǰС��
            for (int j = i; j < food_count - 1; j++) {
                foods[j] = foods[j + 1];
            }
            food_count--;
            ball_radius += 2; // ������
            return 1; // �Ե�С��
        }
    }
    return 0; // δ�Ե�С��
}
/*�����Ϸ����*/
int CheckGameOver(void) {
   // int current_time = GUI_GetTime();
    if (ball_radius >= MAX_BALL_RADIUS) {
        return 1; // ����뾶����ʧ��
    }
//    if (current_time - last_food_time > FOOD_TIMEOUT) {
//        return 2; // ��ʱδ�Ե����С��ʧ��
//    }
    return 0; // ��Ϸδʧ��
}
/* ����С��λ�� */
void UpdateBallPosition(void) {
    ball_x += ball_speed_x;
    ball_y += ball_speed_y;

    if (ball_x - BALL_RADIUS < 0) ball_x = BALL_RADIUS;
    if (ball_x + BALL_RADIUS > GAME_AREA_SIZE) ball_x = GAME_AREA_SIZE - BALL_RADIUS;
    if (ball_y - BALL_RADIUS < 0) ball_y = BALL_RADIUS;
    if (ball_y + BALL_RADIUS > GAME_AREA_SIZE) ball_y = GAME_AREA_SIZE - BALL_RADIUS;
	
	 // ����Ƿ�Ե����С��
//    if (CheckCollision(ball_x, ball_y, BALL_RADIUS, food_x, food_y, FOOD_RADIUS)) {
//         ball_radius += 2; // �Ե���������
//		GenerateFood(); // ������С��
		if (CheckFoodsCollision()) {
        if (food_count == 0) {
			 ball_radius += 2; // �Ե���������
            GenerateFoods(); // ����С�򱻳������������
        }
    }
}
/* ��Ϸʧ�ܴ��� */
void HandleGameOver(int reason) {
	
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_SetColor(GUI_LIGHTRED);
    if (reason == 1) {
        GUI_DispStringAt("GAME OVER!", 240, 5);
		 GUI_DispStringAt("overfeed!", 250, 25);
		gameover1=1;
    } else if (reason == 2) {
        GUI_DispStringHCenterAt("��Ϸʧ�ܣ���ʱδ��С��!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }
    GUI_Delay(2000); // ͣ��2��
}
/* �������¼� */
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

/* ��Ϸ���� */
void ResetGame1(void) {
	gameover1=0;
    ball_x = GAME_AREA_SIZE / 2;
    ball_y = GAME_AREA_SIZE / 2;
	 ball_radius = BALL_RADIUS;
    joystick_current_x = joystick_base_x;
    joystick_current_y = joystick_base_y;
    ball_speed_x = 0;
    ball_speed_y = 0;
	 GenerateFoods(); // ���ɳ�ʼ���С��
    GUI_Clear();
    DrawGameArea();
    DrawJoystick();
    DrawBall();
	  DrawFood();
	DrawThickCircle(joystick_base_x, joystick_base_y, JOYSTICK_BASE_RADIUS, 3, GUI_ORANGE);
}

/* ��ⴥ��״̬������ҡ�� */
void CheckJoystickState(void) {
    GUI_PID_STATE touchState;
    GUI_TOUCH_GetState(&touchState);

    if (touchState.Pressed == 0) {  // ���û�д���
        joystick_current_x = joystick_base_x;
        joystick_current_y = joystick_base_y;
        ball_speed_x = 0;
        ball_speed_y = 0;
    }
}

/* ��Ϸ��ѭ�� */
void BallBattleGame(void) {
	 // ��ʼ����Ϸ����
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();  // ���������Ϊ��ɫ����
    ResetGame1();

    while (!gameover1) {
        GUI_TOUCH_Exec();
        GUI_Delay(30);

		 CheckJoystickState();  // ��鴥��״̬
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

///* ��Ϸ��� */
//void emwin_main(void) {
//    WM_SetCreateFlags(WM_CF_MEMDEV);
//    GUI_Init();
//    BallBattleGame();
//}
