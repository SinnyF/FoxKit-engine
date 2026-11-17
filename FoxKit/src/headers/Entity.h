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
	vec3 getPosition() const;
	vec3 getRotation() const;
	vec3 getScale() const;
	mat4 modelMatrix;
	unsigned int textureID = 0, shaderID = 0, normalID = 0;
	Entity(const string &_modelPath, unsigned int _shaderID = 0, const string &_texturePath = "", const string &_normalPath = "");
	void setTexture(const string &texturePath, const string &normalPath = "");
	Entity();
	string modelPath;
	string texturePath;
	string normalPath;
	vector<tuple<string,float>> uniformsf;
	vector<Entity*> GetChildren() const;
	Entity* GetParent() const;
	void setParent(Entity *_parent);
	vector<Entity*> children;
protected:
	vec3 position = vec3(0);
	vec3 rotation = vec3(0);
	vec3 scale = vec3(1.0,1.0,1.0);
	void Translate();
	Entity *parent = nullptr;
};

