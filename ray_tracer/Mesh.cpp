

#include "Mesh.h"
    /*  Functions  */
    // constructor
Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices/*, vector<Texture> textures*/)
{
	cout << "SIZE OF VER " << vertices.size() << endl;
	cout << "SIZE OF INDEX " << indices.size() << endl;
	for (Vertex v : vertices) {
		printVec3("Position", v.Position);
	}

	for (int i : indices) {
		cout << "I IS : " << i << endl;
	}




	for (int i = 0 ; i < indices.size() ; i+=3)  {

		printVec3("A: ", vertices[indices[i]].Position);
		printVec3("B: ", vertices[indices[i+1]].Position);
		printVec3("C: ", vertices[indices[i+2]].Position);



	}


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


	for (unsigned int i = 0 ; i < indices.size() ; ++i) {

		A = vertices[indices[i  ]].Position;
		B = vertices[indices[i+1]].Position;
		C = vertices[indices[i+2]].Position;

		An = vertices[indices[i  ]].Normal;
		Bn = vertices[indices[i  ]].Normal;
		Cn = vertices[indices[i  ]].Normal;

		this->triangles.push_back(new Triangle(A, B, C, An, Bn, Cn));
	}
}


bool
Mesh::intersectsRay(Ray &r, GLfloat &dist, vec3* point, vec3* normal, vec3* texColor)
{
	GLfloat minDist = INFINITE;

	for (Triangle *t : triangles) {

		if (t->intersectsRay(r, dist, point, normal, texColor)) {

			if (dist < minDist) {

				minDist = dist;
			}
		}
	}

	if (minDist == INFINITE) {
		return false;
	}

	return true;
}

