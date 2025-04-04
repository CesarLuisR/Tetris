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
	Tetrominoe block;
};

enum class GridState {
	End, Continue, RowCompleted
};

struct AnalysisResult {
	GridState state;
	std::vector<int> rows;
};

struct PlacedBlock {
	BlockType type;
	Coord coords;
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
	std::vector<Tetrominoe> m_PlacedBlocks;

	const AnalysisResult Analyze();
	const GridBlock& GetGridData(int i, int j);
	const Coord& GetGridRefPoint();
	const Tetrominoe& CreateBlock();
	BlockStatus NaturalMovement(const Tetrominoe& block);

	void RowCleaning(const std::vector<int>& rows);
	void UpdatePlacedBlocks(const std::vector<int>& rows);
	void UserMovement();
	void RotateBlock();
	void CleanAll();

public:
	bool AbleToSet(int col, int row);
	void SetPos(int x, int y, const BlockType& type, const std::string& color, int id);
	const BlockStatus& SetTetrominoePos(int x, int y);
	void InitMap();
	void UpdateGrid();
};
