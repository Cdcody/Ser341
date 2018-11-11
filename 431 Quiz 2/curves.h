#pragma once


/**
 * Hermite Curves
 */

#include <stdlib.h>
#include <GL/glut.h>
#include "mesh.h"

vector<Vec3f>* hermiteCurve(float geometry[][3], int points) {
	vector<Vec3f>* vec = new vector<Vec3f>();

	for (int i = 0; i != points; ++i) {
		float t = (float)i / (points - 1);
		// calculate blending functions
		float b0 = 2 * t*t*t - 3 * t*t + 1;
		float b1 = -2 * t*t*t + 3 * t*t;
		float b2 = t * t*t - 2 * t*t + t;
		float b3 = t * t*t - t * t;
		// calculate the x, y and z of the curve point
		float x = b0 * geometry[0][0] + b1 * geometry[1][0] + b2 * geometry[2][0] + b3 * geometry[3][0];
		float y = b0 * geometry[0][1] + b1 * geometry[1][1] + b2 * geometry[2][1] + b3 * geometry[3][1];
		float z = b0 * geometry[0][2] + b1 * geometry[1][2] + b2 * geometry[2][2] + b3 * geometry[3][2];
		// specify the point
		Vec3f temp = Vec3f(x, y, z);
		vec->push_back(temp);
	}
	return vec;
}

vector<Vec3f>* chaikinCurve(float geometry[][3], int points) {
	vector<Vec3f>* vec = new vector<Vec3f>(points);

	vec->push_back(Vec3f(geometry[0][0], geometry[0][1], geometry[0][2]));
	for (unsigned int i = 0; i < (points - 1); ++i) {
		// get 2 original points
		Vec3f p0 = Vec3f(geometry[i][0], geometry[i][1], geometry[i][2]);
		Vec3f p1 = Vec3f(geometry[i + 1][0], geometry[i + 1][1], geometry[i + 1][2]);

		Vec3f Q;
		Vec3f R;
		// average the 2 original points to create 2 new points. For each CV, another 2 verts are created.
		Q.x = 0.75f*p0.x + 0.25f*p1.x;
		Q.y = 0.75f*p0.y + 0.25f*p1.y;
		Q.z = 0.75f*p0.z + 0.25f*p1.z;
		R.x = 0.25f*p0.x + 0.75f*p1.x;
		R.y = 0.25f*p0.y + 0.75f*p1.y;
		R.z = 0.25f*p0.z + 0.75f*p1.z;
		vec->push_back(Q);
		vec->push_back(R);
	}
	// keep the last point
	vec->push_back(Vec3f(geometry[points - 1][0], geometry[points - 1][1], geometry[points - 1][2]));

	return vec;
}
