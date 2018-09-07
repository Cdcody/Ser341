/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#include <stdlib.h>
#include <GL/glut.h>
#include <fstream>
#include "mesh.h"
#include "texture.h"
#include "render.h"
#include "controls.h"

#define terrainSamples 100 
 // global
Mesh *floorPlane, *cube1, *cube2, *cube3, *skybox, *mountains;
GLuint display1, display2, display3, display4, display5, display6;
GLuint textures[6];
int turning = 0, o = 0;
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
	// mesh
	floorPlane = createPlane(2000, 2000, 200);
	cube1 = createCube();
	cube2 = createCube();
	cube3 = createCube();
	skybox = createSkyBox(6000);

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

	calculateNormalPerFace(cube1);
	calculateNormalPerVertex(cube1);

	calculateNormalPerFace(cube2);
	calculateNormalPerVertex(cube2);

	calculateNormalPerFace(cube3);
	calculateNormalPerVertex(cube3);

	calculateNormalPerFace(skybox);
	calculateNormalPerVertex(skybox);


	// textures
	loadBMP_custom(textures, "_BMP_files/brick.bmp", 0);
	loadBMP_custom(textures, "_BMP_files/oldbox.bmp", 1);
	codedTexture(textures, 2, 0); //Sky texture - noise multiscale. Type=0
	codedTexture(textures, 3, 1); //Marble texture - noise marble. Type=1
	loadBMP_custom(textures, "_BMP_files/cubesky.bmp", 4);
	codedTexture(textures, 5, 2); //fire


	// display lists
	display1 = meshToDisplayList(floorPlane, 1, textures[0]);
	display2 = meshToDisplayList(cube1, 2, textures[5]);
	display3 = meshToDisplayList(cube2, 3, textures[2]);
	display4 = meshToDisplayList(cube3, 4, textures[3]);
	display5 = meshToDisplayList(skybox, 5, textures[4]);
	display6 = meshToDisplayList(mountains, 6, textures[4]);

	// configuration
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

// reshape
void reshape(int w, int h) {
	window_width = w;
	window_height = h;
}

// text
void renderBitmapString(float x, float y, float z, const char *string) {
	const char *c;
	glRasterPos3f(x, y, z);   // fonts position
	for (c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// display
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, 0, window_width, window_height);
	gluPerspective(45, window_ratio, 10, 100000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// lookAt
	// gluLookAt(0.0f, 40.0f, 320.0,	0.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f);

	gluLookAt(camera_x, camera_y, camera_z, camera_viewing_x, camera_viewing_y, camera_viewing_z, 0.0f, 1.0f, 0.0f);
	// camera
	//glScalef(scale, scale, scale);
	//glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	//glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	//glTranslatef(0.0f, 0.0f, 0.0f);

	//plane
	//glPushMatrix();
	//glTranslatef(-1000, 200, -1000);
	//glCallList(display1);
	//glPopMatrix();
	// box 1
	glPushMatrix();
	glTranslatef(0, 300, 0);
	glCallList(display2);
	glPopMatrix();
	// box 2
	glPushMatrix();
	glTranslatef(200, 300, 0);
	//glTranslatef(0, 17, 500);
	//glRotatef((GLint)rotating, 0.0, 1.0, 0.0);
	glRotatef((GLint)turning, 0.0, 1.0, 0.0);
	glTranslatef(-400.0, 0.0, 0.0);
	glCallList(display3);
	glPopMatrix();
	// box 3
	glPushMatrix();
	glTranslatef(-200, 300, 0);
	glCallList(display4);
	glPopMatrix();
	// end
	// skybox
	glPushMatrix();
	glTranslatef(-2000, -1, -2000);
	glScalef(1.5, 1.5, 1.5);
	glCallList(display5);
	glPopMatrix();

	//mountains
	glPushMatrix();
	glTranslatef(-200, 600, 0);
	glScalef(10, 500, 10);

	glCallList(display6);
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
	gluOrtho2D(0, window_width, 0, window_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, window_height - 13.0f, 0.0f, "Use [Arrows] to move in plain");
	renderBitmapString(0.0, window_height - 26.0f, 0.0f, "Use [W and S] to look up and down");
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();
}

// rotate what the user see
void rotate_point(float angle) {
	float s = sin(angle);
	float c = cos(angle);
	// translate point back to origin:
	camera_viewing_x -= camera_x;
	camera_viewing_z -= camera_z;
	// rotate point
	float xnew = camera_viewing_x * c - camera_viewing_z * s;
	float znew = camera_viewing_x * s + camera_viewing_z * c;
	// translate point back:
	camera_viewing_x = xnew + camera_x;
	camera_viewing_z = znew + camera_z;
}

// callback function for keyboard (alfanumeric keys)
void callbackKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W':
		camera_viewing_y += (10);
		break;
	case 's': case 'S':
		camera_viewing_y -= (10);
		break;
	}
}



// callback function for arrows
void specialkeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		total_moving_angle += -0.05;
		rotate_point(-0.05);
	}
	else if (key == GLUT_KEY_RIGHT) {
		total_moving_angle += 0.05;
		rotate_point(0.05);
	}
	else if (key == GLUT_KEY_DOWN) {
		//printf("Down key is pressed\n");
		//camera_z += 10;
		camera_x += (-10) * sin(total_moving_angle);//*0.1;
		camera_z += (-10) * -cos(total_moving_angle);//*0.1;
		//camera_viewing_y -= 10;
		camera_viewing_x += (-10) * sin(total_moving_angle);//*0.1;
		camera_viewing_z += (-10) * -cos(total_moving_angle);//*0.1;
	}
	else if (key == GLUT_KEY_UP) {
		//printf("Up key is pressed\n");
		//camera_z -= 10;
		camera_x += (10) * sin(total_moving_angle);//*0.1;
		camera_z += (10) * -cos(total_moving_angle);//*0.1;
		camera_viewing_x += (10) * sin(total_moving_angle);//*0.1;
		camera_viewing_z += (10) * -cos(total_moving_angle);//*0.1;
		//camera_viewing_y += 10;
	}
}
/*
void orientMe(float ang) {

	lx = sin(ang); lz = -cos(ang);

}





void moveMeFlat(int i) {

	float newX = x + i * (lx)*0.5;

	float newZ = z + i * (lz)*0.5;

	if (z < -25 || z > 25) {
		if (newX > -24 && newX < 24)
			x = newX;
	}
	else {
		if (newX > -99 && newX < 99)
			x = newX;
	}

	if (x < -25 || x > 25) {
		if (newZ > -24 && newZ < 24)
			z = newZ;
	}
	else {
		if (newZ > -99 && newZ < 99)
			z = newZ;
	}

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
			moveMeFlat(1.0);
		break;

	case GLUT_KEY_DOWN:
		if (FAST)
			moveMeFlat(-2.0);
		else
			moveMeFlat(-1.0);

		break;

	case GLUT_KEY_LEFT:

	 Testing testing
	 testing 
	 123123

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
*/
void myIdle() {
	if (o == 10) {
		turning = (turning + 1) % 360;
		o = 0;
	}
	o++;
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

	//glutMotionFunc(motion);
	//glutMouseFunc(mouse);

	glutSpecialFunc(specialkeys);
	glutKeyboardFunc(callbackKeyboard);
	init();
	glutMainLoop();
	return 0;
}