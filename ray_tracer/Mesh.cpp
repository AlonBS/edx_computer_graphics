

#include "Mesh.h"


ObjectProperties operator*(const MeshProperties& mp, const ObjectProperties& op)
{
	ObjectProperties ret =
	{
			._ambient   = mp._ambient   * op._ambient,
			._emission  = mp._emission  * op._emission,
			._diffuse   = mp._diffuse   * op._diffuse,
			._specular  = mp._specular  * op._specular,
			._shininess = mp._shininess * op._shininess
	};
	return ret;
}

ObjectProperties operator*(const ObjectProperties& op, const MeshProperties& mp)
{
	return mp*op;
}





    /*  Functions  */
    // constructor
Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, MeshProperties& properties /*, vector<Texture> textures*/)
:_properties(properties)
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
Mesh::intersectsRay(Ray &r, GLfloat &dist, vec3* point, vec3* normal, vec3* texColor, MeshProperties* properties)
{
	GLfloat minDist = INFINITE;

	GLfloat tDist;
	vec3 tP, tN, ttC;

	for (Triangle *t : triangles) {

		/* When we iterate over triangles as part of mesh - we take the properties of the mesh
		 * and not the triangle. In fact, this triangle doesn't have other but default properties
		 */
		if (t->intersectsRay(r, tDist, &tP, &tN, &ttC, nullptr)) {

			if (tDist < minDist) {

				dist = minDist = tDist;
				*point = tP;
				*normal = tN;
				*texColor = ttC;
				*properties = this->_properties;
			}
		}
	}

	if (minDist == INFINITE) {
		return false;
	}

	return true;
}

