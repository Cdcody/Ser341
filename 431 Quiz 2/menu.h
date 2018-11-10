#pragma once

#include <stdlib.h>
#include <ctime>
#include <GL/glut.h>



bool shading = false;




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

	// create main menu
	int menu = glutCreateMenu(speedListener);
	glutAddSubMenu("Speed", speedMenu);
	glutAddSubMenu("Shading", shadingMenu);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}