#ifndef SKYBOX
#define SKYBOX
#include <string>
#include "camera.h"

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"

class SkyBox {
    // prototype public methods
public:
    SkyBox();
    SkyBox(const char* fname);
    ~SkyBox();
    void Initialize(GLint posAttribLoc, GLint colAttribLoc);
    void Update(unsigned int dt);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);
    void createVertices();
    void setupBuffers();
    void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);

    glm::mat4 GetModel();

    void setSpeed(glm::vec3 spd, float rt) { m_speed = spd; m_rotate = rt; }
    GLuint getTextureID() { return m_texture->getTextureID(); }

    bool hasTex;


    // prototype private attibutes
private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    //std::vector<glm::vec3> vertices;
    std::vector<unsigned int> Indices;
    glm::vec3 pivotLocation;

    GLuint vao;

    GLuint VB;
    GLuint IB;
    Texture* m_texture;

    glm::vec3 m_speed = glm::vec3(0., 0., 0.);
    float m_rotate = 0.0f;

    float angle;
};


#endif