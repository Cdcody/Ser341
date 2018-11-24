#pragma once

#include <math.h>
#include <GL/glut.h>
#include <ImathVec.h>
#include <vector>
#include "mesh.h"


struct AABB {
	Vec3f pmin;
	Vec3f pmax;
};


float* findBounds(Mesh* m) {
	float xmin = 0, xmax = 0,
		ymin = 0, ymax = 0,
		zmin = 0, zmax = 0;

	for (float ii = 0; ii < m->dot_vertex.size(); ii++)
	{
		Vec3f pofloat = m->dot_vertex[ii];

		if (pofloat.x > xmax) xmax = pofloat.x;
		else if (pofloat.x < xmin) xmin = pofloat.x;

		if (pofloat.y > ymax) ymax = pofloat.y;
		else if (pofloat.y < ymin) ymin = pofloat.y;

		if (pofloat.z > zmax) zmax = pofloat.z;
		else if (pofloat.z < zmin) zmin = pofloat.z;
	}

	float* temp = new float[6];
	temp[0] = xmin;
	temp[1] = xmax;
	temp[2] = ymin;
	temp[3] = ymax;
	temp[4] = zmin;
	temp[5] = zmax;

	return temp;
}

//visual studio gives an error but still runs here
struct AABB findAABB(Mesh* m) {
	float xmin = 0, xmax = 0,
		ymin = 0, ymax = 0,
		zmin = 0, zmax = 0;

	for (float ii = 0; ii < m->dot_vertex.size(); ii++)
	{
		Vec3f pofloat = m->dot_vertex[ii];

		if (pofloat.x > xmax) xmax = pofloat.x;
		else if (pofloat.x < xmin) xmin = pofloat.x;

		if (pofloat.y > ymax) ymax = pofloat.y;
		else if (pofloat.y < ymin) ymin = pofloat.y;

		if (pofloat.z > zmax) zmax = pofloat.z;
		else if (pofloat.z < zmin) zmin = pofloat.z;
	}

	Vec3f pmin = Vec3f(xmin, ymin, zmin);
	Vec3f pmax = Vec3f(xmax, ymax, zmax);

	struct AABB bound;
	bound.pmin = pmin;
	bound.pmax = pmax;
	return bound;
}

//takes the min and max x y and z coordinates and draws lines representing a bounding box
void drawCube(float* bounds) {
	float xmin = bounds[0];
	float xmax = bounds[1];
	float ymin = bounds[2];
	float ymax = bounds[3];
	float zmin = bounds[4];
	float zmax = bounds[5];

	glBegin(GL_LINES);

	glColor3f(1, 1, 1);
	glLineWidth(15);


	glVertex3f(xmin, ymin, zmin);
	glVertex3f(xmin, ymin, zmax);

	glVertex3f(xmin, ymin, zmin);
	glVertex3f(xmin, ymax, zmin);

	glVertex3f(xmin, ymin, zmin);
	glVertex3f(xmax, ymin, zmin);

	glVertex3f(xmin, ymin, zmax);
	glVertex3f(xmin, ymax, zmax);

	glVertex3f(xmin, ymax, zmin);
	glVertex3f(xmin, ymax, zmax);

	glVertex3f(xmin, ymax, zmin);
	glVertex3f(xmax, ymax, zmin);

	glVertex3f(xmax, ymin, zmin);
	glVertex3f(xmax, ymax, zmin);

	glVertex3f(xmax, ymin, zmin);
	glVertex3f(xmax, ymin, zmax);

	glVertex3f(xmin, ymin, zmax);
	glVertex3f(xmax, ymin, zmax);

	glVertex3f(xmax, ymax, zmin);
	glVertex3f(xmax, ymax, zmax);

	glVertex3f(xmin, ymax, zmax);
	glVertex3f(xmax, ymax, zmax);

	glVertex3f(xmax, ymin, zmax);
	glVertex3f(xmax, ymax, zmax);

	glEnd();
}

void AABB(Mesh* m) {
	float* bounds = findBounds(m);

	drawCube(bounds);
	delete(bounds);
}