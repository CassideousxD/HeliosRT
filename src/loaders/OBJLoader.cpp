#include "loaders/OBJLoader.h"
#include "primitives/Triangle.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <array>

std::shared_ptr<Mesh> OBJLoader::load(
    const std::string& filepath,
    std::shared_ptr<Material> material)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "OBJLoader: could not open " << filepath << std::endl;
        return nullptr;
    }

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<float>   texU, texV;
    std::vector<std::shared_ptr<Hittable>> triangles;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            positions.push_back(Vector3(x, y, z));
        }
        else if (token == "vn")
        {
            float x, y, z;
            ss >> x >> y >> z;
            normals.push_back(Vector3(x, y, z));
        }
        else if (token == "vt")
        {
            float u, v;
            ss >> u >> v;
            texU.push_back(u);
            texV.push_back(v);
        }
        else if (token == "f")
        {
            // parse up to 4 face indices (triangulate quads)
            std::vector<std::array<int,3>> faceIndices; // [pos, uv, normal]

            std::string vertToken;
            while (ss >> vertToken)
            {
                std::array<int,3> idx = {-1, -1, -1};
                std::istringstream vs(vertToken);
                std::string part;
                int i = 0;
                while (std::getline(vs, part, '/'))
                {
                    if (!part.empty())
                        idx[i] = std::stoi(part) - 1; // OBJ is 1-indexed
                    i++;
                }
                faceIndices.push_back(idx);
            }

            // triangulate — fan from first vertex
            for (size_t i = 1; i + 1 < faceIndices.size(); i++)
            {
                std::array<std::array<int,3>,3> tri = {
                    faceIndices[0], faceIndices[i], faceIndices[i+1]
                };

                TriangleVertex verts[3];
                for (int j = 0; j < 3; j++)
                {
                    int pi = tri[j][0];
                    int ti = tri[j][1];
                    int ni = tri[j][2];

                    verts[j].position = (pi >= 0) ? positions[pi] : Vector3();
                    verts[j].u        = (ti >= 0 && ti < (int)texU.size()) ? texU[ti] : 0.0f;
                    verts[j].v        = (ti >= 0 && ti < (int)texV.size()) ? texV[ti] : 0.0f;
                    verts[j].normal   = (ni >= 0 && ni < (int)normals.size())
                                        ? normals[ni]
                                        : Vector3();
                }

                triangles.push_back(std::make_shared<Triangle>(
                    verts[0], verts[1], verts[2], material));
            }
        }
    }

    std::cout << "OBJLoader: loaded " << triangles.size()
              << " triangles from " << filepath << std::endl;

    return std::make_shared<Mesh>(std::move(triangles));
}