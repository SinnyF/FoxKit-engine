#pragma once
#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <map>
#include <string>

enum PRIMITIVES
{
    CUBE, PYRAMID, SPHERE
};

using namespace std;
using namespace glm;

class Mesh
{
public:
    Mesh(PRIMITIVES primitive);
    static Mesh* LoadMesh(const string& path);
    static unsigned int LoadTexture(const string& path);
    static void CleanUP();
    Mesh();
    unsigned int VAO = 0, indicies = 0;

private:
    Mesh(string path);
    static map<string, Mesh> loadedMeshes;
    void setBuffers(const vector<float>& vertexList, const vector<unsigned int>& indexList);
};
