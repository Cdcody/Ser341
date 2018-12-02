#pragma once

#include <math.h>
#include <GL/glut.h>
#include "menu.h"

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
int speed = 5, minSpeed = 0, maxSpeed = 50;
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


float jetRotateX = 0;
float jetRotateY = 0;
float jetPosition = 0;


int t = 0;
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

double x = 30.0f, y = 0.0f, z = -45.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;


double spectatorX = 0, spectatorY = 400, spectatorZ = 0, spectatorAngle = 0;
bool spectatorMode;


// callback function for keyboard (alfanumeric keys)
void callbackKeyboard(unsigned char key, int xx, int yy) {
	double temp = 0;

	if (spectatorMode) {
		switch (key) {
		case 'w': case 'W':
			spectatorX += 80 * sin(spectatorAngle);
			spectatorZ += 80 * -cos(spectatorAngle);
			break;
		case 's': case 'S':
			spectatorX -= 80 * sin(spectatorAngle);
			spectatorZ -= 80 * -cos(spectatorAngle);
			break;
		case 'a': case 'A':
			spectatorAngle -= .08;
			break;
		case 'd': case 'D':
			spectatorAngle += .08;
			break;
		case 'g': case 'G':
			spectatorMode = !spectatorMode;
			speed = minSpeed;
			break;
		case ' ':
			spectatorY += 10;
			break;
		case 'z': case 'Z':
			spectatorY -= 10;
		}
		return;
	}
	switch (key) {
	case 'w': case 'W':
		temp = (speed + 1) * 1.15;
		if (temp > maxSpeed)
			speed = maxSpeed;
		else
			speed = temp;
		break;
	case 's': case 'S':
		temp = (speed -1 )* .85;
		if (temp < minSpeed)
			speed = minSpeed;
		else
			speed = temp;
		break;
	case 'g': case 'G':
		spectatorMode = !spectatorMode;
		speed = minSpeed;
		break;
	case ' ':
		y += speed / 3;
		break;
	case 'z': case 'Z':
		temp = y - speed / 3;
		if (temp > 31 || !collision)
			y = temp;
		else
			y = 31;
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
	z = newZ;

	if(newY > 31)
		y = newY;
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
		jetRotateX += (float(x - mouse_x) / 25.0);
		jetRotateY += (float(y - mouse_y) / 25.0);
		jetPosition -= (float(y - mouse_y) / 15.0);
	}
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

void mySpecial(int key, int x, int y) {
	if (spectatorMode)
		return;
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
	if (spectatorMode) {
		skyX = spectatorX - 18000;
		skyY = spectatorY - 18000;
		skyZ = spectatorZ - 18000;
	}
	else {
		skyX = x - 18000;
		skyY = y - 18000;
		skyZ = z - 18000;
		moveMeFlat(speed);
	}

	turning += .1;
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

	//jet rotation
	if (jetRotateX < -20)
		jetRotateX += 0.4;
	else if (jetRotateX > 20)
		jetRotateX -= 0.4;
	else if (jetRotateX < -10)
		jetRotateX += 0.2;
	else if (jetRotateX > 10)
		jetRotateX -= 0.2;
	else if (jetRotateX > 0.1)
		jetRotateX -= 0.1;
	else if (jetRotateX < 0)
		jetRotateX += 0.1;
	


	
	if (!objectReverse) {
		objectPosition += clockSpeed;

		if (objectPosition >= 999) {
			objectPosition = 999;
			objectReverse = true;
		}
	}
	else {
		objectPosition -= clockSpeed;

		if (objectPosition <= 0) {
			objectPosition = 0;
			objectReverse = false;
		}
	}

	if (cameraMode) {
		if (/*t == 2 &&*/ cameraFrame < cameraVec->size()) {
		cameraX = (*cameraVec)[cameraFrame].x;
		cameraY = (*cameraVec)[cameraFrame].y;
		cameraZ = (*cameraVec)[cameraFrame].z;
		cameraFrame += 2;
		//t = 0;
		}
		else {
			cameraMode = false;
		}	
	}
	glutPostRedisplay();
}