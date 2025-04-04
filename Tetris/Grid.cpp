#include <iostream>
#include "Grid.h"

// Some context:
// We use (y, x) in a lot of functions
// This is cause y refers to rows (the x axis) and x refers to cols (the y axis)
// Example:
//   x1 x2 x3
// [ [] [] [] ] y1
// [ [] [] [] ] y2
// [ [] [] [G] ] y3

// To get the G: [y3, x3]
// Why is this? Cause of the loop
// We first iterate the rows, in this case de Ys (You could understand this aswell as height (y) and width (x))
// Then we iterate the columnts, in this case de Xs

Grid::Grid(Coord refPoint) : refPoint(refPoint) {
	InitMap();
}

const GridBlock& Grid::GetGridData(int col, int row) {
	return m_GridData[col][row];
}

const Coord& Grid::GetGridRefPoint() {
	return refPoint;
}

const AnalysisResult Grid::Analyze() {
	std::vector<int> rows;

	// End case
	int x = GRID_COLS / 2; 
	int y = 2;

	if (GetGridData(y, x).GetType() != BlockType::None) 
		return { GridState::End, {} };
	
	// Other cases
	for (int i = GRID_ROWS - 1; i > -1; --i) {
		if (i == 0 || i == GRID_ROWS - 1) continue;

		bool isComplete = true;

		for (int j = 0; j < GRID_COLS; j++) {
			if (j == 0 || j == GRID_COLS - 1) continue;

			if (GetGridData(i, j).GetType() != BlockType::TetrominoeBlock) {
				isComplete = false;
				break;
			}
		}

		if (isComplete) rows.emplace_back(i);
	}

	if (rows.size() > 0) return { GridState::RowCompleted, rows };

	return { GridState::Continue, rows };
};

void Grid::RowCleaning(const std::vector<int>& rows) {
	for (int row : rows) {
		for (int i = GRID_COLS - 1; i > -1; --i) {
			if (i == 0 || i == GRID_COLS - 1) continue;

			SetPos(row, i, BlockType::None, "", -1);
		}
	}
};

void Grid::UpdatePlacedBlocks(const std::vector<int>& rows) {
	std::vector<Tetrominoe> newPlacedBlocks;

	for (const Tetrominoe& block : m_PlacedBlocks) {
		std::vector<Coord> newCoordsUp;
		std::vector<Coord> newCoordsDown;

		for (Coord cd : block.GetShape()) {
			int y = cd.y + block.GetAxisLocation().y;

			if (y < rows.back()) newCoordsUp.emplace_back(cd);
			else if (y > rows.front()) newCoordsDown.emplace_back(cd);
		}
					
		if (newCoordsDown.size() > 0)
			newPlacedBlocks.emplace_back(Tetrominoe(block.m_Color, newCoordsDown, block.GetAxisLocation()));
		if (newCoordsUp.size() > 0) 
			newPlacedBlocks.emplace_back(Tetrominoe(block.m_Color, newCoordsUp, block.GetAxisLocation()));
	}

	m_PlacedBlocks = newPlacedBlocks;
}

void Grid::UpdateGrid() {
	InitMap();
	for (const auto& block : m_PlacedBlocks) {
		for (const auto& coord : block.GetShape()) {
			int x = block.GetAxisLocation().x + coord.x;
			int y = block.GetAxisLocation().y + coord.y;

			SetPos(y, x, BlockType::TetrominoeBlock, block.m_Color, block.blockId);
		}
	}
}

bool Grid::AbleToSet(int col, int row) {
	if (GetGridData(col, row).GetType() == BlockType::None) return true;

	if (GetGridData(col, row).GetType() == BlockType::Border) return false;

	if (GetGridData(col, row).cellId != m_MovingBlock.blockId) return false;

	return true;
}

void Grid::SetPos(int col, int row, const BlockType& type, const std::string& color, int blockId) {
	m_GridData[col][row] = GridBlock(type, color, blockId);
}

void Grid::InitMap() {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; j++) {
			if (i != 0 && j != 0 && i != GRID_ROWS - 1 && j != GRID_COLS - 1)
				Grid::SetPos(i, j, BlockType::None, "", -1);
			else 
				Grid::SetPos(i, j, BlockType::Border, "", -1);
		}
	}
}

void Grid::CleanAll() {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; j++) {
			Grid::SetPos(i, j, BlockType::None, "", -1);
		}
	}
}

const BlockStatus& Grid::SetTetrominoePos(int row, int col) {
	auto coords = m_MovingBlock.GetShape();

	bool isAbleToSet = true;
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x + row;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y + col;

		// { Row, Col }
		isAbleToSet = Grid::AbleToSet(y, x);
		if (isAbleToSet == false) break;
	}

	if (isAbleToSet == false) {
		return { true, m_MovingBlock };
	}

	// Clean the tetrominoe 
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::None, "", -1);
	}

	// Then we update its position in the grid
	Coord newPos = { m_MovingBlock.GetAxisLocation().x + row, m_MovingBlock.GetAxisLocation().y + col };
	m_MovingBlock.SetAxisLocation(newPos);

	// We set the grid again
	for (const Coord& coord : coords) {
		int	x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::TetrominoeBlock, m_MovingBlock.m_Color, m_MovingBlock.blockId);
	}

	BlockStatus res = { false, m_MovingBlock };
	return res;
}

void Grid::RotateBlock() {
	auto rotationCoords = m_MovingBlock.RotationCoords();
	bool isAbleToSet = true;
	for (const Coord& coord : rotationCoords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		isAbleToSet = Grid::AbleToSet(y, x);
		if (isAbleToSet == false) break;
	}

	if (!isAbleToSet) return;

	// Clean the tetrominoe 
	auto coords = m_MovingBlock.GetShape();
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::None, "", -1);
	}

	m_MovingBlock.SetShape(rotationCoords);

	coords = m_MovingBlock.GetShape();
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::TetrominoeBlock, m_MovingBlock.m_Color, m_MovingBlock.blockId);
	}
}

const Tetrominoe& Grid::CreateBlock() {
	// Get center
	int xRelativeAxisCenter = GRID_COLS / 2; 
	// The tetrominoe appears in the second row
	int yAxis = 2;

	// Create the random block
	Coord relativeCoord = { xRelativeAxisCenter, yAxis };
	Tetrominoe tetro = Tetrominoe::CreateRandom(relativeCoord);
	m_MovingBlock = tetro;

	// Put the random block in the grid
	auto coords = m_MovingBlock.GetShape();
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Col, Row, Type }
		Grid::SetPos(y, x, BlockType::TetrominoeBlock, m_MovingBlock.m_Color, m_MovingBlock.blockId);
	}

	return m_MovingBlock;
}

BlockStatus Grid::NaturalMovement(const Tetrominoe& block) {
	m_MovingBlock = block;

	BlockStatus isSettedPos = SetTetrominoePos(0, 1);

	BlockStatus res = { true, m_MovingBlock};
	if (isSettedPos.located) return res;
	
	return { false, m_MovingBlock };
}

void Grid::UserMovement() {
	BlockStatus isSettedPos;

	if (GetKey() == -32) {
		switch (GetKey()) {
		case 72: // Up
			RotateBlock();
			break;
		case 80: // Down
			while (true) {
				isSettedPos = SetTetrominoePos(0, 1);
				if (isSettedPos.located == true) break;
			}
			break;
		case 77: // Right
			SetTetrominoePos(1, 0);
			break;
		case 75: // Left
			SetTetrominoePos(-1, 0);
			break;
		}
	}
}

void Grid::StartInputThread() {
	inputThread = std::thread([this]() {
		while (running) {
			std::lock_guard<std::mutex> lock(mtx); 
			UserMovement();  
			std::this_thread::sleep_for(std::chrono::milliseconds(SPEED));
		}
	});
}

void Grid::StopInputThread() {
	running = false;
	if (inputThread.joinable()) inputThread.join(); 
}
