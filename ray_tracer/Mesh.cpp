

#include "Mesh.h"
    /*  Functions  */
    // constructor
Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices/*, vector<Texture> textures*/)
{
	__triangulate(vertices, indices);
}


Mesh::~Mesh()
{
	for (Triangle *t : triangles) {
		delete(t);
		t = nullptr;
	}

	triangles.clear();
}



void
Mesh::__triangulate(vector<Vertex> vertices, vector<unsigned int> indices)
{
	vec3 A, B, C;
	vec3 An, Bn, Cn;


	for (unsigned int i = 0 ; i < indices.size() ; i+=3) {

		A  = vertices[indices[i  ]].Position;
		An = vertices[indices[i  ]].Normal;

		B  = vertices[indices[i+1]].Position;
		Bn = vertices[indices[i+1]].Normal;

		C  = vertices[indices[i+2]].Position;
		Cn = vertices[indices[i+2]].Normal;

		this->triangles.push_back(new Triangle(A, B, C, An, Bn, Cn));

	}

}


bool
Mesh::intersectsRay(Ray &r, GLfloat &dist, vec3* point, vec3* normal, vec3* texColor)
{
	GLfloat minDist = INFINITE;

	GLfloat tDist;
	vec3 tP, tN, ttC;

	for (Triangle *t : triangles) {

		if (t->intersectsRay(r, tDist, &tP, &tN, &ttC)) {

			if (tDist < minDist) {

				dist = minDist = tDist;
				*point = tP;
				*normal = tN;
				*texColor = ttC;
			}
		}
	}

	if (minDist == INFINITE) {
		return false;
	}

	return true;
}

