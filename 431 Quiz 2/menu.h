#pragma once

#include <stdlib.h>
#include <ctime>
#include <GL/glut.h>



bool shading = false;
bool particles = true;
bool bounding = false;



// menuListener
void speedListener(int option) {
	switch (option) {
	case 1:
		clockSpeed = 1; break;
	case 2:
		clockSpeed = 2; break;
	case 3:
		clockSpeed = 8; break;
	case 4:
		clockSpeed = 15; break;
	}
	glutPostRedisplay();
}

void shadingListener(int option) {
	switch (option) {
	case 1:
		shading = false;
		break;
	case 2:
		shading = true;
		break;
	}
	glutPostRedisplay();
}


void particleListener(int option) {
	switch (option) {
	case 1:
		particles = false;
		break;
	case 2:
		particles = true;
		break;
	}
	glutPostRedisplay();
}

void boundingListener(int option) {
	switch (option) {
	case 1:
		bounding = false;
		break;
	case 2:
		bounding = true;
		break;
	}
	glutPostRedisplay();
}

// create menu
void createMenus() {
	// add entries to speed
	int speedMenu = glutCreateMenu(speedListener);
	glutAddMenuEntry("slow", 1);
	glutAddMenuEntry("normal", 2);
	glutAddMenuEntry("fast", 3);
	glutAddMenuEntry("seizure", 4);

	int shadingMenu = glutCreateMenu(shadingListener);
	glutAddMenuEntry("smooth", 1);
	glutAddMenuEntry("flat", 2);

	int particleMenu = glutCreateMenu(particleListener);
	glutAddMenuEntry("particles off", 1);
	glutAddMenuEntry("particles on", 2);

	int boundingMenu = glutCreateMenu(boundingListener);
	glutAddMenuEntry("bounding boxes off", 1);
	glutAddMenuEntry("bounding boxes on", 2);

	// create main menu
	int menu = glutCreateMenu(speedListener);
	glutAddSubMenu("Speed", speedMenu);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddSubMenu("Particles", particleMenu);
	glutAddSubMenu("Bounding boxes", boundingMenu);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}