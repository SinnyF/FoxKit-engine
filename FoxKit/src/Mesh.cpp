#include "headers\Mesh.h"
#include <math.h>
#include <iostream>
#include <vector>

void generateSortofSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, int rings, int slices) {

    if (rings < 3) { std::cout << "no sphere generated. rings value < 3" << std::endl;  return; }
    if (slices < 4) { std::cout << "no sphere generated. slices value < 4" << std::endl;  return; }

    float PIE = 3.1415f;

    vertices.clear();
    indices.clear();

    float angle_step = 360.0f / slices;
    float current_angle = 0.0f;

    // Vertices
    float y_step = 2.0f / (rings - 1);
    float y_pos = 1.0f;

    for (int ring = 0; ring < rings; ring++) {

        int iteration_slices = slices;
        if (ring == 0 || ring == rings - 1) { iteration_slices = 1; }

        for (int i = 0; i < iteration_slices; i++) {

            float radius = sinf(acos(y_pos));

            float x_pos = cosf(current_angle * PIE / 180) * radius;
            float z_pos = sinf(current_angle * PIE / 180) * radius;

            vertices.push_back(x_pos);
            vertices.push_back(z_pos);
            vertices.push_back(y_pos);

            current_angle += angle_step;
        }

        y_pos -= y_step;
        current_angle = 0.0f;
    }

    std::cout << "Sphere vertex count:" << vertices.size() / 3 << std::endl;

    //Indices
    unsigned int top_most_index = 0;
    unsigned int bottom_most_index = vertices.size() / 3 - 1;

    for (int i = 0; i < rings - 1; i++) {

        for (int j = 0; j < slices; j++) {

            unsigned int tl_index = (i - 1) * slices + j + 1;
            unsigned int tr_index = (i - 1) * slices + j + 1 + 1;
            unsigned int bl_index = (i + 0) * slices + j + 1;
            unsigned int br_index = (i + 0) * slices + j + 1 + 1;

            if (j == slices - 1) { // For the seam
                tr_index = (i - 2) * slices + j + 1 + 1;
                br_index = (i - 1) * slices + j + 1 + 1;
            }

            if (i == 0) { // First ring

                indices.push_back(top_most_index);
                indices.push_back(br_index);
                indices.push_back(bl_index);

            }
            else if (i == rings - 2) { // Last ring

                indices.push_back(tr_index);
                indices.push_back(bottom_most_index);
                indices.push_back(tl_index);

            }
            else {

                indices.push_back(tl_index);
                indices.push_back(br_index);
                indices.push_back(bl_index);

                indices.push_back(tl_index);
                indices.push_back(tr_index);
                indices.push_back(br_index);
            }
        }
    }

    std::cout << "Sphere triangle count:" << indices.size() / 3 << std::endl;
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
		vertexList = {
			 0.5f,  0.5f, -0.5f,//0
			-0.5f,  0.5f, -0.5f,//1
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
		generateSortofSphere(vertexList, indexList, 24, 24);
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
