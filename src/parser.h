#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "vector.h"

bool parseObj3d(std::string path, std::vector<vec3d>& verticies, std::vector<int>& indicies, std::vector<vec3d>& normals)
{
    std::cout << path << std::endl;
    std::fstream file(path);
    std::string line;

    if(!file.is_open())
        return false;

    while(getline(file, line)) {
        vec3d point;
        size_t pos = 0;
        size_t posV = 0;
        size_t posF = 0;
        size_t posN = 0;
        posV = line.find("v ");
        posF = line.find("f ");
        posN = line.find("vn ");

        if(posV != -1){
            line.erase(0, pos + 2);
            pos = line.find(" ");
            point.x = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(" ");
            point.y = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            point.z = stod(line);

            verticies.push_back(point);
        }
        else if(posF != -1){
            line.erase(0, pos + 2);
            pos = line.find("/");
            int i1 = stoi(line.substr(0, pos));
            pos = line.find(" ");
            line.erase(0, pos + 1);
            pos = line.find("/");
            int i2 = stoi(line.substr(0, pos));
            pos = line.find(" ");
            line.erase(0, pos + 1);
            pos = line.find("/");
            int i3 = stoi(line.substr(0, pos));
            pos = line.find(" ");
            line.erase(0, pos + 1);
            pos = line.find("/");
            int i4 = stoi(line.substr(0, pos));

            indicies.push_back(i1);
            indicies.push_back(i2);
            indicies.push_back(i3);

            indicies.push_back(i1);
            indicies.push_back(i3);
            indicies.push_back(i4);
        }
        else if(posN != -1){
            vec3d normal;
            line.erase(0, pos + 3);
            pos = line.find(" ");
            normal.x = stod(line.substr(0, pos));
            line.erase(0, pos+1);
            pos = line.find(" ");
            normal.y = stod(line.substr(0, pos));
            line.erase(0, pos+1);
            pos = line.find(" ");
            normal.z = stod(line.substr(0, pos));
            line.erase(0, pos+1);

            normals.push_back(normal);
        }
    }

    return true;
}
