#include "Block.h"
#include <random>

// Grid Block
GridBlock::GridBlock(BlockType m_Type, std::string color, int id) 
	: m_Type(m_Type), m_Color(color), cellId(id) {}

BlockType GridBlock::GetType() const {
	return m_Type;
}

void GridBlock::SetType(BlockType type) {
	m_Type = type;
}

const std::array<const char*, 20> GridBlock::blockColors = {
	"\033[31m",       // red
	"\033[32m",       // green
	"\033[33m",       // yellow
	"\033[34m",       // blue
	"\033[35m",       // purple
	"\033[36m",       // cyan
	"\033[38;5;208m", // deep orange
	"\033[38;5;202m", // bright orange
	"\033[38;5;196m", // intense red
	"\033[38;5;46m",  // neon green
	"\033[38;5;82m",  // light green
	"\033[38;5;226m", // bright yellow
	"\033[38;5;21m",  // deep blue
	"\033[38;5;99m",  // pink-magenta
	"\033[38;5;93m",  // soft pink
	"\033[38;5;27m",  // electric blue
	"\033[38;5;201m", // neon pink
	"\033[38;5;190m", // golden yellow
	"\033[38;5;51m",  // bright cyan
	"\033[38;5;130m"  // brown-orange
};

// Tetrominoe
int Tetrominoe::idCounter = 0;
int Tetrominoe::randomCounter = 0;
std::array<Tetrominoe, RANDOM_BLOCKS_COUNT> Tetrominoe::randomBlocks;

Tetrominoe::Tetrominoe(std::string color, std::vector<Coord> pos, Coord axisLocation)
	: GridBlock(BlockType::TetrominoeBlock, color, idCounter), m_Pos(pos), m_AxisLocation(axisLocation), blockId(idCounter++) {}

Tetrominoe::Tetrominoe() {}

void Tetrominoe::GenerateRandomSequence() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> colorDist(0, 19);
	std::uniform_int_distribution<int> formDist(0, 6);

	for (int i = 0; i < RANDOM_BLOCKS_COUNT; i++) {
		std::string randomColor = blockColors[colorDist(gen)];
		Shape randomForm = tetrominoes[formDist(gen)];

		std::vector<Coord> positions = { randomForm.axis, randomForm.right, randomForm.left, randomForm.particular };
		randomBlocks[i] = Tetrominoe(randomColor, positions, {0, 0});
	}
}

Tetrominoe Tetrominoe::CreateRandom(const Coord& m_AxisLocation) {
	Tetrominoe block = randomBlocks[randomCounter];
	block.m_AxisLocation = m_AxisLocation;

	randomCounter++;
	if (randomCounter == RANDOM_BLOCKS_COUNT) randomCounter = 0;

	return block;
}

Coord Tetrominoe::GetAxisLocation() const {
	return m_AxisLocation;
}

void Tetrominoe::SetAxisLocation(const Coord& newLoc) {
	m_AxisLocation = newLoc;
}

std::vector<Coord> Tetrominoe::GetShape() const {
	return m_Pos;
}

std::vector<Coord> Tetrominoe::RotationCoords() {
	int temp_x, temp_y;

	std::vector<Coord> rotated;
	for (const auto& coord : m_Pos) {
		rotated.push_back({ -coord.y, coord.x });
	}
	return rotated;
}

void Tetrominoe::SetShape(const std::vector<Coord>& newShape) {
	m_Pos = newShape;
}

