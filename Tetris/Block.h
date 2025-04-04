#pragma once

#include <string>
#include <cstdint>
#include <array>
#include <vector>

struct Coord {
	int x, y;
};

struct Shape {
	Coord axis;
	Coord right;
	Coord left;
	Coord particular;
};

enum class BlockType {
	None, Border, TetrominoeBlock
};

class GridBlock {
private:
	BlockType m_Type;

public:
	static const std::array<const char*, 20> blockColors;
	std::string m_Color;
	int cellId;

public:
	GridBlock(BlockType m_Type) : m_Type(m_Type), m_Color("\033[0m") {};
	GridBlock(BlockType m_Type, std::string color, int id);
	GridBlock() : m_Type(BlockType::None), m_Color("\033[0m") {};

	BlockType GetType() const;
	void SetType(BlockType type);
};

class Tetrominoe : public GridBlock {
private:
	Coord m_AxisLocation;
	std::vector<Coord> m_Pos;

	static constexpr std::array<Shape, 7> tetrominoes = {
		Shape{ Coord{0, 0}, Coord{1, 0}, Coord{-1, 0}, Coord{2, 0} },  // I
		Shape{ Coord{0, 0}, Coord{1, 0}, Coord{0, -1}, Coord{1, -1} }, // O
		Shape{ Coord{0, 0}, Coord{0, -1}, Coord{0, 1}, Coord{1, -1} }, // L
		Shape{ Coord{0, 0}, Coord{1, 0}, Coord{-1, 0}, Coord{-1, 1} }, // J
		Shape{ Coord{0, 0}, Coord{1, 0}, Coord{-1, 0}, Coord{0, 1} },  // T
		Shape{ Coord{0, 0}, Coord{1, 0}, Coord{0, -1}, Coord{1, 1} }, // S
		Shape{ Coord{0, 0}, Coord{1, 0}, Coord{0, 1}, Coord{1, -1} }  // Z
	};

public:
	Tetrominoe(std::string color, std::vector<Coord> m_Pos, Coord axisLocation);
	Tetrominoe();

public:
	static int idCounter;
	int blockId;

	static Tetrominoe CreateRandom(const Coord& m_AxisLocation);
	Coord GetAxisLocation() const;
	void SetAxisLocation(const Coord& newLoc);
	std::vector<Coord> GetShape() const;
	void SetShape(const std::vector<Coord>& newShape);
	std::vector<Coord> RotationCoords();
};

