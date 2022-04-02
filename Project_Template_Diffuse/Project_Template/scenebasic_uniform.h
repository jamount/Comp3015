#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include "helper/plane.h"
#include "helper/teapot.h"
#include "helper/torus.h"
#include "helper/objmesh.h"
#include "helper/cube.h"
#include <glm/glm.hpp>

class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;



    //GLuint fsQuad, fboHandle, renderTex;
    Plane plane;
    std::unique_ptr<ObjMesh> tallTree;
    std::unique_ptr<ObjMesh> rock;
    std::unique_ptr<ObjMesh> sword;
    std::unique_ptr<ObjMesh> crystal;
    //Torus torus;
    //Teapot teapot;
    float angle;
    float tPrev, rotSpeed;
    void setMatrices();
    void compile();
    //void setupFBO();
    //void pass1();
    //void pass2();


public:
    SceneBasic_Uniform();

    void initScene();
    void update( float delta );
    void render( glm::mat4 projection, glm::mat4 view);
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
