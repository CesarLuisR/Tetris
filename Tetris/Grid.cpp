#include <iostream>
#include "Grid.h"
#include "ConsoleUtils.h"

Grid::Grid(Coord refPoint) : refPoint(refPoint) {
	CreateBorders();
}

const GridBlock& Grid::GetGridData(int col, int row) {
	return m_GridData[col][row];
}

const Coord& Grid::GetGridRefPoint() {
	return refPoint;
}

const AnalysisResult& Grid::Analyze() {
	std::vector<int> rows;
	
	for (int i = 0; i < GRID_ROWS; i++) {
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

bool Grid::AbleToSet(int col, int row) {
	if (GetGridData(col, row).GetType() == BlockType::None) return true;

	if (GetGridData(col, row).GetType() == BlockType::Border) return false;

	for (int i = m_PlacedCoords.size() - 1; i >= 0; i--) 
		if (m_PlacedCoords[i].x == row && m_PlacedCoords[i].y == col) 
			return false;

	return true;
}

void Grid::SetPos(int col, int row, const BlockType& type, const std::string& color) {
	m_GridData[col][row] = GridBlock(type, color);
}

void Grid::CreateBorders() {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; j++) {
			if (i != 0 && j != 0 && i != GRID_ROWS - 1 && j != GRID_COLS - 1)
				Grid::SetPos(i, j, BlockType::None, "");
			else 
				Grid::SetPos(i, j, BlockType::Border, "");
		}
	}
}

void Grid::CleanAll() {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; j++) {
			Grid::SetPos(i, j, BlockType::None, "");
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
		std::array<Coord, 4> returningCoords;
		int counter = 0;

		for (const Coord& coord : coords) {
			int	x = m_MovingBlock.GetAxisLocation().x + coord.x;
			int y = m_MovingBlock.GetAxisLocation().y + coord.y;

			returningCoords[counter++] = Coord{ x, y };
		}

		return { true, returningCoords };
	};

	// Clean the tetrominoe 
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::None, "");
	}

	// Then we update its position in the grid
	Coord newPos = { m_MovingBlock.GetAxisLocation().x + row, m_MovingBlock.GetAxisLocation().y + col };
	m_MovingBlock.SetAxisLocation(newPos);

	// We set the grid again
	for (const Coord& coord : coords) {
		int	x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::TetrominoeBlock, m_MovingBlock.m_Color);
	}

	BlockStatus res = { false, {0, 0} };
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
		Grid::SetPos(y, x, BlockType::None, "");
	}

	m_MovingBlock.SetShape(rotationCoords);

	coords = m_MovingBlock.GetShape();
	for (const Coord& coord : coords) {
		int x = m_MovingBlock.GetAxisLocation().x + coord.x;
		int y = m_MovingBlock.GetAxisLocation().y + coord.y;

		// { Row, Col, Type }
		Grid::SetPos(y, x, BlockType::TetrominoeBlock, m_MovingBlock.m_Color);
	}
}

const Tetrominoe& Grid::CreateBlock() {
	// Get center
	int xRelativeAxisCenter = GRID_COLS / 2; 
	// The tetrominoe appears in the second row
	int yAxis = 2;
	//int xAbsoluteAxisCenter = refPoint.x + xRelativeAxisCenter;

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
		Grid::SetPos(y, x, BlockType::TetrominoeBlock, m_MovingBlock.m_Color);
	}

	return m_MovingBlock;
}

const BlockStatus& Grid::NaturalMovement() {
	BlockStatus isSettedPos = SetTetrominoePos(0, 1);

	BlockStatus res = { true, isSettedPos.coords };
	if (isSettedPos.located) return res;
	
	return { false, {0, 0} };
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
			std::lock_guard<std::mutex> lock(mtx);  // Protege acceso
			UserMovement();  // Llama al método en el hilo separado
			std::this_thread::sleep_for(std::chrono::milliseconds(SPEED));
		}
	});
}

void Grid::StopInputThread() {
	running = false;
	if (inputThread.joinable()) inputThread.join();  // Espera a que termine
}
