#include "collision.h"
#include "mesh.h"

class GameObject {
public:
	GLUquadric* quad;
	Mesh* mesh;
	GLuint displayList;
	Vec3f centerPoint;//center position of mesh
	float radius;//maximum radius of mesh
	float scale;//scale multiplier

	GameObject(Mesh* mesh, float scale) {
		this->mesh = mesh;
		centerPoint = findCenter(mesh);
		radius = findRadius(mesh, centerPoint);
		this->scale = scale;
		initSphere();
	}

	~GameObject() {
		gluDeleteQuadric(quad);
	}

	 void initSphere() {
		quad = gluNewQuadric();
		gluQuadricDrawStyle(quad, GLU_LINE);
	}

	void render() {
		glPushMatrix();
		glCallList(displayList);

		if (bounding) {
			glTranslatef(centerPoint.x, centerPoint.y, centerPoint.z);
			gluSphere(quad, radius, 15, 15);
		}
		glPopMatrix();
	}

	//checks if 2 bounding spheres intersect
	boolean checkCollision(GameObject* o) {
		float distance = sqrt((this->centerPoint.x - o->centerPoint.x) * (this->centerPoint.x - o->centerPoint.x) +
			(this->centerPoint.y - o->centerPoint.y) * (this->centerPoint.y - o->centerPoint.y) +
			(this->centerPoint.z - o->centerPoint.z) * (this->centerPoint.z - o->centerPoint.z));
		return distance < (this->radius * this->scale + o->radius * o->scale);
	}
};