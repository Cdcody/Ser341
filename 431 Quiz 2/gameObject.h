#include "collision.h"
#include "mesh.h"

class GameObject {
public:
	Mesh* mesh;
	GLuint displayList;
	float* bounds;
	float matrix[16];

	GameObject(Mesh* mesh) {
		this->mesh = mesh;
		calculateNormalPerFace(mesh);
		calculateNormalPerVertex(mesh);
		bounds = findBounds(mesh);
	}

	void render() {
		glPushMatrix();
		glMultMatrixf(matrix);
		glCallList(displayList);
		glPopMatrix();
	}

	boolean collision(GameObject* o) {
		
		return false;
	}
};