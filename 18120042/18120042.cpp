// snake2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "Windows.h"
#include "time.h"
#include "thread"
#include "conio.h"

using namespace std;

HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX)& ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Hang so

#define MAX_SIZE_SNAKE 30
#define MAX_SIZE_FOOD 4
#define MAX_SPEED 4
//Bien toan cuc
POINT snake[30]; //Con ran
POINT food[4]; // Thuc an
int CHAR_LOCK;//bien xac dinh huong khong the di chuyen (o mot thoi diem co mot huong ran khong the di chuyen)
int MOVING;//Bien xa dinh huong  di chuyen cua ran (o mot thoi diem co ba  huong ran co the di chuyen)
int SPEED;// Co the hieu nhe level, level  cang cao thi toc do cang nhanh
int HeighConSole, WIDTH_CONSOLE;// Do rong va do cao man hinh console
int FOOD_INDEX; // Chi so food hien hanh dang co tren man hinh
int SIZE_SNAKE; // Kich thuoc snake, luc dau co 6 toi da la 10
int STATE = 1; // Trang thai song chet cua ran
char nameSnake[] = { '1', '8', '1', '2', '0', '0', '4', '2' };

POINT gate[6] = { 0 };
int temp;

bool IsValid(int x, int y)
{
	for (int i = 0; i < SIZE_SNAKE; i++)
	if (snake[i].x == x && snake[i].y == y)
		return false;
	return true;
}

void GenerateFood()
{
	int x, y;
	srand(time(NULL));
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
	{
		do
		{
			x = rand() % (WIDTH_CONSOLE - 5) + 4;
			y = rand() % (HeighConSole - 5) + 4;
		} while (IsValid(x, y) == false);
		food[i] = { x, y };
	}
}

void ResetData() {
	//Khoi tao cac gia tri toan cuc
	CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70, HeighConSole = 25, SIZE_SNAKE = 6;
	//khoi tao gia tri mac dinh cho snake
	snake[0] = { 10, 5 }; snake[1] = { 11, 5 };
	snake[2] = { 12, 5 }; snake[3] = { 13, 5 };
	snake[4] = { 14, 5 }; snake[5] = { 15, 5 };
	gate[0].x = 0; gate[0].y = 0;
	GenerateFood();//Tao mang food
}

void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)
{
	GotoXY(x, y);
	printf("%c", 201);
	for (int i = 1; i < width; i++)
		printf("%c", 205);
	printf("%c", 203);
	GotoXY(x, height + y);
	printf("%c", 200);
	for (int i = 1; i < width; i++)
		printf("%c", 205);
	printf("%c", 202);
	for (int i = y + 1; i < height + y; i++)
	{
		GotoXY(x, i); printf("%c", 186);
		GotoXY(x + width, i); printf("%c", 186);
	}
	GotoXY(curPosX, curPosY);
}

void drawInfoFrame()
{
	GotoXY(WIDTH_CONSOLE + 1, 0);
	for (int i = 1; i < 119 - WIDTH_CONSOLE - 1; i++)
		printf("%c", 205);
	GotoXY(WIDTH_CONSOLE + 120, 0); printf("%c", 187);
	GotoXY(WIDTH_CONSOLE + 1, HeighConSole);
	for (int i = 1; i < 118 - WIDTH_CONSOLE; i++)
		printf("%c", 205);
	GotoXY(WIDTH_CONSOLE + 120, HeighConSole); printf("%c", 188);
	for (int i = 1; i < HeighConSole; i++)
	{
		GotoXY(118, i);
		printf("%c", 186);
	}
	GotoXY(WIDTH_CONSOLE + 13, 3); cout << "Menu";
	GotoXY(WIDTH_CONSOLE + 10, 5); cout << ">> T   (upload)";
	GotoXY(WIDTH_CONSOLE + 10, 6); cout << ">> L   (save)";
	GotoXY(WIDTH_CONSOLE + 10, 7); cout << ">> P   (pause)";
	GotoXY(WIDTH_CONSOLE + 10, 8); cout << ">> R   (restart)";
	GotoXY(WIDTH_CONSOLE + 10, 9); cout << ">> Esc (exit)";
	GotoXY(0, 0);
}

void PrePlay()
{
	system("cls");
	WIDTH_CONSOLE = 70, HeighConSole = 25;
	GotoXY(27, HeighConSole / 2 - 2); cout << "|-------------------------------|";
	GotoXY(27, HeighConSole / 2 - 1); cout << "| Nhap T de upload game         |";
	GotoXY(27, HeighConSole / 2);     cout << "| Phim bat ky de bat dau choi   |";
	GotoXY(27, HeighConSole / 2 + 1); cout << "|                               |";
	GotoXY(27, HeighConSole / 2 + 2); cout << "|-------------------------------|";
}

void generateGate()
{
	int x, y;
	srand(time(NULL));
	do{
		x = rand() % (WIDTH_CONSOLE - 5) + 3;
		y = rand() % (HeighConSole - 30) + 10;
	} while (IsValid(x, y) == false);
	gate[0] = { x - 1, y + 1 };
	gate[1] = { x - 1, y };
	gate[2] = { x, y };
	gate[3] = { x + 1, y };
	gate[4] = { x + 1, y + 1 };
	gate[5] = { x, y + 1 };
}

void drawGate()
{
	for (int i = 0; i < 5; i++)
	{
		GotoXY(gate[i].x, gate[i].y);
		cout << "o";
	}
}

void cleanGate()
{
	for (int i = 0; i < 5; i++)
	{
		GotoXY(gate[i].x, gate[i].y);
		cout << " ";
		gate[i].x = 0; gate[i].y = 0;
	}
}

void StartGame()
{
	system("cls");
	ResetData(); //Khoi tao du lieu goc
	DrawBoard(0, 0, WIDTH_CONSOLE, HeighConSole); // Ve man hinh game
	drawInfoFrame();
	STATE = 1;//bat dau cho thead chay
}

//Ham thoat game
void ExitGame(HANDLE t)
{
	system("cls");
	exit(0);
}

//Ham dung game
void PauseGame(HANDLE t)
{
	SuspendThread(t);
}

//Ham cap nhat du lieu toan cuc
void Eat()
{
	snake[SIZE_SNAKE] = food[FOOD_INDEX];
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
	{
		temp = SIZE_SNAKE;
		generateGate();
		drawGate();
	}
	else
	{
		FOOD_INDEX++;
		SIZE_SNAKE++;
	}
}

//Ham xu ly ran chet
void ProcessDead()
{
	STATE = 0;
	int k = 0;
	while (k < 5)
	{
		SetConsoleTextAttribute(consoleHandle, rand() % 15 + 1);
		int j = 0;
		for (int i = 0; i < SIZE_SNAKE; i++)
		{
			GotoXY(snake[i].x, snake[i].y);
			printf("%c", nameSnake[j++]);
			if (j > 6) j = 0;
			Sleep(50);
		}
		k++;
	}
	GotoXY(0, HeighConSole + 2);
	printf("Dead, type y to continue or anykey to exit");
}

//ham ve man hinh

void DrawFood()
{
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	printf("%c", 206);
}

void DrawSnake(char* str)
{
	int j = 0;
	for (int i = 0; i < SIZE_SNAKE; i++)
	{
		GotoXY(snake[i].x, snake[i].y);
		printf("%c", str[j++]);
		if (j > 7)
			j = 0;
	}
}

bool touchBody()
{
	for (int i = 0; i < SIZE_SNAKE - 2; i++)
	if (snake[i].x == snake[SIZE_SNAKE - 1].x && snake[i].y == snake[SIZE_SNAKE - 1].y)
		return true;
	return false;
}

bool touchGate()
{
	for (int i = 0; i < 5; i++)
	if (gate[i].x == snake[SIZE_SNAKE - 1].x && gate[i].y == snake[SIZE_SNAKE - 1].y)
		return true;
	return false;
}

void MoveRight()
{
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1 && touchGate())
		ProcessDead();
	if (touchBody())
		ProcessDead();
	if (snake[SIZE_SNAKE - 1].x + 1 == WIDTH_CONSOLE)
		ProcessDead();
	else
	{
		if (snake[SIZE_SNAKE - 1].x + 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x++;
	}
}

void MoveLeft()
{
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1 && touchGate())
		ProcessDead();
	if (touchBody())
		ProcessDead();
	if (snake[SIZE_SNAKE - 1].x - 1 == 0)
		ProcessDead();
	else
	{
		if (snake[SIZE_SNAKE - 1].x - 1 == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].x--;
	}
}

void MoveDown()
{
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1 && touchGate())
		ProcessDead();
	if (touchBody())
		ProcessDead();
	if (snake[SIZE_SNAKE - 1].y + 1 == HeighConSole)
		ProcessDead();
	else
	{
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y + 1 == food[FOOD_INDEX].y)
			Eat();
		for (int i = 0; i < SIZE_SNAKE - 1; i++)
		{
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y++;
	}
}

void MoveUp()
{
	if (FOOD_INDEX == MAX_SIZE_FOOD - 1){
		for (int i = 0; i < 5; i++){
			if (gate[i].x == snake[SIZE_SNAKE - 1].x&&gate[i].y == snake[SIZE_SNAKE - 1].y)
			{
				ProcessDead();
				break;
			}
		}
		if (snake[SIZE_SNAKE - 1].x == gate[5].x&&snake[SIZE_SNAKE - 1].y == gate[5].y) SIZE_SNAKE--;
		if (SIZE_SNAKE == 0)	{
			SPEED++;
			if (SPEED == MAX_SPEED)
			{
				cleanGate();
				ResetData();
			}
			else{
				FOOD_INDEX = 0;
				SIZE_SNAKE = temp++;
				cleanGate();
			}
		}
	}
	for (int i = 0; i < SIZE_SNAKE - 2; i++)
	{
		if (snake[i].x == snake[SIZE_SNAKE - 1].x && snake[i].y == snake[SIZE_SNAKE - 1].y - 1)
		{
			ProcessDead();
			break;
		}
	}
	if (snake[SIZE_SNAKE - 1].y - 1 == 0) {
		ProcessDead();
	}
	else {
		if (snake[SIZE_SNAKE - 1].x == food[FOOD_INDEX].x && snake[SIZE_SNAKE - 1].y - 1 == food[FOOD_INDEX].y) {
			Eat();
		}
		for (int i = 0; i < SIZE_SNAKE - 1; i++) {
			snake[i].x = snake[i + 1].x;
			snake[i].y = snake[i + 1].y;
		}
		snake[SIZE_SNAKE - 1].y--;
	}
}

//thu tuc cho thread
void ThreadFunc()
{
	while (1)
	{
		if (STATE == 1) //Neu snake van con song
		{
			DrawSnake(" ");
			if (gate[0].x == 0 && gate[0].y == 0)
				DrawFood();
			switch (MOVING)
			{
			case 'A':
				MoveLeft();
				break;
			case 'D':
				MoveRight();
				break;
			case 'W':
				MoveUp();
				break;
			case 'S':
				MoveDown();
				break;
			}
			DrawSnake(nameSnake);
			Sleep(100 / SPEED + 50);//Ham ngu theo toc do speech
		}
	}
}

void saveGame()
{
	char nameGame[40];
	GotoXY(27, HeighConSole / 2 - 2);
	cout << "|-------------------------|";
	GotoXY(27, HeighConSole / 2 - 1);
	cout << "|     Save Game!          |";
	GotoXY(27, HeighConSole / 2);
	cout << "|     Name:               |";
	GotoXY(27, HeighConSole / 2 + 1);
	cout << "|                         |";
	GotoXY(27, HeighConSole / 2 + 2);
	cout << "|-------------------------|";
	GotoXY(39, HeighConSole / 2);
	gets_s(nameGame);
	strcat(nameGame, ".txt");
	FILE *f = fopen(nameGame, "w");
	fprintf(f, "%d\n", FOOD_INDEX);
	fprintf(f, "%d %d\n", food[FOOD_INDEX].x, food[FOOD_INDEX].y);
	fprintf(f, "%d\n", SPEED);
	fprintf(f, "%d\n", SIZE_SNAKE);
	for (int i = 0; i < SIZE_SNAKE; i++)
		fprintf(f, "%d %d\n", snake[i].x, snake[i].y);
	for (int i = 0; i < 5; i++)
		fprintf(f, "%d %d\n", gate[i].x, gate[i].y);
	fclose(f);
	GotoXY(0, 0);
}

void cleanSaveGame()
{
	GotoXY(27, HeighConSole / 2 - 2);
	cout << "                           ";
	GotoXY(27, HeighConSole / 2 - 1);
	cout << "                           ";
	GotoXY(27, HeighConSole / 2);
	cout << "                           ";
	GotoXY(27, HeighConSole / 2 + 1);
	cout << "                           ";
	GotoXY(27, HeighConSole / 2 + 2);
	cout << "                           ";
	DrawSnake(nameSnake);
}

void loadGame()
{
	DrawSnake(" ");
	GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y); cout << " ";
	char nameGame[40];
	system("cls");
	GotoXY(27, HeighConSole / 2 - 5); cout << "Nhap 'CHOI LUON' de bat dau choi";
	GotoXY(27, HeighConSole / 2 - 2); cout << "|-------------------------------|";
	GotoXY(27, HeighConSole / 2 - 1); cout << "|         Upload Game!          |";
	GotoXY(27, HeighConSole / 2);     cout << "|         Name:                 |";
	GotoXY(27, HeighConSole / 2 + 1); cout << "|                               |";
	GotoXY(27, HeighConSole / 2 + 2); cout << "|-------------------------------|";
	GotoXY(43, HeighConSole / 2); gets_s(nameGame);

	if (strcmp(nameGame, "CHOI LUON") == 0)
	{
		StartGame();
	}
	else
	{
		strcat(nameGame, ".txt");
		system("cls");
		FILE *f = fopen(nameGame, "r");
		fseek(f, 0, 0L);
		fscanf(f, "%d\n", &FOOD_INDEX);
		fscanf(f, "%d %d\n", &food[FOOD_INDEX].x, &food[FOOD_INDEX].y);
		fscanf(f, "%d\n", &SPEED);
		fscanf(f, "%d\n", &SIZE_SNAKE);
		for (int i = 0; i < SIZE_SNAKE; i++)
			fscanf(f, "%d %d\n", &snake[i].x, &snake[i].y);
		for (int i = 0; i < 5; i++)
			fscanf(f, "%d %d\n", &gate[i].x, &gate[i].y);
		fclose(f);
		if (FOOD_INDEX == MAX_SIZE_FOOD - 1) drawGate();
	}
}

void cleanLoadGame()
{
	GotoXY(27, HeighConSole / 2 - 2);
	cout << "                                 ";
	GotoXY(27, HeighConSole / 2 - 1);
	cout << "                                 ";
	GotoXY(27, HeighConSole / 2);
	cout << "                                 ";
	GotoXY(27, HeighConSole / 2 + 1);
	cout << "                                 ";
	GotoXY(27, HeighConSole / 2 + 2);
	cout << "                                 ";
	DrawSnake(nameSnake);
}

void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}

void main()
{
	Nocursortype();
	PrePlay();
	int temp = toupper(getch());
	if (temp == 'T')
	{
		system("cls");
		ResetData(); //Khoi tao du lieu goc
		loadGame();
		cleanLoadGame();
		DrawBoard(0, 0, WIDTH_CONSOLE, HeighConSole); // Ve man hinh game
		drawInfoFrame();
	}
	else
		StartGame();
	thread t1(ThreadFunc); //Tao thread cho snake
	HANDLE handle_t1 = t1.native_handle(); //Lay handle cua thread
	while (1)
	{
		temp = toupper(getch());
		if (STATE == 1)
		{
			if (temp == 'P')
			{
				PauseGame(handle_t1);
			}
			else if (temp == 27)
			{
				ExitGame(handle_t1);
				return;
			}
			else if (temp == 'L')
			{
				PauseGame(handle_t1); //pause roi choi tiep thôi
				saveGame();
				Sleep(500);
				cleanSaveGame();
			}
			else if (temp == 'T')
			{
				PauseGame(handle_t1);
				loadGame();
				Sleep(500);
				DrawBoard(0, 0, WIDTH_CONSOLE, HeighConSole); // Ve man hinh game
				drawInfoFrame();
				cleanLoadGame();
			}
			else if (temp == 'R')
			{
				system("cls");
				ResetData();
				DrawBoard(0, 0, WIDTH_CONSOLE, HeighConSole);
				drawInfoFrame();
				STATE = 1;
			}
			else
			{
				ResumeThread(handle_t1);
				if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
				{
					if (temp == 'D') CHAR_LOCK = 'A';
					else if (temp == 'W') CHAR_LOCK = 'S';
					else if (temp == 'S') CHAR_LOCK = 'W';
					else CHAR_LOCK = 'D';
					MOVING = temp;
				}
			}
		}
		else
		{
			if (temp == 'Y')
				StartGame();
			else
			{
				ExitGame(handle_t1);
				return;
			}
		}
	}
}

