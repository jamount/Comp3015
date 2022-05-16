#pragma once

#include "trianglemesh.h"
#include <vector>;
#include <glm/glm.hpp>

class Terrain : public TriangleMesh
{
private:



    std::vector<glm::vec3> objPos;


    void generate(float xsize, float zsize, int xdivs, int zdivs, float smax, float tmax);


public:
    Terrain(float xsize, float zsize, int xdivs, int zdivs, float smax = 1.0f, float tmax = 1.0f);

    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec3> getobjPos();

};
