#include "Block.h"
#include <random>

// Grid Block
GridBlock::GridBlock(BlockType m_Type, std::string color) 
	: m_Type(m_Type), m_Color(color) {}

BlockType GridBlock::GetType() const {
	return m_Type;
}

const std::array<const char*, 7> GridBlock::blockColors = {
	"\033[36m",  // cyan
	"\033[33m",  // yellow
	"\033[35m",  // purple
	"\033[38;5;214m", // orange
	"\033[34m",  // blue
	"\033[32m",  // green
	"\033[31m"   // red
};

// Tetrominoe
Tetrominoe::Tetrominoe(std::string color, Shape pos, Coord axisLocation)
	: GridBlock(BlockType::TetrominoeBlock, color), m_Pos(pos), m_AxisLocation(axisLocation) {}

Tetrominoe::Tetrominoe() {}

Tetrominoe Tetrominoe::CreateRandom(const Coord& m_AxisLocation) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> colorDist(0, 6);
	std::uniform_int_distribution<int> formDist(0, 6);

	std::string randomColor = blockColors[colorDist(gen)];
	Shape randomForm = tetrominoes[formDist(gen)];

	return Tetrominoe(randomColor, randomForm, m_AxisLocation);
}

Coord Tetrominoe::GetAxisLocation() const {
	return m_AxisLocation;
}

void Tetrominoe::SetAxisLocation(const Coord& newLoc) {
	m_AxisLocation = newLoc;
}

std::array<Coord, 4> Tetrominoe::GetShape() const {
	std::array<Coord, 4> arr = {
		m_Pos.axis, m_Pos.right, m_Pos.left, m_Pos.particular
	};

	return arr;
}

std::array <Coord, 4> Tetrominoe::RotationCoords() {
	int temp_x, temp_y;

	temp_x = m_Pos.axis.x;
	temp_y = m_Pos.axis.y;
	Coord axis = { -temp_y, temp_x };

	temp_x = m_Pos.right.x;
	temp_y = m_Pos.right.y;
	Coord right = { -temp_y, temp_x };

	temp_x = m_Pos.left.x;
	temp_y = m_Pos.left.y;
	Coord left = { -temp_y, temp_x };

	temp_x = m_Pos.particular.x;
	temp_y = m_Pos.particular.y;
	Coord particular = { -temp_y, temp_x };

	return { axis, right, left, particular };
}

void Tetrominoe::SetShape(const std::array<Coord, 4>& newShape) {
	m_Pos.axis = newShape[0];
	m_Pos.right = newShape[1];
	m_Pos.left = newShape[2];
	m_Pos.particular = newShape[3];
}

