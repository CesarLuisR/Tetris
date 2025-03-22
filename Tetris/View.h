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
	void RenderTime(std::chrono::steady_clock::time_point& time);
	Edge GetEdge(int WWidth, int WHeight);
}
