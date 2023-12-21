#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "ring.h"
#include "sphere.h"
#include "asteroid.h"
#include "mesh.h"
#include "skybox.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    bool observation_mode = false;
    void HierarchicalUpdate2(double dt);
    void Render();
    void set_mode(bool b);

    Camera* getCamera() { return m_camera; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);
    void ObservationModeUpdate(double dt);
    void EnterObservational();
    void ExitObservational();

    stack<glm::mat4> modelStack;
    glm::mat4 CameraProjectionBackup;
    glm::mat4 CameraViewBackup;
    glm::vec3 CameraPosBackup;
    glm::vec3 CameraFrontBackup;
    glm::vec3 CameraUpBackup;
    double xBackup, yBackup, zBackup;

    Camera *m_camera;
    Camera *backup_camera;
    Shader *m_shader;
    Shader* asteroid_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;


    Sphere* m_sphere;
    Sphere* m_sphere2;
    Sphere* m_sphere3;
    Sphere* m_sphere4;
    Sphere* m_sphere5;
    Sphere* m_sphere6;
    Asteroid* m_sphere7;
    Sphere* m_sphere8;
    Sphere* m_sphere9;
    Sphere* m_sphere10;
    Sphere* m_sphere11;
    Asteroid* m_sphere12;
    Asteroid* m_sphere13;

    Mesh* m_mesh;
    Ring* m_ring;
    SkyBox* m_skybox;



};

#endif /* GRAPHICS_H */
