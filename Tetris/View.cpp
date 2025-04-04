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

	void RenderScoreAndSpeed(int score, double SPEED) {
		ConsoleSize cSize = GetConsoleSize();
		int x = cSize.width * 0.75;
		int y = cSize.height / 2;
		double xSpeed = ((SPEED - 1)/-1) + 1;
		GoTo(x, y - 2);
		std::cout << "SCORE: " << score;
		GoTo(x, y);
		std::cout << "SPEED: x" << xSpeed;
	}

	void RenderFinal(int score) {
		std::system("cls");
		ConsoleSize cSize = GetConsoleSize();
		int x = (cSize.width / 2) - 5;
		int y = (cSize.height / 2) - 2;
		GoTo(x, y);

		std::cout << "FINAL SCORE: " << score << std::endl;
	}
}