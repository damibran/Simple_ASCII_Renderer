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
		screen.resize(XMAX * YMAX);
		for (int y = 0; y < YMAX; ++y)
		{
			for (int x = 0; x < XMAX; ++x)
				screen[y * XMAX + x] = color[0];
		}
	}
	void screen_refresh() // Обновление экрана
	{
		wchar_t s[256];
		swprintf_s(s, 256, L"My little ASCII Renderrer - FPS: %3.2f", 1.0f / deltaTime);
		SetConsoleTitle(s);
		screen[XMAX * YMAX - 1] = '\0';
		WriteConsoleOutputCharacter(hConsle, screen.data(), XMAX * YMAX, { 0,0 }, &dwBytesWritten);
	}
	void debug_massage(std::string s)
	{
		swprintf_s(screen.data(), 256, L"%s", s.c_str());
	}
	void debug_float(float v)
	{
		swprintf_s(screen.data(), 7, L"%3.3f", v);
	}
	void screen_clear()
	{
		for (int y = 0; y < YMAX; ++y)
		{
			for (int x = 0; x < XMAX; ++x)
				screen[y * XMAX + x] = color[0];
		}
	}
	void put_point(int a, int b, float ck)
	{
		screen[(YMAX - b) * XMAX + a] = color[std::round(ck * color.size())-1];
	}


private:
	HANDLE hConsle;
	DWORD dwBytesWritten;
	std::vector<wchar_t> screen;
	std::vector<char> color = { ' ', '.', '-',',',':', '=', '+', '%', '*','8', '#', '@','B'};
};