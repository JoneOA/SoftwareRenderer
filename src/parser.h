#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "vector.h"

std::vector<vec3d> parseObj3d(std::string path, std::vector<int>& indicies)
{
    std::cout << path << std::endl;
    std::fstream file(path);
    std::string line;
    std::vector<vec3d> points;

    while(getline(file, line)) {
        vec3d point;
        size_t pos = 0;
        size_t posV = 0;
        size_t posF = 0;
        posV = line.find("v ");
        posF = line.find("f ");

        if(posV != -1){
            line.erase(0, pos + 2);
            pos = line.find(" ");
            point.x = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(" ");
            point.y = stod(line.substr(0, pos));
            line.erase(0, pos + 1);
            point.z = stod(line);

            points.push_back(point);
            continue;
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
    }

    return points;
}
