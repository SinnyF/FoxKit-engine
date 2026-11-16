#pragma once
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

using namespace glm;
using namespace std;

class Entity
{
public:
	void Move(vec3 move);
	void Rotate(vec3 rotate);
	void Scale(vec3 _scale);
	mat4 modelMatrix;
	unsigned int textureID, shaderID;
	Entity(const string &_modelPath, const string &_texturePath, unsigned int _shaderID = 0);
	string modelPath;
	string texturePath;
	vector<tuple<string,float>> uniformsf;
protected:
	vec3 position = vec3(0);
	vec3 rotation = vec3(0);
	vec3 scale = vec3(1);
	void Translate();
};

