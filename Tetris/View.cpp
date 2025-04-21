#include "View.h"
#include <iostream>

namespace View {
	Edge GetEdge(int WWidth, int WHeight) {
		int CenterWWidth = WWidth / 2;
		int InitCoorX = CenterWWidth - (GRID_COLS / 2);

		int CenterWHeight = WHeight / 2;
		int InitCoorY = CenterWHeight - (GRID_ROWS / 2);

		return { InitCoorX, InitCoorY };
	}

	void RenderGrid(Grid& grid) {
		Coord refPoint = grid.GetGridRefPoint();

		for (int i = 0; i < GRID_ROWS; i++) {
			for (int j = 0; j < GRID_COLS; j++) {
				const GridBlock& block = grid.GetGridData(i, j);

				GoTo(refPoint.x + j, refPoint.y + i);

				switch (block.GetType()) {
				case BlockType::Border:
					std::cout << "X";
					break;

				case BlockType::TetrominoeBlock:
					std::cout << block.m_Color << "O" << "\033[0m";
					break;

				case BlockType::None:
					std::cout << " ";
					break;

				case BlockType::PreviewBlock:
					std::cout << block.m_Color;
					printf("%c", 219);
					std::cout << "\033[0m";
					break;
				}
			}
		}
	}

	void RenderScoreAndSpeed(int score, double SPEED, ConsoleSize cSize) {
		int x = cSize.width * 0.75;
		int y = cSize.height * 0.3;
		double xSpeed = ((SPEED - 1) / -1) + 1;

		GoTo(x, y);
		std::cout << "SCORE: " << score;

		GoTo(x, y + 2);
		std::cout << "SPEED: x" << xSpeed;
	}

	void RenderFinal(int score, ConsoleSize cSize) {
		ClearScreen();
		int x = (cSize.width / 2) - 7;
		int y = (cSize.height / 2);
		GoTo(x, y);

		std::cout << "FINAL SCORE: " << score;
	}

	void RenderInit(ConsoleSize cSize) {
		GoTo(cSize.width / 2 - 10, cSize.height / 2);
		std::cout << "Press any letter to start";
		std::cin.get();
		ClearScreen();
	}

	void RenderNextBlock(PreviewBlockGrid grid, ConsoleSize cSize) {
		int x = cSize.width * 0.75;
		int y = cSize.height * 0.5;

		GoTo(x, y - 2);
		std::cout << "NEXT BLOCK:";

		for (int i = 0; i < PREVIEW_HEIGHT; i++) {
			for (int j = 0; j < PREVIEW_WIDTH; j++) {
				GoTo(x + j, y + i);
				const auto& cell = grid.GetCell(i, j);

				if (cell.GetType() == BlockType::TetrominoeBlock) {
					std::cout << cell.m_Color << "O" << "\033[0m";
				}
				else {
					std::cout << " ";
				}
			}
		}
	}

	void RenderOutOfGridBlock(PreviewBlockGrid grid) {
		int addX = (GRID_COLS / 2) - (PREVIEW_WIDTH / 2);
		for (int i = 0; i < PREVIEW_HEIGHT; i++) {
			for (int j = 0; j < PREVIEW_WIDTH; j++) {
				GoTo(grid.refPoint.x + addX + j, grid.refPoint.y + i);
				const auto& cell = grid.GetCell(i, j);

				switch (cell.GetType()) {
				case BlockType::Border:
					std::cout << "X";
					break;

				case BlockType::TetrominoeBlock:
					std::cout << cell.m_Color << "O" << "\033[0m";
					break;

				case BlockType::PreviewBlock:
					break;

				case BlockType::None:
					std::cout << " ";
					break;
				}
			}
		}
	}
}