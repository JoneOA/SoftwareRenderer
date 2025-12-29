#pragma once
#include <vector>
#include <string>

#include <fstream>

#include "vector.h"

namespace ObjParser 
{
    static std::vector<vec3d> ObjToVec3FromPath(std::string path, std::vector<int>& indexData){
        std::string text;
        std::ifstream objFile(path, std::ios::in);

        std::vector<vec3d> vertexData;

        while(getline(objFile, text)){
            std::string data;
            auto pos = text.find(" ");
            if(text.substr(0, pos) == "v") {
                text.erase(0, pos + 1);
                vec3d point;
                pos = text.find(" ");
                point.x = stod(text.substr(0, pos));
                text.erase(0, pos + 1);
                pos = text.find(" ");
                point.y = stod(text.substr(0, pos));
                text.erase(0, pos + 1);
                pos = text.find(" ");
                point.z = stod(text.substr(0, pos));
                text.erase(0, pos + 1);
                vertexData.push_back(point);
            }
            else if(text.substr(0, pos) == "f"){
                text.erase(0, pos + 1);
                pos = text.find("/");
                int index1 = stoi(text.substr(0, pos)) - 1;
                pos = text.find(" ");
                text.erase(0, pos + 1);
                pos = text.find("/");
                int index2 = stoi(text.substr(0, pos)) - 1;
                pos = text.find(" ");
                text.erase(0, pos + 1);
                pos = text.find("/");
                int index3 = stoi(text.substr(0, pos)) - 1;
                pos = text.find(" ");
                text.erase(0, pos + 1);
                pos = text.find("/");
                int index4 = stoi(text.substr(0, pos)) - 1;
                    
                indexData.push_back(index1);
                indexData.push_back(index2);
                indexData.push_back(index3);

                indexData.push_back(index1);
                indexData.push_back(index3);
                indexData.push_back(index4);
            }
        }
        return vertexData;
    }
}

