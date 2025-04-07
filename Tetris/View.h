#pragma once
#include <chrono>
#include "Grid.h"
#include "ConsoleUtils.h"

struct Edge {
	int InitX;
	int InitY;
};

namespace View {
	void RenderGrid(Grid& grid);
	void RenderScoreAndSpeed(int score, double SPEED, ConsoleSize cSize);
	void RenderFinal(int score, ConsoleSize cSize);
	void RenderInit(ConsoleSize cSize);
	Edge GetEdge(int WWidth, int WHeight);
}
