/**
 * SER 431
 * https://speakerdeck.com/javiergs/ser431-lecture-04
 **/

#pragma once

#include <math.h>
#include <GL/glut.h>

int window_width = 800, window_height = 800;
float window_ratio = 1.0;

// controling parameters
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.3;
float x_angle = 0.0;
float y_angle = 0.0;
float cameraAngle = 0;
int width = 1200;
int height = 600;
int speed = 1;
int skyX = -18000;
int skyY = -17000;
int skyZ = -18000;
bool won;

//box parameters
vector<Vec3f>* boxVec;
int boxFrame = 0;
float boxX = 0;
float boxY = 100;
float boxZ = -800;


int t = 0;
float clockSpeed = 1;
float turning = 0;


//camera parameters
vector<Vec3f>* cameraVec;
bool cameraMode = true;
int cameraFrame = 0;
int cameraX, cameraY, cameraZ;

vector<Vec3f>* objectVec;//stores path of object moving along a curve
int objectPosition = 0;
bool objectReverse = false;

// camera
//float camera_x = 0.0;
//float camera_y = 300.0;
//float camera_z = 320.0;


float total_moving_angle = 0.0;

float x = 0.0f, y = 0.0f, z = 0.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;


// callback function for keyboard (alfanumeric keys)
void callbackKeyboard(unsigned char key, int xx, int yy) {
	switch (key) {
	case 'w': case 'W':
		if (speed < 200)
			speed += (1);
		break;
	case 's': case 'S':
		if (speed > -200)
			speed -= (1);
		break;
	case 'g': case 'G':
		cameraMode = true;
		speed = 0;
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
		moveMeFlat(5 * clockSpeed);
		break;

	case GLUT_KEY_DOWN:
		moveMeFlat(-5 * clockSpeed);
		break;

	case GLUT_KEY_LEFT:


		cameraAngle -= 0.04f * clockSpeed;

		orientMe(cameraAngle);

		break;

	case GLUT_KEY_RIGHT:

		cameraAngle += 0.04f * clockSpeed;

		orientMe(cameraAngle);

		break;


	}

	glutPostRedisplay();

}
void myIdle() {
	skyX = x - 18000;
	skyY = y - 18000;
	skyZ = z - 18000;
	moveMeFlat(speed * clockSpeed);

	turning += .1 * clockSpeed;
	int boxTurn = 1;
	//box curve
	if (boxTurn == 1) {
		boxX = (*boxVec)[boxFrame].x;
		boxY = (*boxVec)[boxFrame].y;
		boxZ = (*boxVec)[boxFrame].z;
		boxFrame++;
		if (boxFrame == 999)
			boxTurn = 0;
		//t = 0;
	}
	if (boxTurn == 0) {
		boxX = (*boxVec)[boxFrame].x;
		boxY = (*boxVec)[boxFrame].y;
		boxZ = (*boxVec)[boxFrame].z;
		boxFrame--;
		if (boxFrame == 1)
			boxTurn = 1;
		//t = 0;
	}
	
	if (!objectReverse) {
		objectPosition++;

		objectReverse = (objectPosition == 999);
	}
	else {
		objectPosition--;

		objectReverse = (!objectPosition == 0);
	}

	if (cameraMode) {
		if (/*t == 2 &&*/ cameraFrame < 1000) {
			cameraX = (*cameraVec)[cameraFrame].x;
		cameraY = (*cameraVec)[cameraFrame].y;
		cameraZ = (*cameraVec)[cameraFrame].z;
		cameraFrame += 2;
		//t = 0;
		}
		//t++;
		
	}
	if (cameraFrame == 1000)
		cameraMode = false;
	
	glutPostRedisplay();
}