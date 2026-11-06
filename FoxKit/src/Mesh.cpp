#include "headers\Mesh.h"
#include <math.h>
#include <iostream>
#include <vector>



void generateIcoSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices) {

    vertices.clear();
    indices.clear();

    vertices = {
        0.8506508f,0.5257311f,0.0f,
        0.000000101405476f, 0.8506507f, -0.525731f,
        0.000000101405476f, 0.8506506f, 0.525731f,
        0.5257309f, -0.00000006267203f, -0.85065067f,
        0.52573115f, -0.00000006267203f, 0.85065067f,
        0.8506508f, -0.5257311f, 0.0f,
        - 0.52573115f, 0.00000006267203f, -0.85065067f,
        - 0.8506508f, 0.5257311f, 0.0f,
        - 0.5257309f, 0.00000006267203f, 0.85065067f,
        - 0.000000101405476f, -0.8506506f, -0.525731f,
        - 0.000000101405476f, -0.8506507f, 0.525731f,
        - 0.8506508f, -0.5257311f, 0.0f
    };

    indices = {
        0,  1,  2,
        0,  3,  1,
        0,  2,  4,
        3,  0,  5,
        0,  4,  5,
        1,  3,  6,
        1,  7,  2,
        7,  1,  6,
        4,  2,  8,
        7,  8,  2,
        9,  3,  5,
        6,  3,  9,
        5,  4, 10,
        4,  8, 10,
        9,  5, 10,
        7,  6, 11,
        7, 11,  8,
        11,  6,  9,
        8, 11, 10,
        10, 11,  9
    };
    
}

void generateUVSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, int rings) {

    if (rings < 3)
        rings = 3;
    int slices = rings / 2;

    float PI = 3.14159f;

    float deltaLatitude = PI / slices;
    float deltaLongitude = 2 * PI / rings;

    for (int i = 0; i <= slices; ++i)
    {
        float latitudeAngle = PI / 2 - i * deltaLatitude;
        for (int j = 0; j <= rings; ++j)
        {
            float longitudeAngle = j * deltaLongitude;
            float x = cosf(latitudeAngle) * cosf(longitudeAngle);
            float y = cosf(latitudeAngle) * sinf(longitudeAngle);
            float z = sinf(latitudeAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    unsigned int k1, k2;
    for (int i = 0; i < slices; ++i)
    {
        k1 = i * (rings + 1);
        k2 = k1 + rings + 1;
        for (int j = 0; j < rings; ++j, ++k1, ++k2)
        {
            if (i != 0){
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (slices - 1)){
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

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
			0,2,1,
			0,3,2,//bottom face
			4,5,6,
			4,6,7,//top face
			4,7,3,
			4,3,0,//+x
			5,2,6,
			5,1,2,//-x
			7,6,2,
			7,2,3,//-y
			4,1,5,
			4,0,1 //+y
		};
		break;
	case PYRAMID:
		vertexList = {
			 0.5f,  0.5f, -0.5f,//0
			-0.5f,  0.5f, -0.5f,//1
			-0.5f, -0.5f, -0.5f,//2
			 0.5f, -0.5f, -0.5f,//3
			 0.0f,  0.0f,  0.5f //4
		};
		indexList = {
			0,2,1,
			0,3,2,//bottom face
			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4
		};
		break;
	case SPHERE:
        generateUVSphere(vertexList, indexList, 24);
        //generateIcoSphere(vertexList, indexList);
		break;
	}
}

vector<float> Mesh::Vertices()
{
	return vertexList;
}

vector<unsigned int> Mesh::Indices()
{
	return indexList;
}
