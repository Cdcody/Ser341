#pragma once

#include <GL/glut.h>

void defaultmat() {
	GLfloat ambient[] = { .2, .2, .2, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { .8, .8, .8, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}

void obsidianmat() {
	//Obsidian	0.05375	0.05	0.06625	1	0.18275	0.17	0.22525	1	0.332741	0.328634	0.346435	1	38.4
	GLfloat ambient1[] = { 0.05375,	0.05,	0.06625,	1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);

	GLfloat diffuse1[] = { 0.18275,	0.17,	0.22525,	1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);

	GLfloat specular1[] = { 0.332741,	0.328634,	0.346435,	1 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);

	glMaterialf(GL_FRONT, GL_SHININESS, 38.4);
}

void plasticmat() {
	//Plastic (White)	0	0	0	1	0.55	0.55	0.55	1	0.7	0.7	0.7	1	32
	GLfloat plastic_ambient[] = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, plastic_ambient);

	GLfloat plastic_diffuse[] = { .55, .55, .55, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plastic_diffuse);

	GLfloat plastic_specular[] = { .7, .7, .7, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plastic_specular);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32);
}

void emeraldmat() {
	//Emerald	0.0215	0.1745	0.0215	1	0.07568	0.61424	0.07568	1	0.633	0.727811	0.633	1	76.8
	GLfloat ambient2[] = { .0215, .1745, .0215, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient2);

	GLfloat diffuse2[] = { .07568, .61424, .07568, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse2);

	GLfloat specular2[] = { .633, .727811, .633, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular2);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 76.8);
}

void bronzemat() {
	//Bronze	0.2125	0.1275	0.054	1	0.714	0.4284	0.18144	1	0.393548	0.271906	0.166721	1	25.6
	GLfloat bronze_ambient[] = { .2125, .1275, .054, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bronze_ambient);

	GLfloat bronze_diffuse[] = { .714, .4284, .18144, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bronze_diffuse);

	GLfloat bronze_specular[] = { .393548, .271906, .166721, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bronze_specular);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 25.6);
}

void chromemat() {
	//Chrome	0.25	0.25	0.25	1	0.4	0.4	0.4	1	0.774597	0.774597	0.774597	1	76.8
	GLfloat ambient[] = { .25, .25, .25, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { .4, .4, .4, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { .774597, .774597, .774597, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 76.8);
}
void jademat() {
	//Jade	0.135	0.2225	0.1575	1	0.54	0.89	0.63	1	0.316228	0.316228	0.316228	1	12.8
	GLfloat ambient[] = { 0.135,	0.2225,	0.1575,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 0.54,	0.89,	0.63,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 0.316228,	0.316228,	0.316228,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	GLfloat emissive[] = { 0.135 / 3,	0.2225 / 3,	0.1575 / 3,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 12.8);
}
void goldmat() {
	//Gold	0.24725	0.1995	0.0745	1	0.75164	0.60648	0.22648	1	0.628281	0.555802	0.366065	1	51.2
	GLfloat ambient[] = { 0.24725,	0.1995,	0.0745,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 0.75164,	0.60648,	0.22648,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 0.628281,	0.555802,	0.366065,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	GLfloat emissive[] = { 0.24725,	0.1995,	0.0745,	1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 51.2);
}
void rubbermat() {
	//Rubber(Red)	0.05	0	0	1	0.5	0.4	0.4	1	0.7	0.04	0.04	1	10
	GLfloat ambient[] = { 0.05,	0,	0,	1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 0.5,	0.4,	0.4,	1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 0.7,	0.04,	0.04,	1 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	glMaterialf(GL_FRONT, GL_SHININESS, 10);
}