/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#include <stdlib.h>
#include <ctime>
#include <GL/glut.h>
#include <fstream>
#include "mesh.h"
#include "texture.h"
#include "render.h"
#include "controls.h"
#include "material.h"

#define terrainSamples 100 


void orientMe(float ang);

 // global
Mesh *floorPlane, *cubeMesh, *skybox, *mountains, *f16;
GLuint brickFloor, metalFloor, metalBox, woodBox, marbleBox, skyBox, terrain, f16List;
GLuint textures[6];


//timer info
std::clock_t start;
int timer = 0;

bool won;

int staticAngle, angle1, angle2;
float turning = 0;
int FAST = 0;

// menuListener
void menuListener(int option) {
	switch (option) {
	case 1:
		FAST = 1; break;
	case 2:
		FAST = 0; break;
	}
	glutPostRedisplay();
}

// create menu
void createMenus() {

	// add entries to speed
	int menuA = glutCreateMenu(menuListener);
	glutAddMenuEntry("faster", 1);
	glutAddMenuEntry("normal", 2);
	// create main menu
	int menu = glutCreateMenu(menuListener);
	glutAddSubMenu("Speed", menuA);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


// init
void init() {

	window_ratio = window_height / window_width;
	// mesh
	floorPlane = createPlane(2000, 2000, 200);
	cubeMesh= createCube();
	skybox = createSkyBox(6000);

	f16 = loadFile("_OBJ_files/f-16.obj");

	ImprovedNoise* noise = new ImprovedNoise();

	//initializes array for heightmap
	double** points = new double*[terrainSamples];
	for (int i = 0; i < terrainSamples; i++) {
		points[i] = new double[terrainSamples];
		for (int j = 0; j < terrainSamples; j++) {
			points[i][j] = 0;
		}
	}

	//fills heightmap with random values
	for (int i = 0; i < terrainSamples; i++) {
		for (int j = 0; j < terrainSamples; j++) {
			points[i][j] = noise->perlinMultiscale(i, j);
		}
	}
	mountains = createTerrain(100, points);



	// normals
	calculateNormalPerFace(floorPlane);
	calculateNormalPerVertex(floorPlane);

	calculateNormalPerFace(cubeMesh);
	calculateNormalPerVertex(cubeMesh);

	calculateNormalPerFace(cubeMesh);
	calculateNormalPerVertex(cubeMesh);

	calculateNormalPerFace(cubeMesh);
	calculateNormalPerVertex(cubeMesh);

	calculateNormalPerFace(skybox);
	calculateNormalPerVertex(skybox);

	calculateNormalPerFace(f16);
	calculateNormalPerVertex(f16);


	// textures
	loadBMP_custom(textures, "_BMP_files/brick.bmp", 0);
	loadBMP_custom(textures, "_BMP_files/oldbox.bmp", 1);
	codedTexture(textures, 2, 0); //Sky texture - noise multiscale. Type=0
	codedTexture(textures, 3, 1); //Marble texture - noise marble. Type=1
	loadBMP_custom(textures, "_BMP_files/cubesky.bmp", 4);
	loadBMP_custom(textures, "_BMP_files/metal.bmp", 5);
	loadBMP_custom(textures, "_BMP_files/grass.bmp", 6);
	


	// display lists
	brickFloor = meshToDisplayList(floorPlane, 1, textures[0]);
	metalBox = meshToDisplayList(cubeMesh, 2, textures[5]);
	woodBox = meshToDisplayList(cubeMesh, 3, textures[1]);
	marbleBox = meshToDisplayList(cubeMesh, 4, textures[3]);
	skyBox = meshToDisplayList(skybox, 5, textures[4]);
	terrain = meshToDisplayList(mountains, 6, textures[6]);
	f16List = meshToDisplayList(f16, 7, textures[5]);
	metalFloor = meshToDisplayList(floorPlane, 1, textures[5]);

	// configuration
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	//GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	start = std::clock();
}


void reshape(int w, int h) {
	window_width = w;
	window_height = h;
	if (h == 0) h = 1;
	window_ratio = 1.0f * w / h;
}

// text
void renderBitmapString(float x, float y, float z, const char *string, bool large) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++) {
		if (!large) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		}
		else {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
		
	}
}

// display
void display(void) {
	orientMe(cameraAngle);

	//resets clock when in starting area
	bool inStart = (x < 50 && x > -50) && (z < 50 && z > -50);

	if (inStart) {
		start = std::clock();
	}
	bool inFinish = (z < 9500 && z > 8000) && (x < -3500 && x > -5000);
	if (inFinish) {
		won = true;
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, window_height * .2, window_width, window_height * .8);
	gluPerspective(45, window_ratio, 10, 100000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// lookAt
	// gluLookAt(0.0f, 40.0f, 320.0,	0.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f);

	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

	// static rotation
	glPushMatrix();
	glTranslatef(0, 100, -800);
	glTranslatef(150 * cos(turning / 10), 0.0, 150 * sin(turning / 10));
	glCallList(woodBox);
	glPopMatrix();

	// stationary box 1
	glPushMatrix();
		glTranslatef(0, 100, -800);
		glCallList(metalBox);

	
		// primary orbit around box 1
		glPushMatrix();
			glRotatef(turning, 0, 0, 1);
			glTranslatef(300, 0, 0);
			glCallList(marbleBox);

			// moon/mini box in secondary orbit
			glPushMatrix();
				glRotatef(turning * 3, 0, 1, 0);
				glTranslatef(100, 0, 0);
				glScalef(.2, .2, .2);
				glCallList(marbleBox);
			glPopMatrix();

		glPopMatrix();
	glPopMatrix();


	//draw start and ending areas
	glPushMatrix();
	glTranslatef(-750, -700, -900);
	glScalef(.75, 1, .75);
	glCallList(brickFloor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5000, -600, 8000);
	glScalef(.75, 1, .75);
	glCallList(metalFloor);
	glPopMatrix();


	//draws airplane and its bounding box
	glPushMatrix();
	glTranslatef(x + lx * 20, y + ly * 20 , z + lz * 20);
	glRotatef(cameraAngle * -57.5 + 180, 0, 1, 0);
	glTranslatef(16, -75, 120);
	glScalef(10, 10, 10);
	glCallList(f16List);
	AABB(f16);
	glPopMatrix();

	// end

	//-9000, -8500, -9000
	// skybox
	glPushMatrix();
	glTranslatef(GLfloat(skyX), GLfloat(skyY), GLfloat(skyZ));
	glScalef(6.0, 6.0, 6.0);
	
	glDisable(GL_LIGHT0);
	GLfloat light_emissive2[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emissive2);
	glCallList(skyBox);
	glPopMatrix();

	glEnable(GL_LIGHT0);
	defaultmat();

	//mountains
	glPushMatrix();
	
	glTranslatef(-12000, 300, -12000);
	glScalef(300, 1200, 300);
	glCallList(terrain);
	glPopMatrix();

	// end
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	// texto
	glMatrixMode(GL_PROJECTION);


	glPushMatrix();
	glLoadIdentity();

	//game help text
	gluOrtho2D(0, window_width, 0, window_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, window_height - 13.0f, 0.0f, "Use [Arrows] to move in plane", false);
	renderBitmapString(0.0, window_height - 26.0f, 0.0f, "Hold [left mouse] to look and change direction", false);
	renderBitmapString(0.0, window_height - 39.0f, 0.0f, "Use [W and S] to speed up or slow down", false);
	renderBitmapString(0.0, window_height - 52.0f, 0.0f, "Use [Spacebar] to stop", false);

	//control viewport
	glViewport(0, 0, window_width, window_height * .2);

	string timeString = to_string(timer) + "s";

	/* for testing position
	string coords = to_string(x) + ", " + to_string(y) + ", "
		+ to_string(z) + ": " + to_string(inFinish);*/

	renderBitmapString(10, window_height * .65, 0.0f, "Time elapsed: ", true);
	renderBitmapString(180, window_height * .65, 0.0f, timeString.c_str(), true);

	
	if (won) {
		glColor3f(.1, 1, .1);
		renderBitmapString(500, window_height * .65, 0.0f, "You Win", true);
	}
	else if (!inStart && !won) {
		glColor3f(255, 0, 255);
		renderBitmapString(window_width - 400, window_height * .65, 0.0f, "Reach the metal surface", true);
	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();

	if (!won) {
		timer = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	}
}

// callback function for keyboard (alfanumeric keys)
void callbackKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		if(speed < 200)
		speed += (1);
		break;
	case 's': case 'S':
		if (speed > -200)
		speed -= (1);
		break;
	case ' ':
			speed = (0);
		break;
	}
}

void orientMe(float ang) {

	lx = sin(ang); lz = -cos(ang);

}

void moveMeFlat(int i) {

	float newX = x + i * (lx)*0.5;
	float newY = y + i * (ly)*0.5;
	float newZ = z + i * (lz)*0.5;
	//float newSkyX = skyX + i * (lx)*0.5;
	//float newSkyZ = skyZ + i * (lz)*0.5;

	//skyX = newSkyX;
	//skyZ = newSkyZ;
	x = newX;
	y = newY;
	z = newZ;
}

void mouse(int button, int state, int x, int y) {
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;
}

//lx = sin(ang); lz = -cos(ang);
// motion
void motion(int x, int y) {
	if (mouse_button == GLUT_LEFT_BUTTON) {

		cameraAngle += (float(x - mouse_x) / width);

		orientMe(cameraAngle);
		ly -= (float(y - mouse_y) / height);
	}
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

void mySpecial(int key, int x, int y) {

	switch (key) {

	case GLUT_KEY_UP:
		if (FAST)
			moveMeFlat(2.0);
		else
			moveMeFlat(20.0);
		break;

	case GLUT_KEY_DOWN:
		if (FAST)
			moveMeFlat(-2.0);
		else
			moveMeFlat(-20.0);

		break;

	case GLUT_KEY_LEFT:


		cameraAngle -= 0.04f;

		orientMe(cameraAngle);

		break;

	case GLUT_KEY_RIGHT:

		cameraAngle += 0.04f;

		orientMe(cameraAngle);

		break;


	}

	glutPostRedisplay();

}
//*/
void myIdle() {

	skyX = x - 18000;
	skyY = y - 18000;
	skyZ = z - 18000;
	moveMeFlat(speed);

	turning += .1;
	glutPostRedisplay();
}

// main
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Sky Box");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(myIdle);

	glutMotionFunc(motion);
	glutMouseFunc(mouse);

	//glutSpecialFunc(specialkeys);
	glutSpecialFunc(mySpecial);
	glutKeyboardFunc(callbackKeyboard);
	init();
	glutMainLoop();
	return 0;
}