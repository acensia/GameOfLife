#pragma once

#include "GL/freeglut.h"
#include "GL/glut.h"

#include "Cell.h"

#include <fstream>
#include <iostream>
#include <vector>


class GameManager
{
	/*
	GameManager class manages state of this app, main logic of GameOfLife, and drawing on the screen.
	You should initialize the world as a 2D vector of Cell at Init() func, 
	and also should implement the main logic at Update() func which will be called at every 1/4 seconds.
	*/
public:
	enum manager_state { // state of the manager //delete static
		READY,		// ready to play, state will change to PAUSED by spacebar 
		ON,			// game is on. state will change to PAUSED by a keyboard input 'p' or 'P'
		PAUSED	// game is paused, Update() func will not do anything. User can kill or revive cells by clicking mouse left button.
	};
	enum rule_type { // enum for rule, manager will update its cells by "std::vector<rule_type>* rule" //delete static
		KILL,		// Kill a living cell
		AWAKE,	// Revive(awake) a dead cell
		STAY		// cell should stay whichever state they are
	};
	GameManager() = default;
	GameManager(int row, int col, int screen_width, int screen_height); // constructor for the manager. should be called only once at "main.cpp"
	~GameManager(); // destructor for the manager. this is actually useless func. however implement the destructor.

	manager_state state = READY;		// manager state. useful for managing the game.

	std::vector<std::pair<rule_type, rule_type>>* rule;			// vector variable for rule. this will be initialized at Init()
	std::vector<std::vector<Cell>> world;	// 2D vector variable for world. Initialized this and allocate each cells in it.
	
	void Init();	// initialize the world, set neighbors for each cells, and initialize rule
	void Init(const char* ruleFilePath);	// init with custom rule from rule file
	void Init(const char* ruleFilePath, const char* presetFilePath); // init with custom rule & preset wold from rule file and preset file
	void Update(); // update the world for each 1/4 seconds. your main logic should be implemented here.
	
	void Render();		// draw the objects to screen. DO NOT modify below funcs until you know exactly what you are doing
	void RenderUI();	// draw the user interface messages for each manager state. DO NOT modify below funcs until you know exactly what you are doing
	void ProcessKeyInput(unsigned char key, int x, int y);	// callback func for keyboard input. DO NOT modify below funcs until you know exactly what you are doing
	void ProcessMouseInput(int button, int state, int mx, int my); // callback func for mouse input. DO NOT modify below funcs until you know exactly what you are doing
	void ProcessContinuousMouseInput(int mx, int my); // callback func for mouse inptu. DO NOT modify below funcs until you know exactly what you are doing

private:
	int _row, _col;					// number of rows and columns in the world
	int _scr_width, _scr_height;		// width and height for the screen viewport
	float _cell_width, _cell_height;	// width and height for each cell

	void MouseCoord2CellCoord(int mx, int my, int& i, int& j);	

	void LoadRule(const char* filePath);
	void LoadPreset(const char* filePath);

	std::pair<int, int> _lastChanged = std::pair<int, int>(-1, -1); // variable for ProcessContinuousMouseInput being smoother
};