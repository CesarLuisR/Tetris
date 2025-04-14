#include "View.h"
#include <iostream>

namespace View {
	Edge GetEdge(int WWidth, int WHeight) {
		int CenterWWidth = WWidth / 2;
		int InitCoorX = CenterWWidth - (GRID_COLS / 2);

		int CenterWHeight = WHeight / 2;
		int InitCoorY = CenterWHeight - (GRID_ROWS / 2);

		return { InitCoorX, InitCoorY + 15 };
	}

	void RenderGrid(Grid& grid) {
		Coord refPoint = grid.GetGridRefPoint();

		for (int i = 0; i < GRID_ROWS; i++) {
			GoTo(refPoint.x, refPoint.y++);

			for (int j = 0; j < GRID_COLS; j++) {
				const GridBlock& block = grid.GetGridData(i, j);

				switch (block.GetType()) {
				case BlockType::Border:
					std::cout << "X";
					break;

				case BlockType::TetrominoeBlock:
					std::cout << block.m_Color;
					std::cout << "O";
					std::cout << "\033[0m";
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

			std::cout << std::endl;
		}
	}

	void RenderScoreAndSpeed(int score, double SPEED, ConsoleSize cSize) {
		int x = cSize.width * 0.75;
		int y = cSize.height / 1.1;
		double xSpeed = ((SPEED - 1) / -1) + 1;
		GoTo(x, y - 2);
		std::cout << "SCORE: " << score;
		GoTo(x, y);
		std::cout << "SPEED: x" << xSpeed;
	}

	void RenderFinal(int score, ConsoleSize cSize) {
		std::system("cls");
		int x = (cSize.width / 2) - 5;
		int y = (cSize.height / 2) - 2;
		GoTo(x, y);

		std::cout << "FINAL SCORE: " << score << std::endl;
	}

	void RenderInit(ConsoleSize cSize) {
		GoTo(cSize.width / 2 - 10, cSize.height / 2 - 2);
		std::cout << "Press any letter to start" << std::endl;
		std::cin.get();
		std::system("cls");
	}

	void RenderNextBlock(PreviewBlockGrid grid, ConsoleSize cSize) {
		int x = cSize.width * 0.75;
		int y = cSize.height * 1;

		GoTo(x, y - 1);
		std::cout << "NEXT BLOCK: ";

		for (int i = 0; i < PREVIEW_HEIGHT; i++) {
			for (int j = 0; j < PREVIEW_WIDTH; j++) {
				GoTo(x + j, y + i);
				const auto& cell = grid.GetCell(i, j);

				if (cell.GetType() == BlockType::TetrominoeBlock) {
					std::cout << cell.m_Color;
					std::cout << "O";
					std::cout << "\033[0m";
				}
				else {
					std::cout << " ";
				}

				std::cout << std::endl;
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
					std::cout << cell.m_Color;
					std::cout << "O";
					std::cout << "\033[0m";
					break;

				case BlockType::None:
					std::cout << " ";
					break;
				}

				std::cout << std::endl;
			}
		}
	}
}