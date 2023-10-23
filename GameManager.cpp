#include "GameManager.h"

GameManager::GameManager(int row, int col, int screen_width, int screen_height)
{
	// initialize row, col, scr_with, scr_height
	_row = row;
	_col = col;
	_scr_width = screen_width;
	_scr_height = screen_height;

	// get cell_width and cell_height for rendering
	_cell_width = (float)_scr_width / (float)_row;
	_cell_height = (float)_scr_height / (float)_col;
}

GameManager::~GameManager()
{
	// destructor for the manager. this is actually useless func. however implement the destructor.
	/*
	clear up used memory space.
	*/	
	delete[] & _row, _col, _scr_width, _scr_height;		// width and height for the screen viewport
	delete[] & _cell_width, _cell_height;;
}

void GameManager::Init()
{
	// initialize the world, set neighbors for each cells, and initialize rule

	// initialize world as a vector of cells >>>>>
	/*
	Initialize 2D veector world variable.
	push back cells with Cell::Cell(int x_pos, int y_pos) constructor at each i, j position.
	After that, we may take out the cell at (i, j) position as following; world.at(i).at(j)
	*/
	world.erase(world.begin(), world.end());
	for (int i = 0; i < _row; i++) {
		std::vector<Cell> depart;
		for (int j = 0; j < _col; j++) {
			depart.push_back(Cell(i, j));
		}
		world.push_back(depart);
	}

	// <<<<< initialize world as a vector of cells 

	// initialize neighbors of  each cells >>>>>
	/*
	for every cells in world, set 8 neighbors. you should call Cell::SetNeighbors 8 times to set  
	bottom left,  bottom center,  bottom right,
	mid left, mid right,
	top left, top center, top right cells.
	Also, connect bottom edge with top edge and left edge with right edge as its neighbors just like the secret gate in PACMAN game.
	*/
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {
			world.at(i).at(j).SetNeighbors(&world.at((i - 1 + _row) % _row).at((j - 1 + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i - 1 + _row) % _row).at((j + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i - 1 + _row) % _row).at((j + 1 + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i + _row) % _row).at((j - 1 + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i + _row) % _row).at((j + 1 + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i + 1 + _row) % _row).at((j - 1 + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i + 1 + _row) % _row).at((j + _col) % _col));
			world.at(i).at(j).SetNeighbors(&world.at((i + 1 + _row) % _row).at((j + 1 + _col) % _col));
		}
	}

	// <<<<< initialize neighbors of  each cells
		
	// initialize a rule >>>>>

	rule = new std::vector<std::pair<rule_type, rule_type>>();

	// without a preset rule, the basic rule is 23/3

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 0

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 1

	rule->push_back(std::pair<rule_type, rule_type>(STAY, STAY)); // 2

	rule->push_back(std::pair<rule_type, rule_type>(AWAKE, STAY)); // 3

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 4

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 5

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 6

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 7

	rule->push_back(std::pair<rule_type, rule_type>(STAY, KILL)); // 8

																  // <<<<< initialize a rule


	// set the manager state as ON
	state = ON;
}

void GameManager::Init(const char * ruleFilePath)
{
	// init with custom rule from rule file
	Init();
	state = PAUSED;
	LoadRule(ruleFilePath);
	state = ON;
}

void GameManager::Init(const char * ruleFilePath, const char * presetFilePath)
{	  
	// init with custom rule & preset wold from rule file and preset file
	Init(ruleFilePath);
	state = PAUSED;
	LoadPreset(presetFilePath);
	state = ON;
}

void GameManager::Update()
{
	// update the world for each 1/4 seconds. your main logic should be implemented here.

	// if state is not ON, do nothing
	if (state != ON) {
		return;
	}

	// Main Logic of Game of Life >>>>>
	/*
	count alive neighbors for each cell,
	and decide whether kill, awake, do nothing with that cell
	*/	
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {
			int num = world.at(i).at(j).CountNeighbors();
			if (!world.at(i).at(j).CurLive()) { if (num == 3) world.at(i).at(j).Awake(); }
			else if (num < 2) world.at(i).at(j).Kill();
			else if (num == 2 || num == 3 && world.at(i).at(j).GetState() ) world.at(i).at(j).Awake();
			else if (num > 3)world.at(i).at(j).Kill();
		}
	}
	
	/*
	call Cell::Update for every cell in the world to change its current state(_curState)
	*/	
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {
			world[i][j].Update();
		}
	}

	//<<<<< Main Logic of Game of Life
}

void GameManager::Render()
{	
	// check every cell whether it is dead or not, and draw it on the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {			
			// get color by cell state
			if (world.at(i).at(j).GetState() == Cell::DEAD)
				glColor3f(1.0f, 1.0f, 1.0f);
			else if (world.at(i).at(j).GetState() == Cell::ALIVE)
				glColor3f(0.0f, 0.0f, 0.0f);

			// draw cells with border ratio 0.06
			glRectf(
				(_cell_width * i + 0.06f*_cell_width) / (float)_scr_width,
				(_cell_height * j+ 0.06f*_cell_height) / (float)_scr_height,
				(_cell_width * (i+1)- 0.06f*_cell_width) / (float)_scr_width,
				(_cell_height * (j+1)- 0.06f*_cell_height) / (float)_scr_height
			);
		}
	}

	// draw user interface messages additionally
	RenderUI();

	// glutSwapBuffers() will show the image on your screen
	glutSwapBuffers();
}

void GameManager::RenderUI()
{		
	// render user interface messages depend on the manager state
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_LIGHTING);
	if (state == READY) {
		std::string msg = "Press spacebar to start!";
		const unsigned char* t = reinterpret_cast<const unsigned char *>(msg.c_str());
		glRasterPos2f(0.4f, 0.5f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);
	}

	if (state == PAUSED) {
		std::string msg = "PAUSED!\n Click to set pattern! \nPress p to resume!";
		const unsigned char* t = reinterpret_cast<const unsigned char *>(msg.c_str());
		glRasterPos2f(0.0f, 0.9f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);
	}

	if (state == PAUSED || state == ON) {
		std::string msg = "Press R to clear!";
		const unsigned char* t = reinterpret_cast<const unsigned char *>(msg.c_str());
		glRasterPos2f(0.8f, 0.95f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);
	}
}

void GameManager::ProcessKeyInput(unsigned char key, int x, int y)
{
	// process keyboard input from user
	switch (key) {
	case 27: // esc key
		glutLeaveMainLoop();
		return;
	case 'p':
		if (state == ON) state = PAUSED;
		else if (state == PAUSED) state = ON;
		return;
	case 'P':
		if (state == ON) state = PAUSED;
		else if (state == PAUSED) state = ON;
		return;
	case 'r':
		if (state == ON || state == PAUSED) {
			Init();
			state = PAUSED;
		}
		return;
	case 'R':
		if (state == ON || state == PAUSED) {
			Init();
			state = PAUSED;
		}
		return;
	case ' ':
		if (state == READY) {
			Init();
			state = PAUSED;
			return;
		}
		if (state == PAUSED) state = ON;
		return;
	}
}

void GameManager::ProcessMouseInput(int button, int state, int mx, int my)
{
	// process mouse input from user
	if (this->state == ON || this->state == READY) return;
	int i = -1, j = -1;
	MouseCoord2CellCoord(mx, my, i, j);

	if (state == GLUT_DOWN) {
		if (world.at(i).at(j).GetState() == Cell::DEAD) world.at(i).at(j).Awake();
		else if (world.at(i).at(j).GetState() == Cell::ALIVE) world.at(i).at(j).Kill();
		world.at(i).at(j).Update();
	}
	_lastChanged = std::pair<int, int>(i, j);
}

void GameManager::ProcessContinuousMouseInput(int mx, int my)
{
	// process mouse input from user
	if (this->state == ON || this->state == READY) return;
	int i = -1, j = -1;
	MouseCoord2CellCoord(mx, my, i, j);
	if (i == _lastChanged.first && j == _lastChanged.second) return;

	if (world.at(i).at(j).GetState() == Cell::DEAD) world.at(i).at(j).Awake();
	else if (world.at(i).at(j).GetState() == Cell::ALIVE) world.at(i).at(j).Kill();
	world.at(i).at(j).Update();
	_lastChanged = std::pair<int, int>(i, j);
}

void GameManager::MouseCoord2CellCoord(int mx, int my, int & i, int & j)
{
	// translate mouse clicked coordinates to our cell-coordinates
	i = mx / _cell_width;
	j = (_scr_height - my) / _cell_height;
}

void GameManager::LoadRule(const char * filePath)
{
	// update rule from text file
	std::ifstream load;
	load.open(filePath, std::ios::out);
	if (load.fail()) return;
	load.close();
}

void GameManager::LoadPreset(const char * filePath)
{
	// update world from text file
	std::ifstream load;
	load.open(filePath, std::ios::out);
	if (load.fail()) return;

	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {
			int a = 0;
			load >> a;
			if (a == 1) world.at(i).at(j).Awake();
			world.at(i).at(j).Update();
		}
	}
	load.close();

}
