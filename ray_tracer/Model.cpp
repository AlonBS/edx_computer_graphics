

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "Model.h"

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

Model::Model(string const &path, mat4& transform, mat3& invTransposeTrans)
:Object()
{
	this->transform() = transform;
	this->invTransposeTrans() = invTransposeTrans;
	loadModel(path);
}

Model::~Model()
{
	for (Mesh* m : meshes)
	{
		delete (m);
		m = nullptr;
	}

	for (Texture * t : loadedTextures) {
		delete(t->texture);
		delete(t);
		t = nullptr;
	}

	meshes.clear();
}


bool
Model::intersectsRay(Ray &r, GLfloat &dist, vec3* point, vec3* normal, ObjectTexColors* texColors, ObjectProperties* properties)
{
	GLfloat minDist = INFINITE;

	GLfloat tDist;
	vec3 tP, tN;
	vec2 ttC;
	MeshProperties meshProps;

	for (Mesh *m : meshes) {

		if (m->intersectsRay(r, tDist, &tP, &tN, &ttC, &meshProps)) {

			if (tDist < minDist) {

				dist = minDist = tDist;
				if (point)  *point = tP;
				if (normal) *normal = tN;

				if (texColors) {
					texColors->_ambientTexColor  = m->getAmbientTextureColor(ttC)  * this->getAmbientTextureColor(ttC);
					texColors->_diffuseTexColor  = m->getDiffuseTextureColor(ttC)  * this->getDiffuseTextureColor(ttC);
					texColors->_specularTexColor = m->getSpecularTextureColor(ttC) * this->getSpecularTextureColor(ttC);
				}

				if (properties) *properties = meshProps * _properties;
			}
		}
	}

	if (minDist == INFINITE) {
		return false;
	}

	return true;

}



void
Model::loadModel(string const &path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	// check for errors
	if(!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void
Model::processNode(aiNode *node, const aiScene *scene)
{
	// process each mesh located at the current node
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene.
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh*
Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	// data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	MeshProperties properties = {};
	Image *ambientTexture, *diffuseTexture, *specularTexture;
	ambientTexture = diffuseTexture = specularTexture = nullptr;

	properties._ambient = vec3(1.0f, 1.0f, 1.0f);

	// Walk through each of the mesh's vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vec3 (_transform * vec4(vector, 1.0f));;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		//vertex.Normal = _invTransposeTrans * vector;
		vertex.Normal = vector;
		// texture coordinates
		if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process materials
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D c (0.0f, 0.0f, 0.0f);

		material->Get(AI_MATKEY_COLOR_AMBIENT, c);
		properties._ambient = vec3(c.r, c.g, c.b);
		if (epsilonCompareVec3(properties._ambient, COLOR_BLACK))
			properties._ambient = COLOR_WHITE;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, c);
		properties._diffuse = vec3(c.r, c.g, c.b);
		if (epsilonCompareVec3(properties._diffuse, COLOR_BLACK))
			properties._diffuse = COLOR_WHITE;
		material->Get(AI_MATKEY_COLOR_SPECULAR, c);
		properties._specular = vec3(c.r, c.g, c.b);
		if (epsilonCompareVec3(properties._specular, COLOR_BLACK))
			properties._specular = COLOR_WHITE;
		material->Get(AI_MATKEY_SHININESS, properties._shininess);
		if (properties._shininess == 0.0f)
			properties._shininess = 2;


		// Load textures
		// Currently - we only support one ambient, one diffuse, and one specular texture
		// 1. Ambient maps
		ambientTexture = this->loadMaterialTextures(material, aiTextureType_AMBIENT);
		// 2. Diffuse maps
		diffuseTexture = this->loadMaterialTextures(material, aiTextureType_DIFFUSE);
		// 3. Specular maps
		specularTexture = this->loadMaterialTextures(material, aiTextureType_SPECULAR);
	}


	// return a mesh object created from the extracted mesh data
	//return Mesh(vertices, indices/*, textures*/);
	return new Mesh(vertices, indices, properties, ambientTexture, diffuseTexture, specularTexture);
}



Image*
Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	if (mat->GetTextureCount(type) == 0) {
		return nullptr;
	}

	Image *texture = nullptr;
	aiString str;

	mat->GetTexture(type, 0, &str); // Currenly - we support only 1 texture of each type.

	// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
	GLboolean skip = false;
	for(GLuint j = 0; j < loadedTextures.size(); ++j)
	{
		if(loadedTextures[j]->name.compare(str.C_Str()) == 0)
		{
			texture = loadedTextures[j]->texture;
			skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
			break;
		}
	}
	if(!skip)
	{   // If texture hasn't been loaded already, load it
		Texture *loadedTexture = new Texture();
		loadedTexture->name = string(str.C_Str());
		loadedTexture->texture = new Image(0, 0);
		string path = directory + "/" + loadedTexture->name;
		loadedTexture->texture->loadImage(path);

		texture = loadedTexture->texture;
		this->loadedTextures.push_back(loadedTexture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	}

	return texture;
}


