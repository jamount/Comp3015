#include "Terrain.h"
#include "glutils.h"
#include <glad/glad.h>
#include "glutils.h"
#include "HeightGenerator.h"
#include "PoissonGenerator.h"
#include <cstdio>
#include <cmath>

int objCount = 50;

Terrain::Terrain(float xsize, float zsize, int xdivs, int zdivs, float smax, float tmax)
{
    generate(xsize, zsize, xdivs, zdivs, smax, tmax);

    PoissonGenerator::DefaultPRNG PRNG;

    const auto Points = PoissonGenerator::generatePoissonPoints(objCount, PRNG, 1.0f); 

    for (int i = 0; i < Points.size(); i++)
    {
        objPos.push_back(glm::vec3(Points[i].x * xsize, 0.0f, Points[i].y * zsize));
    }

}

std::vector<glm::vec3> Terrain::getobjPos() {
    return objPos;
}


void Terrain::generate(float xsize, float zsize, int xdivs, int zdivs, float smax, float tmax) {
    HeightGenerator generator;

    int nPoints = (xdivs + 1) * (zdivs + 1);
    std::vector<GLfloat> p(3 * nPoints);
    std::vector<GLfloat> n(3 * nPoints);
    std::vector<GLfloat> tex(2 * nPoints);
    std::vector<GLfloat> tang(4 * nPoints);
    std::vector<GLuint> el(6 * xdivs * zdivs);

    float x2 = xsize / 2.0f;
    float z2 = zsize / 2.0f;
    float iFactor = (float)zsize / zdivs;
    float jFactor = (float)xsize / xdivs;
    float texi = smax / xdivs;
    float texj = tmax / zdivs;
    float x,y, z;
    int vidx = 0, tidx = 0;
    for (int i = 0; i <= zdivs; i++) {
        z = iFactor * i - z2;
        for (int j = 0; j <= xdivs; j++) {
            x = jFactor * j - x2;
            y = generator.generateHeight(x, z);

            p[vidx] = x;
            p[vidx + 1] = y;
            p[vidx + 2] = z;


            n[vidx] = 0.0f;
            n[vidx + 1] = 1.0f;
            n[vidx + 2] = 0.0f;

            tex[tidx] = j * texi;
            tex[tidx + 1] = (zdivs - i) * texj;

            vidx += 3;
            tidx += 2;
        }
    }

    for (int i = 0; i < nPoints; i++) {
        tang[i * 4 + 0] = 1.0f;
        tang[i * 4 + 1] = 0.0f;
        tang[i * 4 + 2] = 0.0f;
        tang[i * 4 + 3] = 1.0f;
    }

    GLuint rowStart, nextRowStart;
    int idx = 0;
    for (int i = 0; i < zdivs; i++) {
        rowStart = (GLuint)(i * (xdivs + 1));
        nextRowStart = (GLuint)((i + 1) * (xdivs + 1));
        for (int j = 0; j < xdivs; j++) {
            el[idx] = rowStart + j;
            el[idx + 1] = nextRowStart + j;
            el[idx + 2] = nextRowStart + j + 1;
            el[idx + 3] = rowStart + j;
            el[idx + 4] = nextRowStart + j + 1;
            el[idx + 5] = rowStart + j + 1;
            idx += 6;
        }
    }



    initBuffers(&el, &p, &n, &tex, &tang);
}
