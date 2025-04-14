#include <iostream>
#include "View.h"
#include "Grid.h"
#include "Block.h"
#include "PreviewBlockGrid.h"
#include "ConsoleUtils.h"
#include <chrono>
#include <thread>
#include <cmath>
#include "CONSTANTS.h"

int main() {
	ConsoleSize cSize = GetConsoleSize();

	View::RenderInit(cSize);
	Tetrominoe::GenerateRandomSequence();

	// Create grid and score
	Edge edge = View::GetEdge(cSize.width, cSize.height);

	Coord refPoint = { edge.InitX, edge.InitY };
	Grid grid(refPoint);
	PreviewBlockGrid previewGrid(refPoint);
	PreviewBlockGrid nextBlock(refPoint);

	// Hide the cursor
	std::cout << "\033[?25l"; 

	// Create the thread for concurrent movement
	grid.StartInputThread();

	unsigned long long int score = 0;

	while (true) {
		// Speed manager
		const unsigned int max = 500;  
		double speedManager = 0.5;
		if (score <= 500)
			speedManager = 0.5 + (0.5 * (max - score) / max);
		SPEED = std::round(speedManager * INIT_SPEED);

		// render score
		View::RenderScoreAndSpeed(score, speedManager, cSize);

		// grid analysis
		const AnalysisResult result = grid.Analyze();

		if (result.state == GridState::End) {
			View::RenderFinal(score, cSize);
			grid.StopInputThread();
			std::cin.get();
			return 0;
		}

		if (result.state == GridState::RowCompleted) {
			// score manager
			score += pow(4, result.rows.size());

			// Deleting row and updating blocks structure
			grid.RowCleaning(result.rows);
			grid.UpdatePlacedBlocks(result.rows);
			grid.UpdateGrid();

			// Choose from which row start gravity
			// Trying to get the lowest row possible to avoid floating blocks
			int init = result.rows[0] + 1 > GRID_ROWS - 1
				? result.rows[0] + 2 >  GRID_ROWS - 1
					? result.rows[0] + 2
					: result.rows[0] + 1
				: result.rows[0];

			// Apply gravity by order of blocks
			// Takes the lowest rows and look for the blocks down there
			for (int i = init; i > -1; i--) {
				if (i == 0 || i == GRID_ROWS - 1) continue;

				for (int j = 0; j < GRID_COLS; j++) {
					if (j == 0 || j == GRID_COLS - 1) continue;

					int id = grid.GetGridData(i, j).cellId;

					for (auto& block : grid.m_PlacedBlocks) {
						if (block.blockId != id) continue;

						while (true) {
							std::this_thread::sleep_for(std::chrono::milliseconds(SPEED/4));
							BlockStatus state = grid.NaturalMovement(block);
							block = state.block;
							View::RenderGrid(grid);

							if (state.located) break;
						}
						break;
					}
				}
			}

			continue;
		}

		if (result.state == GridState::Continue) {
			// Render empty grid
			View::RenderGrid(grid);

			// create tetrominoe and set the preview
			const Tetrominoe& block = grid.CreateBlock();
			previewGrid.InitGrid();
			previewGrid.SetBlock(block);

			// Set the next block grid
			nextBlock.SetBlock(block.randomBlocks[block.randomCounter]);

			// Render the preview and next block grid
			View::RenderOutOfGridBlock(previewGrid);
			View::RenderNextBlock(nextBlock, cSize);
			std::this_thread::sleep_for(std::chrono::milliseconds(SPEED * 3));

			// Clean the preview grid
			previewGrid.InitGrid();
			View::RenderOutOfGridBlock(previewGrid);

			while (true) {
				std::this_thread::sleep_for(std::chrono::milliseconds(SPEED));
				grid.PreviewFinalPos();
				BlockStatus status = grid.NaturalMovement(block);

				View::RenderGrid(grid);
				if (status.located) {
					grid.m_PlacedBlocks.emplace_back(status.block);
					break;
				}
			}
		}
	}

	// Stop the thread
	grid.StopInputThread();

	// Show cursor
	std::cout << "\033[?25h";
}
