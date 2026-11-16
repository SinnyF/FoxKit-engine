#define TINYOBJLOADER_IMPLEMENTATION
#include "headers/tiny_obj_loader.h"
#include "headers/Mesh.h"
#include <tuple>
#include <chrono>
#include "headers/CustomReader.h"
#include "headers/stb_image.h"

map<string, Mesh> Mesh::loadedMeshes = map<string, Mesh>();

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
        	vertices.push_back((float)j/rings);
        	vertices.push_back((float)i/slices);
        	
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
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0, 1.0,//0
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0, 1.0,//1
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0, 0.0,//2
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0, 0.0,//3
		
			 0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0, 1.0,//4
			-0.5f,  0.5f,  0.5f, 0.0f,  1.0f, 0.0f,0.0, 1.0,//5
			-0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,0.0, 0.0,//6
			 0.5f,  0.5f, -0.5f, 0.0f,  1.0f, 0.0f,1.0, 0.0,//7
		
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,0.0, 1.0,//8
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,0.0, 0.0,//9
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,1.0, 0.0,//10
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,1.0, 1.0,//11
		
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,0.0, 1.0,//12
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,0.0, 0.0,//13
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,1.0, 1.0,//14
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,1.0, 0.0,//15
			
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,1.0, 0.0,//16
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0, 0.0,//17
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,1.0, 1.0,//18
			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,0.0, 1.0,//19
			
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0, 0.0,//20
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0, 0.0,//21
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,1.0, 1.0,//22
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,0.0, 1.0,//23
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

Mesh* Mesh::LoadMesh(string path)
{
	if (loadedMeshes.find(path) == loadedMeshes.end())
	{
		loadedMeshes[path] = Mesh(path);
		loadedMeshes[path].setBuffers();
	}
	return &loadedMeshes[path];
}

unsigned int Mesh::LoadTexture(string path)
{
	int width, height, channels; 
	unsigned int texture;
	stbi_set_flip_vertically_on_load(true);
	auto imgData = stbi_load(path.c_str(),&width ,&height,&channels ,4);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);
	return texture;
}

Mesh::Mesh()
{
	
}

Mesh::Mesh(string path, PRIMITIVES primitive)
{
	//Switch to 3rd Party loader
	if(primitive == CUBE)
	{
		cout << "Loading model: " << path << endl;
		auto start = chrono::high_resolution_clock::now();
	    tinyobj::ObjReaderConfig readerConfig;
	    readerConfig.mtl_search_path = "./";
	    readerConfig.triangulate = true;
	    tinyobj::ObjReader reader;

	    if (!reader.ParseFromFile(path, readerConfig)) {
	        if (!reader.Error().empty()) {
	            std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
	        }
	        exit(1);
	    }

	    if (!reader.Warning().empty()) {
	        std::cout << "TinyObjReader: " << reader.Warning() << std::endl;
	    }

	    auto& attrib = reader.GetAttrib();
	    auto& shapes = reader.GetShapes();
		//update to unordered_map for faster lookup?
	    std::map<std::tuple<int, int, int>, unsigned int> uniqueVertices;
		unsigned int newIndex = 0;
		int dupecount = 0, vertexcount = 0;
	    for (size_t s = 0; s < shapes.size(); s++) {
	        size_t index_offset = 0;
	        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
	            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
        		
	            for (size_t v = 0; v < fv; v++) {
	                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
	                auto key = std::make_tuple(idx.vertex_index, idx.normal_index, idx.texcoord_index);

	                //Check if this index combination already exists, if we get the default return we didn't get a match
            		//and need to create new vertex attributes
	                if (uniqueVertices.find(key) == uniqueVertices.end()) {
	                    uniqueVertices[key] = newIndex;
                		newIndex++;
	                    //Vertex
	                    vertexList.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
	                    vertexList.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
	                    vertexList.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);

	                    //Normals default to 0 if not defined in .obj
	                    if (idx.normal_index >= 0) {
	                        vertexList.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
	                        vertexList.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
	                        vertexList.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);
	                    } else {
	                        vertexList.push_back(0.0f);
	                        vertexList.push_back(0.0f);
	                        vertexList.push_back(0.0f);
	                    }

	                    //UV also set to 0 if missing
	                    if (idx.texcoord_index >= 0) {
	                        vertexList.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
	                        vertexList.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
	                    } else {
	                        vertexList.push_back(0.0f);
	                        vertexList.push_back(0.0f);
	                    }
	                }
	            	else
	            	{
	            		dupecount++;
	            	}
	            	vertexcount++;
	                //Push the index
	                indexList.push_back(uniqueVertices[key]);
	            }
	            index_offset += fv;
	        }
	    }
		auto stop = chrono::high_resolution_clock::now();
		
		auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
		cout << "tinyobj loader done" << endl;
		cout << "Time taken: " << duration.count() << " microseconds" << endl;
		cout <<"dupe count: " << dupecount << endl;
		cout <<"vertex count: " << vertexcount << endl;
	}
	//Custom .obj loader
	else
	{
		cout << "Loading model: " << path << endl;
		auto start = chrono::high_resolution_clock::now();
	    
		vector<CustomReader::Indexes> data;
		vector<float> vertices;
		vector<float> normals;
		vector<float> uvs;
		CustomReader::ReadFile(data, path, vertices, normals, uvs);

		//update to unordered_map for faster lookup?
	    std::map<std::tuple<int, int, int>, unsigned int> uniqueVertices;
		unsigned int newIndex = 0;
		int dupecount = 0, vertexcount = 0;
        		
        for (size_t v = 0; v < data.size(); v++) {
            auto key = std::make_tuple(data[v].position, data[v].normal, data[v].uv);

            //Check if this index combination already exists, if we get the default return we didn't get a match
            //and need to create new vertex attributes
            if (uniqueVertices.find(key) == uniqueVertices.end()) {
                uniqueVertices[key] = newIndex;
                newIndex++;
                //Vertex
            	int fixedID = (data[v].position -1) * 3;
                vertexList.push_back(vertices[fixedID + 0]);
                vertexList.push_back(vertices[fixedID + 1]);
                vertexList.push_back(vertices[fixedID + 2]);

                //Normals default to 0 if not defined in .obj
                if (data[v].normal >= 0) {
                	fixedID = (data[v].normal -1) * 3;
                    vertexList.push_back(normals[fixedID + 0]);
                    vertexList.push_back(normals[fixedID + 1]);
                    vertexList.push_back(normals[fixedID + 2]);
                } else {
                    vertexList.push_back(0.0f);
                    vertexList.push_back(0.0f);
                    vertexList.push_back(0.0f);
                }

                //UV also set to 0 if missing
                if (data[v].uv >= 0) {
                	fixedID = (data[v].uv -1) * 2;
                    vertexList.push_back(uvs[fixedID + 0]);
                    vertexList.push_back(uvs[fixedID + 1]);
                } else {
                    vertexList.push_back(0.0f);
                    vertexList.push_back(0.0f);
                }
            }
            else
            {
	            dupecount++;
            }
            vertexcount++;
            //Push the index
            indexList.push_back(uniqueVertices[key]);
        }
		auto stop = chrono::high_resolution_clock::now();
		
		auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
		cout <<"Custom loader done" << endl;
		cout << "Time taken: " << duration.count() << " microseconds" << endl;
		cout <<"dupe count: " << dupecount << endl;
		cout <<"vertex count: " << vertexcount << endl;
	}
}

void Mesh::setBuffers()
{
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices().size() * sizeof(float), &Vertices()[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices().size() * sizeof(unsigned int), &Indices()[0], GL_STATIC_DRAW);
	//Position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//UV attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

vector<float> Mesh::Vertices()
{
	return vertexList;
}

vector<unsigned int> Mesh::Indices()
{
	return indexList;
}

void Mesh::CleanUP()
{
	for (auto &mesh : loadedMeshes)
	{
		glDeleteBuffers(1, &mesh.second.VAO);
		mesh.second.indexList.clear();
		mesh.second.vertexList.clear();
	}
	loadedMeshes.clear();
}
