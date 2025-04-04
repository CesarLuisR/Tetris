#pragma once

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/ioctl.h>
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
