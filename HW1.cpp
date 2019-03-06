/*********
CTIS164 - Template Source Program
----------
STUDENT : M. Kasım VAROL
ID Number: 21602808
SECTION : 01
HOMEWORK: 1
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  25 // Period for the timer.
#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool activeTimer = false;
double xIncDec[4] = { 0 }; //x move
bool comeBack[4] = { 0 }; //check it if it needs to turn back
int counter = 0; // time count
bool info = true; // check if race is over
int winner; // show the winner
int r = 255, g = 255, b = 255; // initial random colors

//for F1 restart all events

void restartAll()
{
	for(int i=0; i<4; i++)
	{
	xIncDec[i] = 0;
	comeBack[i] = 0;
	}
	activeTimer = false; counter = 0; info = true;

}
						  
// to draw circle, center at (x,y), radius=r
						  
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}
void drawLine(float x1, float y1, float x2, float y2) {
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}
//ship template
void drawShip(int moveX, float y1) {
	//spaceship's glass
	glColor3f(1, 0.9, 1);
	circle(-350+moveX, 100+y1, 30);
	//alien's head
	glColor3f(0.1, 1, 0.1);
	circle(-350+moveX, 110+y1, 15);
	//spaceship's body
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	glVertex2f(-380+moveX, 100+ y1);
	glVertex2f(-400+moveX, 70+ y1);
	glVertex2f(-300+moveX, 70+y1);
	glVertex2f(-320+moveX, 100+ y1);
	glEnd();
	//spaceship's body light
	glColor3ub((r+173)%256, (g+39)%256, (b+83)%256);
	glBegin(GL_QUADS);
	glVertex2f(-375 + moveX, 70 + y1);
	glVertex2f(-390 + moveX, 55 + y1);
	glVertex2f(-310 + moveX, 55 + y1);
	glVertex2f(-325 + moveX, 70 + y1);
	glEnd();
	//alien's eyes
	glColor3f(0, 0, 0);
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(-355+moveX, 115+ y1);
	glVertex2f(-345+moveX, 115+ y1);
	glEnd();
	//alien's smile :)
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-360+moveX, 110+ y1);
	glVertex2f(-355+moveX, 105+ y1);
	glVertex2f(-345+moveX, 105+ y1);
	glVertex2f(-340+moveX, 110+ y1);
	glEnd();
	//spaceship's antenna
	glBegin(GL_LINES);
	glColor3f(1, 0.1, 0);
	glVertex2f(-350+moveX, 130+ y1);
	glVertex2f(-350+moveX, 140+ y1);
	glEnd();
	//antenna's receiver & random colour
	glColor3ub(r, g, b);
	circle(-350+moveX, 140+ y1, 3);
	//spaceship's lights

	glColor3f(1, 0, 0);
	circle(-375+moveX, 80+ y1, 5); 
	glColor3f(0, 1, 0);	
	circle(-350+moveX, 80+ y1, 5);
	glColor3f(0, 0, 1);
	circle(-325+moveX, 80+ y1, 5);
	//number of spaceships'
	vprint(-365+moveX, 88+ y1, GLUT_BITMAP_8_BY_13, "[0%.f]", (-y1/100)+1);
}
void display() {
	// spacey colour
	glClearColor(0, 0, 0.1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	//black background of top
	glColor3f(0, 0, 0);
	glRectf(-400, 150, 400, 250);
	//draw 4 ships
	for (int i = 0; i < 4; i++)
		drawShip(xIncDec[i], -i*100);
	//lines
	glColor3f(0.0, 1.0, 1.0);
	for (int i = -winHeight / 2 / 50 * 50; i <= winHeight / 2; i += 100)
		drawLine(-400, i, 400, i);
	/*INFORMATIONS*/
	//time count
	glColor3f(1, 1, 1);
	vprint(-350, 225, GLUT_BITMAP_8_BY_13, "TIMER");
	vprint2(-348, 210, 0.1, "%d:%02d", counter/40/60,  (counter/40)%60);
	//show winner
	glColor3f(1, 1, 1);
	vprint(275, 225, GLUT_BITMAP_8_BY_13, "WINNER");
	if(activeTimer)
		vprint(290, 210, GLUT_BITMAP_9_BY_15, "%02d", winner+1);
	glColor3f(1, 0, 0);
	for (int w = 0; w < 4; w++)
	{
		if (!info && xIncDec[w] <= 0)
			vprint(290, 210, GLUT_BITMAP_9_BY_15, "%02d", winner + 1);
	}
	//rectangle background for animation's informations
	glColor3f(0.6, 0.6, 0.6);
	glRectf(-250, 185, 200, 225);
	
	//animation's information
	glColor3f(0.5, 1, 0);
	vprint(-245, 200, GLUT_BITMAP_9_BY_15, "Racing Alien Zekiyes' Animation by M. KASIM VAROL");
	
	//button information table
	glColor3f(1, 0.5, 0);
	if(info)
		vprint(-235, 170, GLUT_BITMAP_8_BY_13, "Press <SpaceBar> to stop or resume Alien Zekiyes");
	else
		vprint(-235, 170, GLUT_BITMAP_8_BY_13, "Press <F1> to RESTART these exciting animation.");
	glutSwapBuffers();

}

void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ') 
		if(info)
		activeTimer = !activeTimer; //to stop or resume timer.

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	case GLUT_KEY_F1:  restartAll(); break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	//random colours
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
	if (activeTimer) 
	{
		int i;
		counter++;
		winner = 0;
		srand(time(NULL));
		for (i = 0; i < 4; i++)
		{
			//while going 
			if (comeBack[i] == 0) 
			{
				xIncDec[i] += rand() % 3 + 1; 
				if (xIncDec[i] >= winWidth - 100)
					comeBack[i] = 1;
			}
			//while returning
			else {
				xIncDec[i] -= rand() % 3 + 1;
				if (xIncDec[i] <= 0) {
					activeTimer = !activeTimer;
					info = false;
				}
			}//determine winner
			if (comeBack[0] == 0 && comeBack[1] == 0 && comeBack[2] == 0 && comeBack[3] == 0)
			{
				if (xIncDec[i] > xIncDec[winner])
					winner = i;
			}
			else
			{
				if (xIncDec[i] < xIncDec[winner])
					winner = i;
			}
		}
	}
	glutPostRedisplay(); // display()
}

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("HomeWork-1 | Aliens' Racing");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);


	glutTimerFunc(1000, onTimer, 0);

	Init();

	glutMainLoop();
}