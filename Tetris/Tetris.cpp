#include <iostream>
#include "View.h"
#include "Grid.h"
#include "ConsoleUtils.h"
#include <chrono>
#include <thread>
#include "CONSTANTS.h"

int main() {
	// Create grid and score
	ConsoleSize cSize = GetConsoleSize();
	Edge edge = View::GetEdge(cSize.width, cSize.height);

	Coord refPoint = { edge.InitX, edge.InitY };
	Grid grid(refPoint);

	int score = 0;

	// Start time
	auto timer = std::chrono::high_resolution_clock::now();

	// Hide the cursor
	std::cout << "\033[?25l"; 

	// Create the thread for concurrent movement
	grid.StartInputThread();

	while (true) {
		const AnalysisResult& result = grid.Analyze();

		if (result.state == GridState::End) {
			continue;
		}

		if (result.state == GridState::RowCompleted) {
			std::cout << "GRID COMPLETED" << std::endl;
			continue;
		}

		if (result.state == GridState::Continue) {
			const Tetrominoe& block = grid.CreateBlock();

			while (true) {
				std::this_thread::sleep_for(std::chrono::milliseconds(SPEED)); 
				BlockStatus status = grid.NaturalMovement();

				View::RenderGrid(grid);
				if (status.located) {
					for (Coord coord : status.coords) 
						grid.m_PlacedCoords.emplace_back(coord);
					break;
				}
			}

			continue;
		}
	}

	// Stop the thread
	grid.StopInputThread();

	// Show cursor
	std::cout << "\033[?25h";

	//while (true) {
	//	GridState state = Grid.Analyze();

	//	if (state == GridState::End) {
	//		FinalScreen();
	//		break;
	//	}
	//	else if (state == GridState::RowElimination) {
	//		Grid.DeleteRow(x);
	//	}
	//	else if (state == GridState::Normal) {
	//		Block block();
	//		Grid.CreateMovingBlock(block);

	//		std::thread worker(block.NaturalMovement);
	//		block.UserMovement();
	//	}

	//	Render();
	//}

}
