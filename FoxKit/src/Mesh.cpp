#include "headers\Mesh.h"

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

    rings = std::max(rings, 3);
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
            float z = cosf(latitudeAngle) * sinf(longitudeAngle);
            float y = sinf(latitudeAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vec3 normal = normalize(vec3(x,y,z));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
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
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                
            }
            if (i != (slices - 1)){
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
                indices.push_back(k2);
                
            }
        }
    }
}

Mesh::Mesh(PRIMITIVES primitive)
{
	switch (primitive) {
	case CUBE:
		vertexList = {
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,//0
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,//1
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,//2
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,//3
		
			 0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,//4
			-0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,//5
			-0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,//6
			 0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,//7
		
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,//8
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,//9
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,//10
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,//11
		
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,//12
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,//13
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,//14
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,//15
			
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//16
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//17
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//18
			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//19
			
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,//20
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,//21
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,//22
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,//23
		};
		indexList = {
			0,1,2,
			0,2,3,//bottom face
			4,6,5,
			4,7,6,//top face
			8,10,11,
			8,9,10,
			12,15,13,
			12,14,15,
			16,19,17,
			16,18,19,
			20,21,23,
			20,23,22,
		};
		break;
	case PYRAMID:
		vertexList = {
			 0.5f, -0.5f,  0.5f,//0
			-0.5f, -0.5f,  0.5f,//1
			-0.5f, -0.5f, -0.5f,//2
			 0.5f, -0.5f, -0.5f,//3
			 0.0f,  0.0f,  0.5f //4
		};
		indexList = {
			0,1,2,
			0,2,3,//bottom face
			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4
		};
		break;
	case SPHERE:
        generateUVSphere(vertexList, indexList, 64);
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
