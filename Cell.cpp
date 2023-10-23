#include "Cell.h"

Cell::Cell()
{
	// default constructor
	_x_pos = -1;
	_y_pos = -1;

	_curState = DEAD;
	_nxtState = DEAD;
}

Cell::Cell(int x_pos, int y_pos)
{	
	// constructor with initializing cell's position
	_x_pos = x_pos;
	_y_pos = y_pos;
}

Cell::~Cell()
{	
}

void Cell::SetNeighbors(Cell * cell)
{
	// indicate a neighbor cell. you should call this 8 times to pushback 8  touching cells to the private variable _neighbors
	_neighbors.push_back(cell);
}

std::vector<Cell*> Cell::GetNeighbors()
{
	return _neighbors;
}


int Cell::CountNeighbors()
{	
	// count alive neighbors. this will probably be used in GameManager::Update() func
	int count = 0;

	// count alive neighbors >>>>>
	/*
	TODO : 
	count alive neighbors
	*/
	for (int i = 0; i < _neighbors.size(); i++) {
		if (_neighbors.at(i)->_curState == ALIVE) count++;
	}

	// <<<<< count alive neighbors
	return count;
}
///////////////////////////////////
bool Cell::CurLive() {
	return (_curState == ALIVE);
}