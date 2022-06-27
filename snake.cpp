/*練習參考 https://www.796t.com/content/1549648095.html 並修改一些設定
音樂製作:朱嘉偉(朋友製作非本人) 音樂下載:https://drive.google.com/file/d/1lNvvjQwOBHQOROT7h2h5L03EP1MzwHXF/view?fbclid=IwAR3ikMualYEE48Qxb4aZthLC_zcz6Jaorl-w9C89DZWiy2RK5Rz7tLtVq7k
備用音樂載點:https://drive.google.com/file/d/1_txABIP9Uf_ov74yoerI1uYOSFYdCY_Y/view?usp=sharing 
遊戲試玩影片https://www.youtube.com/watch?v=oPNyFmXvkZA*/

#include<iostream>
#include<windows.h>
#include<deque>
#include<stdio.h>
#include <conio.h>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
HANDLE hOut;
COORD pos;

bool win = false;
int score;
int level;
void SetColor(int f = 7, int b = 0)
{
	unsigned short ForeColor = f + 16 * b;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon, ForeColor);
}
void gotoxy(short x, short y) //更改游標位置
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	pos = { x,y };
	SetConsoleCursorPosition(hOut, pos);
}
void HideCursor() //隱藏游標
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);


}
struct Snake
{
	char body;
	int position_x, position_y;
};
class Game {
private:
	char image;
	enum mapSize { width = 60, height = 30 };
	deque<Snake>snake;

	char hit = 'w';
	bool eat_Food = false;
	short food_x, food_y;
	int speed = 300;
	bool snake_state = true;

public:
	Game();
	void draw_Frame() //畫邊框
	{
		for (int i = 0; i < height; i++) {
			gotoxy(0, i);
			cout << "●";
			gotoxy(width, i);
			cout << "●";
		}
		for (int i = 0; i <= width; i += 2) {
			gotoxy(i, 0);
			cout << "●";
			gotoxy(i, height);
			cout << "●";
		}

	}
	void init_Snake()//初始化蛇蛇
	{
		snake.push_back({ '@',width / 2,height / 2 });
		for (int i = 0; i < 3; i++) {
			snake.push_back({ 'o',(height / 2) + 1 + i });
		}
		snake.push_back({ ' ',height / 2 + 4 });
	}
	void draw_Snake() //畫蛇
	{
		for (int i = 0; i < snake.size(); i++) {
			gotoxy(snake[i].position_x, snake[i].position_y);
			cout << snake[i].body;
		}
	}
	void clear_Tail() //清除蛇尾
	{
		int k = snake.size() - 1;
		gotoxy(snake[k].position_x, snake[k].position_y);
		cout << " ";
	}
	bool wrong_Location()//檢查食物位置
	{
		for (auto i : snake) {
			if (food_x == i.position_x && food_y == i.position_y)
				return 0;
		}
		return 1;
	}
	void draw_Food()  //畫食物
	{
		while (1) {
			food_x = rand() % (width - 4) + 2;
			food_y = rand() % (height - 2) + 1;
			if (wrong_Location() && food_x % 2 == 0)
				break;
		}
		gotoxy(food_x, food_y);
		cout << "9";


	}
	void judge_eatFood() //蛇有沒有吃到食物
	{
		if (food_x == snake[0].position_x && food_y == snake[0].position_y)
			eat_Food = true;
	}
	void judge_state() //蛇的狀態
	{
		if (snake.size() >= 2) {
			deque<Snake>::iterator iter = snake.begin() + 1;
			int x = (iter - 1)->position_x, y = (iter - 1)->position_y;
			for (; iter != snake.end(); ++iter) {
				if (iter->position_x == x && iter->position_y == y) {
					snake_state = false;
				}
			}
			if (snake[0].position_x == 1 || snake[0].position_x == 59 || snake[0].position_y == 0 || snake[0].position_y == 30) {
				snake_state = false;
			}

		}

	}
	void key_Down()//控制方向
	{
		char key = hit;
		if (_kbhit())
			hit = _getch();
		for (int i = snake.size() - 1; i > 0; i--) {
			snake[i].position_x = snake[i - 1].position_x;
			snake[i].position_y = snake[i - 1].position_y;
		}
		if (hit == 'a' || hit == 'A') {
			hit = 'a';
			snake[0].position_x--;
		}
		else if (hit == 'd' || hit == 'D')
		{
			hit = 'd';
			snake[0].position_x++;
		}
		else if (hit == 'w' || hit == 'W')
		{
			hit = 'w';
			snake[0].position_y--;
		}
		else if (hit == 's' || hit == 'S')
		{
			hit = 's';
			snake[0].position_y++;
		}

	}
	void show()//顯示分數和Level
	{
		gotoxy(65, 5);
		cout << "score:" << score;
		gotoxy(65, 6);
		cout << "Level:" << level;

	}
};
Game::Game() {
	HideCursor();
	srand(time(0));
	init_Snake();
	draw_Food();
	Snake tail;
	while (1) {
		mciSendString("play Hypers.mp3 repeat", 0, 0, 0);//撥放音樂
		draw_Frame();
		tail = snake.back();
		if (eat_Food) {
			snake.back().body = 'o';
			snake.push_back(tail);
			gotoxy(food_x, food_y);
			cout << " ";
			draw_Food();
			score++;
			if (score % 3 == 0) {
				speed *= 0.6;
				level++;
			}
			eat_Food = false;

		}
		if (level == 5) {
			win = true;
			break;
		}
		key_Down();
		draw_Snake();
		judge_state();
		if (!snake_state) {
			win = false;
			break;
		}
		judge_eatFood();
		Sleep(speed);
		clear_Tail();
		show();
	}
}
int main()
{
	system("mode con cols=100 lines=40");	
	system("title snake");
	mciSendString("open Hypers.mp3", 0, 0, 0);	
	gotoxy(26, 15);
	cout << "遊戲玩法:" << endl;
	gotoxy(26, 16);
	cout << "1.使用wasd(WASD)控制上下左右。";
	gotoxy(26, 17);
	cout << "2.@為蛇的頭部，o為身體。";
	gotoxy(26, 18);
	cout << "3.'9'為小老鼠，吃掉一隻加一分，每吃3隻等級就會提升。";
	gotoxy(26, 19);
	cout << "4.撞到邊界、身體遊戲失敗或是等級到達Level 5則遊戲獲勝。";
	gotoxy(26, 20);
	cout << "ENTER:開始遊戲";
	gotoxy(26, 21);
	cout << "ESC:離開遊戲。";
	
	while (1) {
		if (_getch() == 13) {
			
			SetColor(6, 0);
			int start = time(0);
			score = 0;
			level = 1;
			system("cls");
			Game game;
			system("cls");
			SetColor();
			if (win == true) {
				cout << "YOU WIN!!!" << endl;
			}
			else {
				
				cout << "YOU LOSS" << endl << "score:" << score << endl << "level:" << level << endl;
				
				
			}
			cout << "遊戲時間:";
			int end = time(0) - start;
			if (end < 60)
				cout << end << "秒";
			else
			{
				cout << end / 60 << "分";
				if (end % 60 != 0) {
					cout << end % 60 << "秒";
				}
			}
			cout << endl;
			break;
		}
		else if (_getch() == 27) {
			system("cls");
			cout << "Bye bye!";
			break;
		}
	}
}
