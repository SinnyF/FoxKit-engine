#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <map>
#include <string>
static enum PRIMITIVES
{
	CUBE, PYRAMID, SPHERE
};

using namespace std;
using namespace glm;
class Mesh
{
public:
	Mesh(PRIMITIVES primitive);
	static Mesh* LoadMesh(string path);
	static unsigned int LoadTexture(string path);
	vector<float> Vertices();
	vector<unsigned int> Indices();
	static void CleanUP();
	Mesh();
	unsigned int VAO;
private:
	
	Mesh(string path, PRIMITIVES primitive = CUBE);
	vector<float> vertexList;
	vector<unsigned int> indexList;
	static map<string, Mesh> loadedMeshes;
	void setBuffers();
};

