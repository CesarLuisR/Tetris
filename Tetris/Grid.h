#pragma once
#include "Block.h"
#include "ConsoleUtils.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "CONSTANTS.h"

#include <array>
#include <vector>

struct BlockStatus {
	bool located;
	std::array<Coord, 4> coords{};
};

enum class GridState {
	End, Continue, RowCompleted
};

struct AnalysisResult {
	GridState state;
	std::vector<int> rows;
};

class Grid {
private:
	Coord refPoint;
	GridBlock m_GridData[GRID_ROWS][GRID_COLS];
	Tetrominoe m_MovingBlock;

public:
	Grid(Coord refPoint);

private:
	std::thread inputThread;
	std::atomic<bool> running{ true };  
	std::mutex mtx;

public:
	void StartInputThread();
	void StopInputThread();

public:
	std::vector<Coord> m_PlacedCoords;

	const AnalysisResult& Analyze();
	const GridBlock& GetGridData(int i, int j);
	const Coord& GetGridRefPoint();
	const Tetrominoe& CreateBlock();
	const BlockStatus& NaturalMovement();

	void UserMovement();
	void RotateBlock();
	void CleanAll();

private:
	bool AbleToSet(int col, int row);
	void SetPos(int x, int y, const BlockType& type, const std::string& color);
	const BlockStatus& SetTetrominoePos(int x, int y);
	void CreateBorders();
};
