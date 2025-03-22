#pragma once

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/iocth.h>
#include <cstdio>
#include <termios.h>
#endif

struct ConsoleSize {
	int width;
	int height;
};

ConsoleSize GetConsoleSize();
void GoTo(int x, int y);
char GetKey();
