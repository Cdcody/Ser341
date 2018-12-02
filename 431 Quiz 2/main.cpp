//Chris Cody and Justin V.

#include <stdlib.h>
#include <ctime>
#include <GL/glut.h>
#include <fstream>
#include "mesh.h"
#include "texture.h"
#include "render.h"
#include "controls.h"
#include "curves.h"
#include "material.h"
#include "menu.h"
#include "particles.h"
#include "timer.h"
#include "curves.h"
#include "collision.h"
#include "gameObject.h"
#include "fractals.h"


#define terrainSamples 100 

#define V_NUMPOINTS    4
#define U_NUMPOINTS	   4
#define V_NUMKNOTS    (V_NUMPOINTS + 4)
#define U_NUMKNOTS    (U_NUMPOINTS + 4)

// Knot sequences for cubic bezier surface and trims 
GLfloat sknots[V_NUMKNOTS] = { 0., 0., 0., 0., 1., 1., 1., 1. };
GLfloat tknots[U_NUMKNOTS] = { 0., 0., 0., 0., 1., 1., 1., 1. };

// Control points for the flag. The Z values are modified to make it wave
GLfloat ctlpoints[V_NUMPOINTS][U_NUMPOINTS][3] = {
	{ { 0., 3., 0. },{ 1., 3., 0. },{ 2., 3., 0 },{ 3., 3., 0. } },
	{ { 0., 2., 0. },{ 1., 2., 0. },{ 2., 2., 0 },{ 3., 2., 0. } },
	{ { 0., 1., 0. },{ 1., 1., 0. },{ 2., 1., 0 },{ 3., 1., 0. } },
	{ { 0., 0., 0. },{ 1., 0., 0. },{ 2., 0., 0 },{ 3., 0., 0. } }
};

GLfloat texpoints[2][2][2] =
{
{ { 1.0f, 0.0f }, { 1.0f, 1.0f } },
{ { 0.0f, 0.0f }, { 0.0f, 1.0f } }
};

GLfloat textureknots[4] = { 0.0, 0.0, 1.0, 1.0 }; // Texture in knots


GLUnurbsObj *nurbsflag;


 // global
Mesh *floorPlane, *floorPlane2, *cubeMesh, *skybox, *terrainMesh, *f16, *gem;
GLuint brickFloor, metalFloor, metalBox, woodBox, marbleBox, skyBox, grassyTerrain, f16List, fireCube,
plainFloor, plainCube, plainTerrain, grassyFloor, goldDiamond;
GLUnurbsObj *theNurb;
GLuint textures[10];

ParticleSystem* jetFlame = new ParticleSystem();

//timer info
std::clock_t start;
int timer = 0;
float particleTimer = 0;

int remaining = 10;
int staticAngle, angle1, angle2;

// Shadows
GLfloat light_position[4];
GLfloat shadow_matrix[4][4];
Vec3f floor_normal;
vector<Vec3f> dot_vertex_floor;
float lightAngle = 0.0, lightHeight = 500;


// calculate floor normal
void calculate_floor_normal(Vec3f *plane, vector<Vec3f> dot_floor) {
	Vec3<GLfloat> AB = dot_floor[1] - dot_floor[0];
	Vec3<GLfloat> AC = dot_floor[2] - dot_floor[0];
	*plane = AB.cross(AC);
}

// Create a matrix that will project the desired shadow
void shadowMatrix(GLfloat shadowMat[4][4], Vec3f plane_normal, GLfloat lightpos[4]) {
	GLfloat dot;
	Vec3f lightpos_v; lightpos_v.x = lightpos[0]; lightpos_v.y = lightpos[1]; lightpos_v.z = lightpos[2];
	dot = plane_normal.dot(lightpos_v);
	shadowMat[0][0] = dot - lightpos[0] * plane_normal[0];
	shadowMat[1][0] = 0.f - lightpos[0] * plane_normal[1];
	shadowMat[2][0] = 0.f - lightpos[0] * plane_normal[2];
	shadowMat[3][0] = 0.f - lightpos[0] * plane_normal[3];
	shadowMat[0][1] = 0.f - lightpos[1] * plane_normal[0];
	shadowMat[1][1] = dot - lightpos[1] * plane_normal[1];
	shadowMat[2][1] = 0.f - lightpos[1] * plane_normal[2];
	shadowMat[3][1] = 0.f - lightpos[1] * plane_normal[3];
	shadowMat[0][2] = 0.f - lightpos[2] * plane_normal[0];
	shadowMat[1][2] = 0.f - lightpos[2] * plane_normal[1];
	shadowMat[2][2] = dot - lightpos[2] * plane_normal[2];
	shadowMat[3][2] = 0.f - lightpos[2] * plane_normal[3];
	shadowMat[0][3] = 0.f - lightpos[3] * plane_normal[0];
	shadowMat[1][3] = 0.f - lightpos[3] * plane_normal[1];
	shadowMat[2][3] = 0.f - lightpos[3] * plane_normal[2];
	shadowMat[3][3] = dot - lightpos[3] * plane_normal[3];
}

GameObject* jet;
int diamondScale = 15;

vector<GameObject*> objects = vector<GameObject*>();



//3d surface parameters
const int V_size = 6;
const int U_size = 6;
const int ORDER = 4;
GLfloat mountainControlPoints[U_size][V_size][3] = {
	{ { 25, 5,  15 } ,{ 20, 5,  15 },{ 0, 10,  15 },{ -5, 10,  15 },{ -10, 5,  15 }   ,{ -15, 5,  15 } },
	{ { 25, 5,  10 } ,{ 20, 0,  10 },{ 0, 20,  10 },{ -5, 19,  10 },{ -10, 0,  10 }   ,{ -15, 5,  10 } },
	{ { 25, 0,   5 } ,{ 20, 0,   5 },{ 0, 15,   5 },{ -5, 15,   5 },{ -10, 12,   5 } ,{ -15, 0,  5 } },
	{ { 25, 0,  -5 } ,{ 20, 0,  -5 },{ 0, 40,  -5 },{ -5, 35,  -5 },{ -10, 8,  -5 } ,{ -15, 0,  -5 } },
	{ { 25, 5,  -10 } ,{ 20, 0, -10 },{ 0, 0, -10 },{ -5, 0, -10 },{ -10, 0, -10 }   ,{ -15, 5,  -10 } },
	{ { 25, 0,  -15 } ,{ 20, -5, -15 },{ 0, 0, -15 },{ -5, 0, -15 },{ -10, 5, -15 }   ,{ -15, 5,  -15 } }
};
GLfloat vknots[V_size + ORDER] = { 0.0, 0.0, 0.0, 0.0, 1.0, 3.0, 5.0, 5.0, 5.0, 5.0 };
GLfloat uknots[U_size + ORDER] = { 0.0, 0.0, 0.0, 0.0, 1.0, 3.0, 5.0, 5.0, 5.0, 5.0 };



//camera path parameters 
GLfloat cameraPathControlPoints[4][3] = {
	{ -800, 800, 0 },
	{ 0, 0, -300 },
	{ 800, 0, 0 },
	{ 0, 0, -800 },
};


//box path parameters
GLfloat boxPathControlPoints[4][3] = {
	{ 500, 100, -800 },
	{ -500, 100, -800 },
	{ 500, 300, -1000 },
	{ -500, 300, -1000 },
};

void placeObstacle(GLfloat xPos, GLfloat yPos, GLfloat zPos, int speed) {
	// stationary box 1
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	//imageTextures ? glCallList(metalBox) : glCallList(plainCube);   //this line adds the stationary box

	// primary orbit around box 1
	glPushMatrix();
	glRotatef(turning * speed, 0, 0, 1);
	glTranslatef(300, 0, 0);
	proceduralTextures ? glCallList(marbleBox) : glCallList(plainCube);

	// moon/mini box in secondary orbit
	glPushMatrix();
	glRotatef(turning * (speed + 3), 0, 1, 0);
	glTranslatef(100, 0, -800);
	glScalef(.2, .2, .2);
	proceduralTextures ? glCallList(marbleBox) : glCallList(plainCube);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}




//object path parameters
GLfloat objectPathControlPoints[4][3] = {
	{ 400, 0,  -800},
	{ 40, 0, 0 },
	{ 800,  0, 0 },
	{ 300, -400, -800 },
};

// init
void init() {
	CreateTreeLists();
	init_frame_timer();
	createMenus();

	cameraVec = hermiteCurve(cameraPathControlPoints, 400);
	objectVec = hermiteCurve(objectPathControlPoints, 1000);
	boxVec = hermiteCurve(boxPathControlPoints, 1000);

	window_ratio = window_height / window_width;
	// mesh
	floorPlane = createPlane(2000, 2000, 150);
	floorPlane2 = createPlane(2000, 2000, 10);
	cubeMesh = createCube();
	skybox = createSkyBox(6000);

	f16 = loadFile("_OBJ_files/f-16.obj");
	gem = loadFile("_OBJ_files/diamond.obj");

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
	terrainMesh = createTerrain(100, points);



	// normals
	calculateNormalPerFace(floorPlane);
	calculateNormalPerVertex(floorPlane);

	calculateNormalPerFace(cubeMesh);
	calculateNormalPerVertex(cubeMesh);

	calculateNormalPerFace(skybox);
	calculateNormalPerVertex(skybox);

	calculateNormalPerFace(f16);
	calculateNormalPerVertex(f16);


	// textures
	loadBMP_custom(textures, "_BMP_files/brick.bmp", 0);
	loadBMP_custom(textures, "_BMP_files/oldbox.bmp", 1);
	codedTexture(textures, 2, 0); //Sky texture - noise multiscale. Type=0
	codedTexture(textures, 3, 1); //Marble texture - noise marble. Type=1
	loadBMP_custom(textures, "_BMP_files/cubesky.bmp", 4);
	loadBMP_custom(textures, "_BMP_files/metal.bmp", 5);
	loadBMP_custom(textures, "_BMP_files/gold.bmp", 7);
	loadBMP_custom(textures, "_BMP_files/grass.bmp", 6);
	codedTexture(textures, 8, 2);
	


	// display lists
	plainFloor = meshToDisplayList(floorPlane, 1, NULL);
	brickFloor = meshToDisplayList(floorPlane, 1, textures[0]);
	metalFloor = meshToDisplayList(floorPlane, 1, textures[5]);
	grassyFloor = meshToDisplayList(floorPlane2, 1, textures[6]);

	plainTerrain = meshToDisplayList(terrainMesh, 6, NULL);
	grassyTerrain = meshToDisplayList(terrainMesh, 6, textures[6]);

	metalBox = meshToDisplayList(cubeMesh, 2, textures[5]);
	woodBox = meshToDisplayList(cubeMesh, 3, textures[1]);
	marbleBox = meshToDisplayList(cubeMesh, 4, textures[3]);
	fireCube = meshToDisplayList(cubeMesh, 8, textures[7]);
	plainCube = meshToDisplayList(cubeMesh, 8, NULL);
	skyBox = meshToDisplayList(skybox, 5, textures[4]);

	f16List = meshToDisplayList(f16, 7, textures[5]);
	goldDiamond = meshToDisplayList(gem, 10, textures[7]);

	jet = new GameObject(f16, 10, f16List);

	for (int ii = 0; ii < 10; ii++) {
		GameObject* o = new GameObject(gem, 4, goldDiamond);
		objects.push_back(o);
	}


	
	//glEnable(GL_DEPTH_TEST);
	// light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 100.0, 1.0, 0.0 };
	//GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_POINT_SMOOTH);


	glFogi(GL_FOG_MODE, GL_LINEAR);
	float fogColor[] = { .7, .7, .7, .2 };
	glFogfv(GL_FOG_COLOR, fogColor );
	glFogf(GL_FOG_DENSITY, .02);

	glFogf(GL_FOG_START, 4000);
	glFogf(GL_FOG_END, 10000);
	
	glCullFace(GL_BACK);

	start = std::clock();


	// shadow
	glClearStencil(0);
	// floor vertex
	dot_vertex_floor.push_back(Vec3<GLfloat>(-2000.0, 11.0, 2000.0));
	dot_vertex_floor.push_back(Vec3<GLfloat>(2000.0, 11.0, 2000.0));
	dot_vertex_floor.push_back(Vec3<GLfloat>(2000.0, 11.0, -2000.0));
	dot_vertex_floor.push_back(Vec3<GLfloat>(-2000.0, 11.0, -2000.0));
	calculate_floor_normal(&floor_normal, dot_vertex_floor);
	// light
	

	// STENCIL|STEP 2. NEW LINES
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearStencil(0); //define the value to use as clean.
}

void reshape(int w, int h) {
	window_width = w;
	window_height = h;
	if (h == 0) h = 1;
	window_ratio = 1.0f * w / h;
}

void drawMountain() {
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 200);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsProperty(theNurb, GLU_CULLING, true);

	glPushMatrix();
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb,
		U_size + ORDER, uknots,
		V_size + ORDER, vknots,
		V_size * 3,
		3,
		&mountainControlPoints[0][0][0],
		ORDER, ORDER,
		GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);
	
	// control graph
	glDisable(GL_LIGHTING);
	glPointSize(1.0);
	glColor3f(0, 0, 1);
	for (int i = 0; i < U_size; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < V_size; j++) {
			glVertex3f(mountainControlPoints[i][j][0], mountainControlPoints[i][j][1], mountainControlPoints[i][j][2]);
		}
		glEnd();
	}
	// show control points
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < U_size; i++) {
		for (int j = 0; j < V_size; j++) {
			glVertex3f(mountainControlPoints[i][j][0], mountainControlPoints[i][j][1], mountainControlPoints[i][j][2]);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void drawFlag() {
	nurbsflag = gluNewNurbsRenderer();
	gluNurbsProperty(nurbsflag, GLU_SAMPLING_TOLERANCE, 25.0f);
	gluNurbsProperty(nurbsflag, GLU_DISPLAY_MODE, GLU_FILL);
	gluNurbsProperty(nurbsflag, GLU_CULLING, true);

	static GLfloat angle = 0.0;
	int i, j;
	// wave the flag by rotating Z coords though a sine wave
	for (i = 1; i < 4; i++)
		for (j = 0; j < 4; j++)
			ctlpoints[i][j][2] = sin((GLfloat)i + angle);
	angle += 0.1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, textures[3]);

	glPushMatrix();
	glTranslatef(2.5, -1.0, 0.0);
	glScalef(1.5, 1.0, 1.0);
	glRotatef(90, 0., 0., 1.);

	gluBeginSurface(nurbsflag);

	gluNurbsSurface(nurbsflag,
		V_NUMKNOTS, sknots,
		U_NUMKNOTS, tknots,
		3 * U_NUMPOINTS,
		3,
		&ctlpoints[0][0][0],
		4, 4,
		GL_MAP2_VERTEX_3);


	gluNurbsSurface(nurbsflag,
		4, textureknots,
		4, textureknots,
		2 * 2,
		2,
		&texpoints[0][0][0],
		2, 2,
		GL_MAP2_TEXTURE_COORD_2); 
	/*gluNurbsSurface(nurbsflag,
		2, textureknots,
		2, textureknots,
		2 * U_NUMPOINTS, // u stride=#control points in v direction X size of data
		2, //v stride
		&texpoints[0][0][0], //control points
		2, 2, // order of u nurbs and v nurbs
		GL_MAP2_TEXTURE_COORD_2);*/
		
	gluEndSurface(nurbsflag);
	glPopMatrix();
}

// display
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// light source position
	light_position[0] = 500 * cos(lightAngle);
	light_position[1] = 1500;
	light_position[2] = 500 * sin(lightAngle);
	light_position[3] = 0.0; // directional light
	lightAngle += 0.0005;
	// Calculate Shadow matrix
	shadowMatrix(shadow_matrix, floor_normal, light_position);


	glDisable(GL_FOG);
	orientMe(cameraAngle);

	// configuration
	if (shading) {
		glShadeModel(GL_FLAT);
	}
	else {
		glShadeModel(GL_SMOOTH);
	}

	if (light) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport(0, window_height * .2, window_width, window_height * .8);
	gluPerspective(45, window_ratio, 10, 100000);
	// view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// lookAt
	// gluLookAt(0.0f, 40.0f, 320.0,	0.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f);


	if (shadows) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	}

	// Draw floor using blending to blend in reflection
	
	/*commented out temporarily
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 0.3);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	//glRotatef(90, 1, 0, 0);
	glTranslatef(-12000, (multiscaleTerrain ? 300 : -1000), -12000);
	glScalef(300, 1200, 300);
	glCallList(grassyFloor);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glDisable(GL_BLEND);
	*/

	// Shadows

	if (shadows) {
		glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); 
		//  To eliminate depth buffer artifacts, use glEnable(GL_POLYGON_OFFSET_FILL);
		// Render 50% black shadow color on top of whatever the floor appareance is
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);  /* Force the 50% black. */
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPushMatrix();
		// Project the shadow
		glMultMatrixf((GLfloat *)shadow_matrix);
		// jet shadow
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslatef(0, jetPosition, 0);
		glTranslatef(x + lx * 20, 0, z + lz * 20);
		glRotatef(cameraAngle * -57.5 + 180, 0, 1, 0);
		glTranslatef(16, 0, 120);//y was -75
		glRotatef(jetRotateY, 1, 0, 0);
		glRotatef(jetRotateX, 0, 0, 1);
		glScalef(10, 10, 10);

		glCallList(f16List);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		// To eliminate depth buffer artifacts, use glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_STENCIL_TEST);
	}

	//**********************************************************************************************camera curve begin
	if(cameraMode) {
		gluLookAt(cameraX, cameraY, cameraZ, x, y, z, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(cameraX, cameraY, cameraZ);
	}
	else if (spectatorMode) {
		gluLookAt(spectatorX, spectatorY, spectatorZ,
			spectatorX + sin(spectatorAngle) + .001, spectatorY, spectatorZ - cos(spectatorAngle) + .001,
			0.0f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(spectatorX, spectatorY, spectatorZ);
	}
	else {
		gluLookAt(x - lx * 250, y - ly * 250, z - lz * 250, x, y, z, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(x, y, z);
	}

	//using fog here to depend on camera
	if (fog) {
		glEnable(GL_FOG);
	}
	else {
		glDisable(GL_FOG);
	}
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-150, -400, 0);
	glRotatef(-90, 0, 1, 0);
	glScalef(50, 50, 50);
	drawFlag();
	glPopMatrix();

	//current point on dynamic curve will be used as position for multiple objects
	Vec3f temp = (*objectVec)[objectPosition];
	
	if (materials)
		goldmat();

	// static rotation
	glPushMatrix();
	glTranslatef(0, 100, -800);
	glTranslatef(150 * cos(turning / 10), 0.0, 150 * sin(turning / 10));
	glScalef(diamondScale, diamondScale, diamondScale);
	objects[1]->render();
	objects[1]->position = Vec3f(150 * cos(turning / 10), 100, 150 * sin(turning / 10) - 800);
	//imageTextures? glCallList(woodBox) : glCallList(plainCube);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(temp.x - 1400, temp.y, temp.z - 1500);
	glScalef(diamondScale, diamondScale, diamondScale);
	objects[0]->position = Vec3f(temp.x - 1400, temp.y, temp.z - 1500);
	objects[0]->render();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1500 * cos(turning / 10) + 800, 500, 1500 * sin(turning / 10) - 800);
	glScalef(diamondScale, diamondScale, diamondScale);
	objects[2]->render();
	objects[2]->position = Vec3f(1500 * cos(turning / 10) + 800, 500, 1500 * sin(turning / 10) - 800);
	glPopMatrix();

	/*
	// stationary box 1
	glPushMatrix();
		glTranslatef(boxX, boxY, boxZ);
		imageTextures ? glCallList(metalBox) : glCallList(plainCube);
	
		// primary orbit around box 1
		glPushMatrix();
			glRotatef(turning, 0, 0, 1);
			glTranslatef(300, 0, 0);
			proceduralTextures ? glCallList(marbleBox) : glCallList(plainCube);

			// moon/mini box in secondary orbit
			glPushMatrix();
				glRotatef(turning * 3, 0, 1, 0);
				glTranslatef(100, 0, -800);
				glScalef(.2, .2, .2);
				proceduralTextures ? glCallList(marbleBox) : glCallList(plainCube);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	*/

	/*
	placeObstacle(-1500, 100, 8300, 1);
	placeObstacle(-2500, 100, 8300,1);
	placeObstacle(-3500, 100, 9300,1);  placeObstacle(-3500, 100, 8300, 2);  placeObstacle(-3500, 100, 7300, 3);  placeObstacle(-3500, 100, 6300, 2);
	placeObstacle(-4500, 100, 9300,1);  placeObstacle(-4500, 100, 8300, 2);  placeObstacle(-4500, 100, 7300, 3);  placeObstacle(-4500, 100, 6300, 2);
	placeObstacle(-5500, 100, 8300,1);
	*/

	
	// tree fractal
	GLfloat row = 18000;
	GLfloat column = 18000;
	if (fractals) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				generateTree(row, -700, column);
				column = column - 6000;
			}
			column = 18000;
			row = row - 6000;
		}
	}

	/*
	//draw start and ending areas
	glPushMatrix();
	glTranslatef(-750, -700, -900);
	glScalef(.75, 1, .75);
	if (materials)
		plasticmat();
	imageTextures ? glCallList(brickFloor) : glCallList(plainFloor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5000, -600, 8000);
	glScalef(.75, 1, .75);
	if (materials)
		chromemat();
	imageTextures ? glCallList(metalFloor) : glCallList(plainFloor);
	glPopMatrix();
	*/

	//draws airplane and its bounding box
	if (materials)
		chromemat();

	float convertedAngle = cameraAngle * -57.5 + 180;
	glPushMatrix();
	//glTranslatef(0, jetPosition, 0);
	glTranslatef(x, y, z);
	//glTranslatef(x + lx * 20, y + ly * 20 , z + lz * 20);
	glRotatef(convertedAngle, 0, 1, 0);
	glTranslatef(20, -31, 15);
	glRotatef(jetRotateY, 1, 0, 0);
	//glRotatef(jetRotateX, 0, 0, 1);
	glScalef(10, 10, 10);

	jet->position = Vec3f(x, y, z);
	jet->render();
	glPopMatrix();



	//draw fire particles if enabled

	if (particles) {
		jetFlame->add();
		jetFlame->update(calculate_frame_time());
		jetFlame->remove();


		defaultmat();
		glPushMatrix();
		glTranslatef(0, jetPosition, 0);
		glTranslatef(x + lx * 20, y + ly * 20, z + lz * 20);//translate based on plane's position
		glRotatef(cameraAngle * -57.5, 0, 1, 0);//make flames match plane's direction
		//glTranslatef(0, -02, 0);//y was -75
		//glRotatef(jetRotateY, 1, 0, 0);
		//glRotatef(jetRotateX, 0, 0, 1);
		glTranslatef(-7, 2, -40);//make flames originate in exhaust
		glRotatef(90, 1, 0, 0);//emit flames backward
		glScalef(.18, .18, .18);//make fire smaller
		jetFlame->drawParticles(fireCube);
		glPopMatrix();
	}


	// 3d surface
	if(materials)
		jademat();

	glPushMatrix();
	glTranslatef(-300, -1300, -3000);
	glScalef(40, 40, 40);
	drawMountain();
	glPopMatrix();

	defaultmat();

	//-9000, -8500, -9000
	// skybox
	glPushMatrix();
	glTranslatef(GLfloat(skyX), GLfloat(skyY), GLfloat(skyZ));
	glScalef(6.0, 6.0, 6.0);
	
	glDisable(GL_LIGHT0);
	GLfloat light_emissive2[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_emissive2);
	glCallList(skyBox);
	glPopMatrix();

	glEnable(GL_LIGHT0);
	defaultmat();

	//mountains
	glPushMatrix();
	
	glTranslatef(-12000, (multiscaleTerrain? 300 : -1000), -12000);
	glScalef(200, 1200, 200);
	if (materials) {
		emeraldmat();
	}

	if (multiscaleTerrain) {

		if (imageTextures) {
			glCallList(grassyTerrain);
		}
		else {
			glCallList(plainTerrain);
		}
	}

	else if (multiscaleTerrain == false) {
		if (imageTextures) {
			glCallList(grassyFloor);
		}
		else {
			glCallList(plainFloor);
		}
	}
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

	//game help text
	gluOrtho2D(0, window_width, 0, window_height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, window_height - 13.0f, 0.0f, "Use [Arrows] to move in plane", false);
	renderBitmapString(0.0, window_height - 26.0f, 0.0f, "Hold [left mouse] to look and change direction", false);
	renderBitmapString(0.0, window_height - 39.0f, 0.0f, "Use [W and S] to speed up or slow down", false);
	renderBitmapString(0.0, window_height - 52.0f, 0.0f, "Use [Spacebar] to ascend", false);
	renderBitmapString(0.0, window_height - 65.0f, 0.0f, "Use [Z] to descend", false);
	renderBitmapString(0.0, window_height - 78.0f, 0.0f, "Use [G] for spectator mode", false);

	//control viewport
	glViewport(0, 0, window_width, window_height * .2);

	string timeString = to_string(timer) + "s";

	/* for testing position
	string coords = to_string(x) + ", " + to_string(y) + ", "
		+ to_string(z) + ": " + to_string(inFinish);*/

	renderBitmapString(10, window_height * .65, 0.0f, "Time elapsed: ", true);
	renderBitmapString(180, window_height * .65, 0.0f, timeString.c_str(), true);

	//check to see if plane has touched any diamonds
	if (collision) {
		for (int ii = 0; ii < objects.size(); ii++) {
			GameObject* obj = objects[ii];
			if (jet->checkCollision(obj)) {
				obj->destroyed = true;
				remaining--;

				maxSpeed *= 1.15;
				minSpeed = (minSpeed + 5) * 1.2;

				if (speed > maxSpeed)
					speed = maxSpeed;
				else if (speed < minSpeed)
					speed = minSpeed;
			}
		}
	}
	won = remaining == 0;

	if (won) {
		cameraMode = true;
		glColor3f(.1, 1, .1);
		renderBitmapString(500, window_height * .65, 0.0f, "You Win", true);
	}
	else {
		string maxString = "Max Speed: " + to_string(maxSpeed);
		string currentString = "Your Speed: " + to_string(speed);
		string minString = "Minimum Speed: " + to_string(minSpeed);
		glColor3f(255, 0, 255);
		renderBitmapString(window_width - 400, window_height * .8, 0.0f, "Collect all 10 diamonds", true);
		renderBitmapString(window_width - 400, window_height * .7, 0.0f, maxString.c_str(), false);
		renderBitmapString(window_width - 400, window_height * .6, 0.0f, currentString.c_str(), false);
		renderBitmapString(window_width - 400, window_height * .5, 0.0f, minString.c_str(), false);

	}


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();

	if (!won) {
		int temp = std::clock();
		timer = (temp - start) / (double)CLOCKS_PER_SEC;
		particleTimer = (temp - start) / (double)CLOCKS_PER_SEC;
	}
}


// main
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Sky Box");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(myIdle);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);

	
	//glutSpecialFunc(specialkeys);
	glutSpecialFunc(mySpecial);
	glutKeyboardFunc(callbackKeyboard);
	init();
	glutMainLoop();
	return 0;
}