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
	void RenderScoreAndSpeed(int score, double SPEED);
	void RenderFinal(int score);
	Edge GetEdge(int WWidth, int WHeight);
}
