#include "View.h"
#include <iostream>

namespace View {
	Edge GetEdge(int WWidth, int WHeight) {
		int CenterWWidth = WWidth / 2;
		int InitCoorX = CenterWWidth - (GRID_COLS/2);

		int CenterWHeight = WHeight / 2;
		int InitCoorY = CenterWHeight - (GRID_ROWS/2);

		return { InitCoorX, InitCoorY };
	}

	void RenderGrid(Grid& grid) {
		Coord refPoint = grid.GetGridRefPoint();

		for (int i = 0; i < GRID_ROWS; i++) {
			GoTo(refPoint.x, refPoint.y++);

			for (int j = 0; j < GRID_COLS; j++) {
				const GridBlock& block = grid.GetGridData(i, j);

				switch (block.GetType()) {
				case BlockType::Border:
					//printf("%c", 219);
					std::cout << "X";
					break;

				case BlockType::TetrominoeBlock:
					std::cout << block.m_Color;
					//printf("%c", 219);
					std::cout << "O";
					std::cout << "\033[0m";
					break;
			
				case BlockType::None:
					std::cout << " ";
					break;
				}
			}

			std::cout << std::endl;
		}
	}

	void RenderTime(std::chrono::steady_clock::time_point& time) {
		//std::cout << time << "\n";
	}
}