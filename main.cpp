#include "GameManager.h"
/*
Simple implementation of Conway's Game Of Life.
by YHG @ MILAB (youaredead@snu.ac.kr)
*/

// hard coded configuration variables >>>>>
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

const unsigned int ROW = 50;
const unsigned int COL = 50;

const int FPS = 4;
// <<<<< hard coded variables

// Instantiate the GameManager
GameManager manager(ROW, COL, SCREEN_WIDTH, SCREEN_HEIGHT);

// lists of glut config func and callback funcs >>>>>
void init();
void render();
void keyboardInput(unsigned char key, int x, int y); 
void mouseInput(int button, int state, int mx, int my); 
void timerCallback(int); 
void windowResizeCallback(int width, int height);
void continuousMouseInput(int mx, int my);
// <<<<< lists of glut callback funcs

int main(int argc, char** argv) {
	// initialize the manager before rendering, yet set its state as READY
	manager.Init();
	manager.state = GameManager::READY;

	// bind glut callback funcs >>>>>
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Game Of Life");
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboardInput);
	glutMouseFunc(mouseInput);	
	glutReshapeFunc(windowResizeCallback);
	glutMotionFunc(continuousMouseInput);
	// <<<<< bind glut callback funcs

	init(); // config glut properties

	glutTimerFunc(1000 / FPS, timerCallback, 0); // bind glut timer callback funcs

	// start the main loop
	glutMainLoop();

	return 0;
}

void init()
{
	glClearColor(0.2f, 0.3f, 0.6f, 1.0f); // background color(the border line color)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	manager.Render();
}

void keyboardInput(unsigned char key, int x, int y)
{
	manager.ProcessKeyInput(key, x, y);
}

void mouseInput(int button, int state, int mx, int my)
{
	manager.ProcessMouseInput(button, state, mx, my);
}

void timerCallback(int)
{
	manager.Update();
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timerCallback, 0);
}

void windowResizeCallback(int width, int height) {
	glutReshapeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void continuousMouseInput(int mx, int my)
{
	manager.ProcessContinuousMouseInput(mx, my);
}
