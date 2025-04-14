#pragma once
#include "CONSTANTS.h"
#include "Grid.h"

#define PREVIEW_WIDTH 8 
#define PREVIEW_HEIGHT 4

class PreviewBlockGrid {
private:
	GridBlock grid[PREVIEW_HEIGHT][PREVIEW_WIDTH];

public:
	Coord refPoint;

	PreviewBlockGrid(Coord refPoint);
	const GridBlock& GetCell(int x, int y);

	void SetBlock(const Tetrominoe& block);
	void InitGrid();
};
