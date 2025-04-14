#include "PreviewBlockGrid.h"

PreviewBlockGrid::PreviewBlockGrid(Coord refPoint) {
	this->refPoint.x = refPoint.x;
	this->refPoint.y = refPoint.y - 4;
	InitGrid();
}

const GridBlock& PreviewBlockGrid::GetCell(int x, int y) {
	return grid[x][y];
}

void PreviewBlockGrid::SetBlock(const Tetrominoe& block) {
	int gridCenter = PREVIEW_WIDTH / 2; 
	int yAxis = 2;

	InitGrid();
	for (const Coord& cd : block.GetShape()) {
		int x = cd.x + gridCenter;
		int y = cd.y + yAxis;

		grid[y][x] = GridBlock(block.GetType(), block.m_Color, block.cellId);
	}
}

void PreviewBlockGrid::InitGrid() {
	for (int i = 0; i < PREVIEW_HEIGHT; i++) {
		for (int j = 0; j < PREVIEW_WIDTH; j++) {
			if (i == 0 || j == 0 || j == PREVIEW_WIDTH - 1) {
				grid[i][j] = GridBlock(BlockType::Border);
				continue;
			}

			grid[i][j] = GridBlock(BlockType::None);
		}
	}
}