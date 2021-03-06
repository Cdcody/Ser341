#pragma once

#define NOMINMAX

// Include C++ headers
#include <algorithm>
using namespace std;

// Include Windows headers
#include <windows.h>
#include <gdiplus.h>

#include <ImathVec.h>
#include <ImathMatrix.h>
#include <GL/glut.h>
#include <vector>
#include "mesh.h"



using namespace Imath;
using namespace std;

typedef Vec3<float> Vec3f;
typedef Vec2<float> Vec2f;

void AABB(Mesh* m);
void drawCube(float* f);
float* findBounds(Mesh* m);


typedef Matrix44<float> Mat4f;

//finds min and max of x,y,z in xmin, xmax, ymin, ymax..... format
float* findBounds(Mesh* m) {
	float xmin = 0, xmax = 0,
		ymin = 0, ymax = 0,
		zmin = 0, zmax = 0;

	for (unsigned int ii = 0; ii < m->dot_vertex.size(); ii++)
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

void boundsToVertices(float* bounds, Vec3f* targetArray) {
	float xmin = bounds[0];
	float xmax = bounds[1];
	float ymin = bounds[2];
	float ymax = bounds[3];
	float zmin = bounds[4];
	float zmax = bounds[5];

	targetArray[0] = Vec3f(xmin, ymin, zmin);
	targetArray[1] = Vec3f(xmax, ymin, zmin);
	targetArray[2] = Vec3f(xmax, ymin, zmax);
	targetArray[3] = Vec3f(xmin, ymin, zmax);

	targetArray[4] = Vec3f(xmin, ymax, zmin);
	targetArray[5] = Vec3f(xmax, ymax, zmin);
	targetArray[6] = Vec3f(xmax, ymax, zmax);
	targetArray[7] = Vec3f(xmin, ymax, zmax);
}

void verticesToBounds(Vec3f* vertices, float* bounds) {
	bounds[0] = vertices[0].x;
	bounds[1] = vertices[5].x;

	bounds[2] = vertices[0].y;
	bounds[3] = vertices[5].y;

	bounds[4] = vertices[0].z;
	bounds[5] = vertices[5].z;
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

void formatMatrix(float* model, float result[4][4]) {
	float array[4][4] = {
	{model[0], model[1], model[2], model[3]},
	{model[4], model[5], model[6], model[7]},
	{model[8], model[9], model[10], model[11]},
	{model[12], model[13], model[14], model[15]},
	};

	result = array;
}

//calculates center of mesh by averaging coordinates
Vec3f findCenter(Mesh* m) {
	float sumX = 0, sumY = 0, sumZ = 0;

	int size = m->dot_vertex.size();
	for (unsigned int ii = 0; ii < size; ii++)
	{
		Vec3f pofloat = m->dot_vertex[ii];

		sumX += pofloat.x;
		sumY += pofloat.y;
		sumZ += pofloat.z;
	}
	return Vec3f(sumX / size, sumY / size, sumZ / size);
}

//finds point with greatest radius from center
float findRadius(Mesh* m, Vec3f center) {
	float max = 0;

	for (unsigned int ii = 0; ii < m->dot_vertex.size(); ii++)
	{
		Vec3f pofloat = m->dot_vertex[ii];

		Vec3f dist = pofloat - center;
		float thisRadius = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
		if (thisRadius >= max)
			max = thisRadius;
	}
	return max;
}


boolean checkCollision(float* bounds1, float* bounds2) {
	float xmin1 = bounds1[0],
	xmax1 = bounds1[1],
	ymin1 = bounds1[2],
	ymax1 = bounds1[3],
	zmin1 = bounds1[4],
	zmax1 = bounds1[5];

	float xmin2 = bounds2[0],
	xmax2 = bounds2[1],
	ymin2 = bounds2[2],
	ymax2 = bounds2[3],
	zmin2 = bounds2[4],
	zmax2 = bounds2[5];


	if ((xmax1 < xmin2) ||
		(xmin1 > xmax2))
		return false;

	if ((ymax1 < ymin2) ||
		(ymin1 > ymax2))
		return false;

	if ((zmax1 < zmin2) ||
		(zmin1 > zmax2))
		return false;

	return true;
}