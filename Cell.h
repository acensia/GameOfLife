#pragma once

#include <vector>

class Cell
{
public:
	enum state_type { // state of a cell //delete static
		ALIVE,	// this cell is breathing, represented in black square on the screen
		DEAD	// goodbye
	};

	Cell();
	Cell(int x_pos, int y_pos);
	~Cell();

	// cell's position in the world space. 
	// the most bottom left cell has (0,0),
	// the most bottom right cell has (49, 0),
	// the most top left cell has (0, 49),
	// and the most top right cell has (49, 49)
	int _x_pos;
	int _y_pos;

	inline state_type GetState() { return _nxtState; } // getter for the next state
	void SetNeighbors(Cell* cell); // indicate a neighbor cell. you should call this 8 times to pushback 8  touching cells to the private variable _neighbors
	std::vector<Cell*> GetNeighbors(); // getter for the neighbors
	bool CurLive();

	int CountNeighbors();	// count alive neighbors. this will probably be used in GameManager::Update() func

	inline void Kill() { _nxtState = DEAD; }			// goodbye. this cell will set to die at next when its Cell::Update() is called
	inline void Awake() { _nxtState = ALIVE; }		// awake this cell at next time when its Cell::Update() is called
	inline void Update() { _curState = _nxtState; }	// change current state to the next reserved state

private:
	state_type _curState = DEAD;	// current state of this cell
	state_type _nxtState = DEAD;	// reserved state of this cell
	std::vector<Cell*> _neighbors;	// neighbors of this cell
};
