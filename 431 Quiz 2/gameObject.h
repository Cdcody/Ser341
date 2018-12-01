#include "collision.h"
#include "mesh.h"

class GameObject {
public:
	GLUquadric* quad;
	Mesh* mesh;
	GLuint displayList;
	Vec3f position;
	float scale;//scale multiplier
	boolean colliding = false;

	GameObject(Mesh* mesh, float scale) {
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
		glPushMatrix();
		glCallList(displayList);

		if (bounding) {
			glTranslatef(modelCenter.x, modelCenter.y, modelCenter.z);
			glDisable(GL_LIGHTING);
			if (colliding) {
				glColor3f(1, 0, 0);
			}
			gluSphere(quad, radius, 15, 15);
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	}

	//checks if 2 bounding spheres intersect
	boolean checkCollision(GameObject* o) {
		float distX = (this->modelCenter.x + this->position.x) - (o->modelCenter.x + o->position.x);
		float distY = (this->modelCenter.y + this->position.y) - (o->modelCenter.y + o->position.y);
		float distZ = (this->modelCenter.z + this->position.z) - (o->modelCenter.z + o->position.z);
		float distance = sqrt(distX * distX + distY * distY + distZ + distZ);
		return distance < (this->radius * this->scale + o->radius * o->scale);
	}

private:
	Vec3f modelCenter;//center position of mesh
	float radius;//maximum radius of mesh
};