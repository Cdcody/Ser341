#pragma once

#include <stdlib.h>
#include <ctime>
#include <GL/glut.h>



bool shading = false;
bool particles = true;
bool bounding = true;
bool imageTextures = true;
bool proceduralTextures = true;
bool light = true;
bool materials = true;
bool collision = true;
bool multiscaleTerrain = false;
bool fog = false;
bool reflections = true;
bool shadows = true;
bool fractals = true;



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

void fogListener(int option) {
	switch (option) {
	case 1:
		fog = false;
		break;
	case 2:
		fog = true;
		break;
	}
	glutPostRedisplay();
}

void fractalListener(int option) {
	switch (option) {
	case 1:
		fractals = false;
		break;
	case 2:
		fractals = true;
		break;
	}
	glutPostRedisplay();
}

void shadowListener(int option) {
	switch (option) {
	case 1:
		shadows = false;
		break;
	case 2:
		shadows = true;
		break;
	}
	glutPostRedisplay();
}

void reflectionListener(int option) {
	switch (option) {
	case 1:
		reflections = false;
		break;
	case 2:
		reflections = true;
		break;
	}
	glutPostRedisplay();
}

void textureListener(int option) {
	switch (option) {
	case 1:
		imageTextures = false;
		break;
	case 2:
		imageTextures = true;
		break;
	case 3:
		proceduralTextures = false;
		break;
	case 4:
		proceduralTextures = true;
		break;
	}
	glutPostRedisplay();
}

void materialListener(int option) {
	switch (option) {
	case 1:
		materials = false;
		break;
	case 2:
		materials = true;
		break;
	}
	glutPostRedisplay();
}

void lightListener(int option) {
	switch (option) {
	case 1:
		light= false;
		break;
	case 2:
		light = true;
		break;
	}
	glutPostRedisplay();
}

void terrainListener(int option) {
	switch (option) {
	case 1:
		multiscaleTerrain = false;
		break;
	case 2:
		multiscaleTerrain = true;
		break;
	}
	glutPostRedisplay();
}

void collisionListener(int option) {
	switch (option) {
	case 1:
		bounding = false;
		break;
	case 2:
		bounding = true;
		break;
	case 3:
		collision = false;
		break;
	case 4:
		collision = true;
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

	int collisionMenu = glutCreateMenu(collisionListener);
	glutAddMenuEntry("bounding volumes off", 1);
	glutAddMenuEntry("bounding volumes on", 2);
	glutAddMenuEntry("collision off", 3);
	glutAddMenuEntry("collision on", 4);

	int textureMenu = glutCreateMenu(textureListener);
	glutAddMenuEntry("image textures off", 1);
	glutAddMenuEntry("image textures on", 2);
	glutAddMenuEntry("procedural textures of", 3);
	glutAddMenuEntry("procedural textures on", 4);

	int shadowMenu = glutCreateMenu(shadowListener);
	glutAddMenuEntry("shadows off", 1);
	glutAddMenuEntry("shadows on", 2);

	int reflectionMenu = glutCreateMenu(reflectionListener);
	glutAddMenuEntry("reflections off", 1);
	glutAddMenuEntry("reflections on", 2);

	int fogMenu = glutCreateMenu(fogListener);
	glutAddMenuEntry("fog off", 1);
	glutAddMenuEntry("fog on", 2);

	int fractalMenu = glutCreateMenu(fractalListener);
	glutAddMenuEntry("fractals off", 1);
	glutAddMenuEntry("fractals on", 2);

	int materialMenu = glutCreateMenu(materialListener);
	glutAddMenuEntry("materials off", 1);
	glutAddMenuEntry("materials on", 2);

	int lightMenu = glutCreateMenu(lightListener);
	glutAddMenuEntry("lighting off", 1);
	glutAddMenuEntry("lighting on", 2);

	int terrainMenu = glutCreateMenu(terrainListener);
	glutAddMenuEntry("flat terrain", 1);
	glutAddMenuEntry("multiscale terrain", 2);

	// create main menu
	int menu = glutCreateMenu(speedListener);
	glutAddSubMenu("Speed", speedMenu);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddSubMenu("Particles", particleMenu);
	glutAddSubMenu("Shadows", shadowMenu);
	glutAddSubMenu("Reflections", reflectionMenu);
	glutAddSubMenu("Fog", fogMenu);
	glutAddSubMenu("Textures", textureMenu);
	glutAddSubMenu("Light", lightMenu);
	glutAddSubMenu("Materials", materialMenu);
	glutAddSubMenu("Terrain", terrainMenu);
	glutAddSubMenu("Collision", collisionMenu);
	glutAddSubMenu("Fractal Objects", fractalMenu);


	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}