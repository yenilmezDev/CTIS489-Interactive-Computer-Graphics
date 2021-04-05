#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "camera.h"
#include "texture.h"
#include "LightSource.h"
#include "PointLightSource.h"
#include "Material.h"
#include "Model.h"
#include "stb_image.h"
#include <sstream>
#include <map>
#include <vector>
#include <time.h>


/*CTIS 489 MIDTERM 2 
21302450 
Huseyin YENILMEZ
*/


#define WINDOW_WIDTH  1600
#define WINDOW_HEIGHT 900

#define HALF_WIDTH (WINDOW_WIDTH >> 1)     
#define HALF_HEIGHT (WINDOW_HEIGHT >> 1)

// Perspective Camera Parameters
#define FOVY 70.0f
#define NEAR_PLANE 0.01f
#define FAR_PLANE  500.0f

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define MOUSE_SENSIVITY 4.0f

#define D2R 0.0174532

#define MAX_TREE 50
#define MAX_FLOWER 200

/* Global Variables for Template File */
bool upKey = false, downKey = false, rightKey = false, leftKey = false;
bool wKey = false, sKey = false, aKey = false, dKey = false, spaceKey = false;
int  winWidth, winHeight; // current Window width and height
bool mode = true;  // F1 key to toggle drawing mode between GL_LINE and GL_FILL
bool capture = true;  // Middle Mouse button set/unset mouse capture
int mouse_x = HALF_WIDTH;
int mouse_y = HALF_HEIGHT;

//
// APPLICATION DATA
//
Model models[6];
Model trees[4];
Model formula;
Camera cam(32, 6, 27, 179, -27, .5);
bool drawMode = false;  /* F1 to toggle between LINE and FILL drawing */
Texture flowerTexture;

typedef struct {
	int i, j;
	int type;
}tree_t;

typedef struct {
	int i, j;
	int type;
	float trX, trZ;
}flower_t;

tree_t treePos[MAX_TREE];
flower_t flowerPos[MAX_FLOWER];


typedef struct {
	int objIndex;
	float rotate;
}tile_t;

tile_t grass[22][22] = { 0 };

PointLightSource WhiteLS1 = {
	GL_LIGHT0, {1,1,1,1}, {1,1,1,1}, true
};

PointLightSource WhiteLS2 = {
	GL_LIGHT1, {1,1,1,1}, {1,1,1,1}, true
};

void texturedFlower() {

	glBindTexture(GL_TEXTURE_2D, flowerTexture.id);

	//one side
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, 0);

	glTexCoord2f(1, 0);
	glVertex3f(1, -1, 0);

	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 0);

	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, 0);
	glEnd();

}

void initGrass() {
	grass[4][2].objIndex = 1; grass[4][2].rotate = 90;
	grass[4][4].objIndex = 5; grass[4][4].rotate = 180;
	grass[4][5].objIndex = 5; grass[4][5].rotate = 180;
	grass[4][6].objIndex = 5; grass[4][6].rotate = 180;
	grass[3][7].objIndex = 1; grass[3][7].rotate = -90;
	grass[2][8].objIndex = 2; grass[2][8].rotate = 90;
	grass[2][9].objIndex = 5; grass[2][9].rotate = 180;
	grass[2][10].objIndex = 2; grass[2][10].rotate = 180;
	grass[3][10].objIndex = 5; grass[3][10].rotate = -90;
	grass[4][10].objIndex = 5; grass[4][10].rotate = -90;
	grass[5][10].objIndex = 5; grass[5][10].rotate = -90;
	grass[6][10].objIndex = 5; grass[6][10].rotate = -90;
	grass[7][10].objIndex = 5; grass[7][10].rotate = -90;
	grass[8][10].objIndex = 1; grass[8][10].rotate = 0;
	grass[9][12].objIndex = 5; grass[9][12].rotate = 0;
	grass[9][13].objIndex = 5; grass[9][13].rotate = 0;
	grass[9][14].objIndex = 5; grass[9][14].rotate = 0;
	grass[9][15].objIndex = 5; grass[9][15].rotate = 0;
	grass[9][16].objIndex = 5; grass[9][16].rotate = 0;
	grass[9][17].objIndex = 5; grass[9][17].rotate = 0;
	grass[9][18].objIndex = 1; grass[9][18].rotate = 180;
	grass[11][19].objIndex = 5; grass[11][19].rotate = -90;
	grass[12][19].objIndex = 5; grass[12][19].rotate = -90;
	grass[13][19].objIndex = 5; grass[13][19].rotate = -90;
	grass[14][19].objIndex = 5; grass[14][19].rotate = -90;
	grass[15][19].objIndex = 5; grass[15][19].rotate = -90;
	grass[16][19].objIndex = 2; grass[16][19].rotate = -90;
	grass[16][18].objIndex = 5; grass[16][18].rotate = 0;
	grass[15][16].objIndex = 4; grass[15][16].rotate = 90;
	grass[15][15].objIndex = 5; grass[15][15].rotate = 0;
	grass[15][14].objIndex = 5; grass[15][14].rotate = 0;
	grass[15][12].objIndex = 3; grass[15][12].rotate = 90;
	grass[16][11].objIndex = 5; grass[16][11].rotate = 0;
	grass[16][10].objIndex = 5; grass[16][10].rotate = 0;
	grass[16][9].objIndex = 5; grass[16][9].rotate = 0;
	grass[16][8].objIndex = 5; grass[16][8].rotate = 0;
	grass[15][6].objIndex = 1; grass[15][6].rotate = 0;
	grass[14][6].objIndex = 5; grass[14][6].rotate = 90;
	grass[13][6].objIndex = 5; grass[13][6].rotate = 90;
	grass[12][6].objIndex = 5; grass[12][6].rotate = 90;
	grass[10][5].objIndex = 1; grass[10][5].rotate = 180;
	grass[10][4].objIndex = 5; grass[10][4].rotate = 0;
	grass[10][3].objIndex = 5; grass[10][3].rotate = 0;
	grass[10][2].objIndex = 2; grass[10][2].rotate = 0;
	grass[9][2].objIndex = 5; grass[9][2].rotate = 90;
	grass[8][2].objIndex = 5; grass[8][2].rotate = 90;
	grass[7][2].objIndex = 5; grass[7][2].rotate = 90;
	grass[6][2].objIndex = 5; grass[6][2].rotate = 90;

}

void drawRoad() {

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			if (grass[i][j].objIndex != 0) {
				glDisable(GL_LIGHTING);
				glPushMatrix();

				if (grass[i][j].objIndex == 1 || grass[i][j].objIndex == 3 || grass[i][j].objIndex == 4)
					glTranslatef(i * 8 + 4, 0, j * 8 + 4);
				else
					glTranslatef(i * 8, 0, j * 8);

				glRotatef(grass[i][j].rotate, 0, 1, 0);
				models[grass[i][j].objIndex].render();
				glPopMatrix();
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
				glPushMatrix();
				glTranslatef(i * 8, -1, j * 8);
				models[0].render();
				glPopMatrix();
				glEnable(GL_LIGHTING);
			}

		}
	}
}

// Option 3: At least 20 trees with random types and positions on grass tiles only
void initRandomTrees() {

	bool inTheGrass;
	for (int k = 0; k < MAX_TREE; k++) {
		int i;
		int j;
		i = rand() % 21 + 1;
		j = rand() % 21 + 1;
		do {
			inTheGrass = true;
			i = rand() % 21 + 1;
			j = rand() % 21 + 1;
			treePos[k].i = i;
			treePos[k].j = j;
			treePos[k].type = rand() % 4;

			//check if the selected index is grass or not. If it is grass, check if it is large tile or not
			if (grass[i][j].objIndex != 0) {
				inTheGrass = false;
			}
			else if (grass[i - 1][j].objIndex == 1 || grass[i - 1][j].objIndex == 3 || grass[i - 1][j].objIndex == 4) {
				inTheGrass = false;
			}
			else if (grass[i][j - 1].objIndex == 1 || grass[i][j - 1].objIndex == 3 || grass[i][j - 1].objIndex == 4) {
				inTheGrass = false;
			}
			else if (grass[i - 1][j - 1].objIndex == 1 || grass[i - 1][j - 1].objIndex == 3 || grass[i - 1][j - 1].objIndex == 4) {
				inTheGrass = false;
			}


		} while (!inTheGrass);

		printf("i= %d, j=%d, objIndex: %d\n", treePos[k].i, treePos[k].j, grass[treePos[k].i][treePos[k].j].objIndex);

		//check if already exists
		for (int g = 0; g < k; g++) {

			if (treePos[g].i == i && treePos[g].j == j) {
				k--;
				//printf("for loop breaks.\n");
				break;
			}
		}
	}
}

void initRandomFlower() {
	bool inTheGrass;
	for (int k = 0; k < MAX_FLOWER; k++) {
		int i;
		int j;
		i = rand() % 21 + 1;
		j = rand() % 21 + 1;
		do {
			inTheGrass = true;
			i = rand() % 21 + 1;
			j = rand() % 21 + 1;
			flowerPos[k].i = i;
			flowerPos[k].j = j;
			//flowerPos[k].type = rand() % 4;
			flowerPos[k].trX = rand() % 4;
			flowerPos[k].trZ = rand() % 4;
			//check if the selected index is grass or not. If it is grass, check if it is large tile or not
			if (grass[i][j].objIndex != 0) {
				inTheGrass = false;
			}
			else if (grass[i - 1][j].objIndex == 1 || grass[i - 1][j].objIndex == 3 || grass[i - 1][j].objIndex == 4) {
				inTheGrass = false;
			}
			else if (grass[i][j - 1].objIndex == 1 || grass[i][j - 1].objIndex == 3 || grass[i][j - 1].objIndex == 4) {
				inTheGrass = false;
			}
			else if (grass[i - 1][j - 1].objIndex == 1 || grass[i - 1][j - 1].objIndex == 3 || grass[i - 1][j - 1].objIndex == 4) {
				inTheGrass = false;
			}


		} while (!inTheGrass);

		printf("Flower: i= %d, j=%d, objIndex: %d\n", flowerPos[k].i, flowerPos[k].j, grass[flowerPos[k].i][flowerPos[k].j].objIndex);

		//check if already exists
		for (int g = 0; g < k; g++) {

			if (flowerPos[g].i == i && flowerPos[g].j == j) {
				k--;
				//printf("for loop breaks.\n");
				break;
			}
		}
	}
}

void drawRandomTrees() {

	for (int i = 0; i < MAX_TREE; i++) {
		glPushMatrix();
		glTranslatef(treePos[i].i * 8, 4, treePos[i].j * 8);
		glScalef(0.02, 0.02, 0.02);
		trees[treePos[i].type].render();
		glPopMatrix();
	}

}


void drawFlower() {
	glPushMatrix();
	texturedFlower();
	glRotatef(90, 0, 1, 0);
	texturedFlower();
	glPopMatrix();
}

void drawRandomFlowers() {

	glDisable(GL_LIGHTING);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_ALPHA_TEST);
	//alpha test. If alpha of the pixel is greater than 0.5, it will be displayed.
	glAlphaFunc(GL_GREATER, 0.5);

	for (int i = 0; i < MAX_FLOWER; i++) {
		glPushMatrix();
		glTranslatef(flowerPos[i].i * 8 + flowerPos[i].trX, 1, flowerPos[i].j * 8 + +flowerPos[i].trZ);
		drawFlower();
		glPopMatrix();
	}

	glEnable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);
}

void drawCar() {
	glPushMatrix();
	glTranslatef(32, 1, 30);
	glScalef(4, 4, 4);
	formula.render();
	glPopMatrix();
}




//
// To display onto window using OpenGL commands
//
void display()
{
	glClearColor(0.5, 0.6, 0.85, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	cam.LookAt();

	glPushMatrix();
	glTranslatef(250, 50, 100); // position of point light source
	WhiteLS1.addToScene(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, 50, 100); // position of point light source
	WhiteLS2.addToScene(0.5);
	glPopMatrix();

	//drawing the scene without lighting
	drawRoad();

	drawRandomTrees();
	drawCar();
	drawRandomFlowers();




	// detech cam pos
	//printf("Cam: %.0f, %.0f, %.0f, %.0f, %.0f\n", cam.x, cam.y, cam.z, cam.yaw, cam.pitch);

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeydown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	switch (key) {
	case 'w':
	case 'W': wKey = true; break;
	case 's':
	case 'S': sKey = true; break;
	case 'a':
	case 'A': aKey = true; break;
	case 'd':
	case 'D': dKey = true; break;
	case ' ': spaceKey = true; break;
	}
}

void onKeyup(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	switch (key) {
	case 'w':
	case 'W': wKey = false; break;
	case 's':
	case 'S': sKey = false; break;
	case 'a':
	case 'A': aKey = false; break;
	case 'd':
	case 'D': dKey = false; break;
	case ' ': spaceKey = false; break;
	}
}

void onSpecialKeydown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: upKey = true;	break;
	case GLUT_KEY_DOWN: downKey = true; break;
	case GLUT_KEY_LEFT: leftKey = true; break;
	case GLUT_KEY_RIGHT: rightKey = true; break;
	case GLUT_KEY_F2: initRandomTrees(); initRandomFlower(); break;
	}
}

void onSpecialKeyup(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: upKey = false;  break;
	case GLUT_KEY_DOWN: downKey = false; break;
	case GLUT_KEY_LEFT: leftKey = false; break;
	case GLUT_KEY_RIGHT: rightKey = false; break;
	case GLUT_KEY_F1: drawMode = !drawMode;
		if (drawMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
}


void onClick(int button, int status, int x, int y)
{
	// Write your codes here.
	if (button == GLUT_MIDDLE_BUTTON && status == GLUT_DOWN) {
		capture = !capture;
		if (capture) {
			glutSetCursor(GLUT_CURSOR_NONE);
			glutWarpPointer(HALF_WIDTH, HALF_HEIGHT);
			mouse_x = HALF_WIDTH;
			mouse_y = HALF_HEIGHT;
		}
		else {
			glutSetCursor(GLUT_CURSOR_INHERIT);
		}
	}

}

void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, winWidth * 1.0f / winHeight, NEAR_PLANE, FAR_PLANE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	display(); // refresh window.
}

void onMove(int x, int y) {
	// Write your codes here.
	mouse_x = x;
	mouse_y = y;
}

void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.


	if (wKey) {
		cam.Forward();
	}
	if (sKey) {
		cam.Backward();
	}

	if (aKey) {
		cam.Left();
	}

	if (dKey) {
		cam.Right();
	}


	if (capture) {
		cam.TurnRightLeft((HALF_WIDTH - mouse_x) / MOUSE_SENSIVITY);
		cam.TurnUpDown((HALF_HEIGHT - mouse_y) / MOUSE_SENSIVITY);
		glutWarpPointer(HALF_WIDTH, HALF_HEIGHT);
	}

	glutPostRedisplay(); // display()

}

void Init() {

	WhiteLS1.init();
	WhiteLS2.init();
	srand(time(NULL));

	// Load Models
	models[0].init("models/racetrack", "terrain-grass.obj");
	models[1].init("models/racetrack", "track-corner-large.obj");
	models[2].init("models/racetrack", "track-corner-small.obj");
	models[3].init("models/racetrack", "track-left-right-large.obj");
	models[4].init("models/racetrack", "track-right-left-large.obj");
	models[5].init("models/racetrack", "track-straight-small.obj");

	trees[0].init("models/tree/tree1", "tree1.obj");
	trees[1].init("models/tree/tree2", "tree2.obj");
	trees[2].init("models/tree/tree3", "tree3.obj");
	trees[3].init("models/tree/tree4", "tree4.obj");

	formula.init("models/car", "car-formula.obj");

	flowerTexture.Load("texture/flower1.png");

	//inits the map
	initGrass();

	//inits the random trees
	initRandomTrees();

	//inits random flower
	initRandomFlower();

	// Enable lighting and texturing
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE); // to normalize Normal vector for uniform scaling.


	// set the cursor position
	glutWarpPointer(HALF_WIDTH, HALF_HEIGHT);
	// hide cursor
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);


}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CTIS489 - Midterm #2");

	Init();

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeydown);
	glutSpecialFunc(onSpecialKeydown);

	glutKeyboardUpFunc(onKeyup);
	glutSpecialUpFunc(onSpecialKeyup);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutPassiveMotionFunc(onMove);

	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	glutMainLoop();
}