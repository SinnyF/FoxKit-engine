#pragma once
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace glm;
using namespace std;
class CustomReader
{
public:
    struct Indexes
    {
        int position = -1;
        int normal = -1;
        int uv = -1;
    };
    static void ReadFile(vector<Indexes> &_indexes, string &path, vector<float> &vertices, vector<float> &normals, vector<float> &uvs)
    {
        ifstream file(path.c_str());
        if (!file.is_open())
        {
            cout<< "Failed to open file " << path << endl;
            return;
        }
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string prefix;
            ss >> prefix;
            vector<string> tokens;
            string token;
            while (ss >> token)
            {
                tokens.push_back(token);
            }
            if (prefix == "v")
            {
                if (tokens.size() != 3)
                {
                    cout << "wrong formating for vertex positions" << endl;
                    return;
                }
                vertices.push_back(stof(tokens[0]));
                vertices.push_back(stof(tokens[1]));
                vertices.push_back(stof(tokens[2]));
            }
            else if (prefix == "vt")
            {
                if (tokens.size() != 2)
                {
                    cout << "wrong formating for UVs" << endl;
                    return;
                }
                uvs.push_back(stof(tokens[0]));
                uvs.push_back(stof(tokens[1]));
            }
            else if (prefix == "vn")
            {
                if (tokens.size() != 3)
                {
                    cout << "wrong formating for normals" << endl;
                    return;
                }
                normals.push_back(stof(tokens[0]));
                normals.push_back(stof(tokens[1]));
                normals.push_back(stof(tokens[2]));
            }
            else if (prefix == "f")
            {
                if (tokens.size() < 3 || tokens.size() > 4)
                    cout<< "Unsupported face size, ignoring" << endl;
                else
                {
                    for (int i = 0; i < tokens.size(); i++)
                    {
                        stringstream tokenizer(tokens[i]);
                        int count = 0;
                        Indexes vertex;
                        while (getline(tokenizer, token, '/'))
                        {
                            if (count == 0)
                                vertex.position = stoi(token);
                            else if (count == 1)
                                vertex.uv = stoi(token);
                            else if (count == 2)
                                vertex.normal = stoi(token);
                            count++;
                        }
                        _indexes.push_back(vertex);
                        if(i == 2 && tokens.size() == 4)
                        {
                            Indexes one, three;
                            one = _indexes[_indexes.size() - 3];
                            three = _indexes[_indexes.size() - 1];
                            _indexes.push_back(one);
                            _indexes.push_back(three);
                        }
                    }
                }
            }
        }
    }
};
