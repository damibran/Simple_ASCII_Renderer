#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdio>
#include<vector>

class Screen
{
public:
	int XMAX;
	int YMAX;
	float deltaTime;
	Screen(int Width, int Height)
	{
		hConsle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		dwBytesWritten = 0;
		SetConsoleActiveScreenBuffer(hConsle);

		XMAX = Width;
		YMAX = Height;
		screen = new wchar_t[XMAX * YMAX];
		for (int y = 0; y < YMAX; ++y)
		{
			for (int x = 0; x < XMAX; ++x)
				screen[y * XMAX + x] = color[0];
		}
	}
	~Screen()
	{
		delete[] screen;
	}
	void screen_refresh() // Обновление экрана
	{
		wchar_t s[256];
		swprintf_s(s, 256, L"My little ASCII Renderrer - FPS: %3.2f", 1.0f / deltaTime);
		SetConsoleTitle(s);
		screen[XMAX * YMAX - 1] = '\0';
		WriteConsoleOutputCharacter(hConsle, screen, XMAX * YMAX, { 0,0 }, &dwBytesWritten);
	}
	void debug_massage(std::string s)
	{
		swprintf_s(screen, 256, L"%s", s.c_str());
	}
	void debug_float(float v)
	{
		swprintf_s(screen, 7, L"%3.3f", v);
	}
	void screen_clear()
	{
		for (int y = 0; y < YMAX; ++y)
		{
			for (int x = 0; x < XMAX; ++x)
				screen[y * XMAX + x] = color[0];
		}
	}
	bool on_screen(int a, int b) // проверка попадания точки на экран
	{
		return (0 <= a && a < XMAX && 0 < b && b < YMAX);
	}
	void put_point(int a, int b, float ck)
	{
		if (on_screen(a, b)) screen[(YMAX - b) * XMAX + a] = color[std::round(ck * color.size())-1];
	}
	//void put_point(int a, int b)
	//{
	//	if (on_screen(a, b)) screen[(YMAX - b) * XMAX + a] = black;
	//}
	//void put_line(int x0, int y0, int x1, int y1)
	//{
	//	int dx = 1;
	//	int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
	//	int dy = 1;
	//	int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
	//	int two_a = 2 * a;
	//	int two_b = 2 * b;
	//	int xcrit = -b + two_a;
	//	int eps = 0;
	//	for (;;) { //Формирование прямой линии по точкам
	//		put_point(x0, y0);
	//		if (x0 == x1 && y0 == y1) break;
	//		if (eps <= xcrit) x0 += dx, eps += two_b;
	//		if (eps >= a || a < b) y0 += dy, eps -= two_a;
	//	}
	//}

private:
	HANDLE hConsle;
	DWORD dwBytesWritten;
	wchar_t* screen;
	std::vector<char> color = { ' ', '.', '-',',',':', '=', '+', '%', '*','8', '#', '@','B'};
	//std::vector<char> color = { ' ', '.', '\'', '`', '^', '"', ',', ':', ';', 'I', 'l',
	//	'!', 'i', '>', '<', '~', '+', '_', '-', '?', ']', '[', '}', '{', '1', ')', '(',
	//	'|', '\\', '/', 't', 'f', 'j', 'r', 'x', 'n', 'u', 'v', 'c', 'z', 'X', 'Y', 'U',
	//	'J', 'C', 'L', 'Q', '0', 'O', 'Z', 'm', 'w', 'q', 'p', 'd', 'b', 'k', 'h', 'a', 
	//	'o', '*', '#', 'M', 'W', '&', '8', '%', 'B', '@', '$' };

};