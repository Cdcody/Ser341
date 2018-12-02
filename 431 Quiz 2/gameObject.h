#include "collision.h"
#include "mesh.h"

class GameObject {
public:
	GLUquadric* quad;
	Mesh* mesh;
	GLuint displayList;
	Vec3f position;
	float scale;//scale multiplier
	boolean destroyed = false;

	GameObject(Mesh* mesh, float scale, GLuint display) {
		displayList = display;
		this->mesh = mesh;
		modelCenter = findCenter(mesh);
		radius = findRadius(mesh, modelCenter);
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
		if (destroyed)
			return;

		glPushMatrix();
		glCallList(displayList);

		if (bounding) {
			glTranslatef(modelCenter.x, modelCenter.y, modelCenter.z);
			glDisable(GL_LIGHTING);
			gluSphere(quad, radius, 15, 15);
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	}

	//checks if 2 bounding spheres intersect
	boolean checkCollision(GameObject* o) {
		if (destroyed || o->destroyed)
			return false;

		//using long doubles to avoid overflow
		long double distX = (this->modelCenter.x + this->position.x) - (o->modelCenter.x + o->position.x);
		long double distY = (this->modelCenter.y + this->position.y) - (o->modelCenter.y + o->position.y);
		long double distZ = (this->modelCenter.z + this->position.z) - (o->modelCenter.z + o->position.z);
		long double distance = sqrt(distX * distX + distY * distY + distZ * distZ);
		if (distance < (this->radius * this->scale + o->radius * o->scale))
			return true;
		return false;
	}

private:
	Vec3f modelCenter;//center position of mesh
	float radius;//maximum radius of mesh
};