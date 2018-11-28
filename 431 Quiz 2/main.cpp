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

#define terrainSamples 100 


 // global
Mesh *floorPlane, *floorPlane2, *cubeMesh, *skybox, *terrainMesh, *f16;
GLuint brickFloor, metalFloor, metalBox, woodBox, marbleBox, skyBox, grassyTerrain, f16List, fireCube,
plainFloor, plainCube, plainTerrain, grassyFloor;
GLUnurbsObj *theNurb;
GLuint textures[10];

ParticleSystem* jetFlame = new ParticleSystem();

//timer info
std::clock_t start;
int timer = 0;
float particleTimer = 0;

//bool won;

int staticAngle, angle1, angle2;


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




//object path parameters
GLfloat objectPathControlPoints[4][3] = {
	{ 400, 0,  -800},
	{ 40, 0, 0 },
	{ 800,  0, 0 },
	{ 300, -400, -800 },
};

// init
void init() {
	init_frame_timer();
	createMenus();

	cameraVec = hermiteCurve(cameraPathControlPoints, 1000);
	objectVec = hermiteCurve(objectPathControlPoints, 1000);
	boxVec = hermiteCurve(boxPathControlPoints, 1000);

	window_ratio = window_height / window_width;
	// mesh
	floorPlane = createPlane(2000, 2000, 150);
	floorPlane2 = createPlane(2000, 2000, 10);
	cubeMesh = createCube();
	skybox = createSkyBox(6000);

	f16 = loadFile("_OBJ_files/f-16.obj");

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
	loadBMP_custom(textures, "_BMP_files/grass.bmp", 6);
	codedTexture(textures, 7, 2);
	


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

	
	glEnable(GL_DEPTH_TEST);

	// light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };
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

	start = std::clock();
}

void reshape(int w, int h) {
	window_width = w;
	window_height = h;
	if (h == 0) h = 1;
	window_ratio = 1.0f * w / h;
}

void drawNurb() {
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

// display
void display(void) {
	glDisable(GL_FOG);
	orientMe(cameraAngle);

	//resets clock when in starting area
	bool inStart = (x < 50 && x > -50) && (z < 50 && z > -50);

	if (inStart) {
		start = std::clock();
	}
	bool inFinish = (z < 9500 && z > 8000) && (x < -3500 && x > -5000);
	if (inFinish) {
		won = true;
	}

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

	if (fog) {

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

	//**********************************************************************************************camera curve begin
	if (!cameraMode) {
		gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(x, y, z);
	}
	else {
		gluLookAt(cameraX, cameraY, cameraZ, x, y, z, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
		glTranslatef(cameraX, cameraY, cameraZ);
	}
	//using fog here to depend on camera o
	if (fog) {
		glEnable(GL_FOG);
	}
	else {
		glDisable(GL_FOG);
	}
	glPopMatrix();

	// static rotation
	glPushMatrix();
	glTranslatef(0, 100, -800);
	glTranslatef(150 * cos(turning / 10), 0.0, 150 * sin(turning / 10));
	imageTextures? glCallList(woodBox) : glCallList(plainCube);
	glPopMatrix();

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
				glTranslatef(100, 0, 0);
				glScalef(.2, .2, .2);
				proceduralTextures ? glCallList(marbleBox) : glCallList(plainCube);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();


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


	//draws airplane and its bounding box
	if (materials)
		chromemat();

	glPushMatrix();
	glTranslatef(x + lx * 20, y + ly * 20 , z + lz * 20);
	glRotatef(cameraAngle * -57.5 + 180, 0, 1, 0);
	glTranslatef(16, -62, 120);//y was -75
	glScalef(10, 10, 10);
	glCallList(f16List);

	if (bounding) {
		AABB(f16);
	}
	glPopMatrix();

	//draw fire particles if enabled

	if (particles) {
		jetFlame->add();
		jetFlame->update(calculate_frame_time());
		jetFlame->remove();


		glPushMatrix();
		glTranslatef(x + lx * 20, y + ly * 20, z + lz * 20);
		glRotatef(cameraAngle * -57.5, 0, 1, 0);
		//glTranslatef(16, -75, 120);
		glTranslatef(-2, -25, -70);//y was -40
		glRotatef(90, 1, 0, 0);
		glScalef(.1, .1, .1);
		jetFlame->drawParticles(fireCube);
		glPopMatrix();
	}

	//object moving on curve


	glPushMatrix();
	Vec3f temp = (*objectVec)[objectPosition];
	glTranslatef(temp.x - 1400, temp.y, temp.z - 1500);
	glScalef(2, 2, 2);
	imageTextures ? glCallList(woodBox) : glCallList(plainCube);
	glPopMatrix();

	// 3d surface
	if(materials)
		jademat();

	glPushMatrix();
	glTranslatef(-300, -1300, -3000);
	glScalef(40, 40, 40);
	drawNurb();
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
	glScalef(300, 1200, 300);
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
	renderBitmapString(0.0, window_height - 52.0f, 0.0f, "Use [Spacebar] to stop", false);

	//control viewport
	glViewport(0, 0, window_width, window_height * .2);

	string timeString = to_string(timer) + "s";

	/* for testing position
	string coords = to_string(x) + ", " + to_string(y) + ", "
		+ to_string(z) + ": " + to_string(inFinish);*/

	renderBitmapString(10, window_height * .65, 0.0f, "Time elapsed: ", true);
	renderBitmapString(180, window_height * .65, 0.0f, timeString.c_str(), true);

	
	if (won) {
		cameraMode = true;
		glColor3f(.1, 1, .1);
		renderBitmapString(500, window_height * .65, 0.0f, "You Win", true);
	}
	else if (!inStart && !won) {
		glColor3f(255, 0, 255);
		renderBitmapString(window_width - 400, window_height * .65, 0.0f, "Reach the metal surface", true);
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