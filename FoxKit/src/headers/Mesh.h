#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Mesh.h"
#include <math.h>
#include <iostream>

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
	vector<float> Vertices();
	vector<unsigned int> Indices();
	vec3 position = vec3(0,0,0);
	unsigned int VAO;
	int shaderID;
private:
	vector<float> vertexList;
	vector<unsigned int> indexList;
	vector<Mesh> meshes;
};

