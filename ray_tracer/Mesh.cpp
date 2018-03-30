

#include "Mesh.h"
    /*  Functions  */
    // constructor
Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices/*, vector<Texture> textures*/)
{
	this->triangles = __triangulate(vertices, indices);
//	this->indices = indices;
//	this->textures = textures;

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
//	setupMesh();
}

vector<Triangle*>
Mesh::__triangulate(vector<Vertex> vertices, vector<unsigned int> indices)
{
	vector<Triangle*> v;
	return v;
}

