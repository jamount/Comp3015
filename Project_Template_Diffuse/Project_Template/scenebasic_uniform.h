#pragma once
#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "helper/plane.h"
#include "helper/Terrain.h"
#include "helper/teapot.h"
#include "helper/torus.h"
#include "helper/objmesh.h"
#include "helper/cube.h"
#include "helper/ParticleEmitterFountain.h"
#include "helper//Fire.h"
#include "helper/frustum.h"
#include <glm/glm.hpp>

class SceneBasic_Uniform : public Scene
{
    enum objectType
    {
        Rock,
        Tree
    };

    struct Object{
        glm::vec3 pos;
        objectType type;
    };

private:
    GLSLProgram shadowVolume, shadowRender, shadowComp;

    GLuint colorDepthFBO, fsQuad;

    GLuint texTree, texRock, texGrass;



    glm::vec4 lightPos;

    Terrain terrain;
    //ParticleEmitterFountain fountain;
    //Fire fire;
    std::unique_ptr<ObjMesh> tallTree;
    std::unique_ptr<ObjMesh> rock;
    std::unique_ptr<ObjMesh> sword;
    std::unique_ptr<ObjMesh> crystal;
    float angle;
    float tPrev, rotSpeed;

    std::vector<glm::vec3> objectPos;
    std::vector<Object> objects;

    void setMatrices(GLSLProgram&);
    void compile();
    void setupFBO();

    void drawScene(GLSLProgram&, bool);
    void drawObject(Object obj, GLSLProgram& prog);

    void pass1();
    void pass2();
    void pass3();
    void updateLight();

    void regenerate(); 
    void skiptime();


public:
    SceneBasic_Uniform();

    void initScene();
    void update( float delta );
    void render( glm::mat4 projection, glm::mat4 view);
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
