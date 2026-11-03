#include "headers\Mesh.h"

Mesh::Mesh(PRIMITIVES primitive)
{
	switch (primitive) {
	case CUBE:
		vertexList = {
			 0.5f,  0.5f, -0.5f,//0
			-0.5f,  0.5f, -0.5f,//1
			-0.5f, -0.5f, -0.5f,//2
			 0.5f, -0.5f, -0.5f,//3
			 0.5f,  0.5f,  0.5f,//4
			-0.5f,  0.5f,  0.5f,//5
			-0.5f, -0.5f,  0.5f,//6
			 0.5f, -0.5f,  0.5f,//7
		};
		indexList = {
			0,1,2,
			0,2,3,//bottom face
			4,6,5,
			4,7,6,//top face
			4,3,7,
			4,0,3,//+x
			5,6,2,
			5,2,1,//-x
			7,2,6,
			7,3,2,//-y
			4,5,1,
			4,1,0 //+y
		};
		break;
	case PYRAMID:
		break;
	case SPHERE:

	}
}

vector<float>* Mesh::Vertices()
{
	return nullptr;
}

vector<float>* Mesh::Indices()
{
	return nullptr;
}
